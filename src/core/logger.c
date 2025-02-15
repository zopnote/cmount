#include <core.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

logger_t* logger_create(
    const char* name,
    const bool verbose,
    const bool should_print_in_console,
    const logger_callback_t logger_log_function
) {

    time_t raw_time;
    time(&raw_time);
    struct tm* time_info = localtime(&raw_time);
    logger_t* logger = malloc(
        sizeof(logger_t)
    );

    if (!logger) return NULL;

    *logger = (logger_t) {
        .name = name,
        .verbose = verbose,
        .should_print_in_console = should_print_in_console,
        .logger_log_function = logger_log_function
    };

    logger->time = time_info;

    return logger;
}

static bool move_and_rename_log_file(
    FILE* file,
    const char* path_of_file
) {
    char directory_of_file_path[
        strlen(path_of_file) * sizeof(char) + 1
    ];
    directory_of_file_path[0] = '\0';

    for (size_t i = strlen(path_of_file); i > 0; i--) {
        if (
            path_of_file[i] == '\\' ||
            path_of_file[i] == '/'
        ) {
            strncpy(directory_of_file_path, path_of_file, i);
        }
    }

    char current_character;
    long last_bracket_opening = -1;
    long last_bracket_closing = -1;
    while ((current_character = fgetc(file)) != EOF) {

        if (current_character == '(') {
            last_bracket_opening = ftell(file);
            continue;
        }

        if (current_character == ')') {
            last_bracket_closing = ftell(file);
        }
    }
    if (last_bracket_opening == -1 ||
        last_bracket_closing == -1 ||
        last_bracket_opening >= last_bracket_closing
    ) {
        return false;
    }

    fseek(
        file,
        last_bracket_opening,
        SEEK_SET
    );
    const size_t bracket_content_size =
        last_bracket_closing - last_bracket_opening - 1;
    char bracket_buffer[bracket_content_size + 12];

    fread(
        bracket_buffer, 1,
        bracket_content_size,
        file
    );
    bracket_buffer[bracket_content_size] = '\0';


    sprintf(bracket_buffer, strlwr(bracket_buffer));
    strcat(bracket_buffer, ".log");

    for (size_t i = 0; i < bracket_content_size; i++) {
        if (bracket_buffer[i] == ':') {
            bracket_buffer[i] = '-';
        }
        if (bracket_buffer[i] == ',') {
            bracket_buffer[i] = '_';
        }
        if (bracket_buffer[i] == ' ') {
            bracket_buffer[i] = '_';
        }
    }

    char new_file_path[
        strlen(directory_of_file_path) * sizeof(char) +
        strlen(bracket_buffer) * sizeof(char) + 7
    ];
    new_file_path[0] = '\0';

    sprintf(
        new_file_path,
        "%s/logs",
        directory_of_file_path
    );
    os_make_directory(new_file_path);
    strcat(new_file_path, "/");
    strcat(new_file_path, bracket_buffer);

    return rename(path_of_file, new_file_path) == 0;
}


void logger_create_file_target(
    logger_t* logger,
    const bool file_should_be_named_after_logger,
    const char* directory_path
) {

    const char* standard_log_name = "latest.log";
    const char* name_of_logger = logger->name;
    char file_name[
        strlen(standard_log_name) * sizeof(char) +
        strlen(name_of_logger) * sizeof(char) + 1
    ];
    file_name[0] = '\0';


    if (file_should_be_named_after_logger) {

        char prefix[
            strlen(name_of_logger) * sizeof(char) + 1
        ];
        char* name_of_logger_copy = strlwr(
            strdup(name_of_logger)
        );

        sprintf(prefix, "%s.", strlwr(name_of_logger_copy));
        strcat(file_name, prefix);

        free(name_of_logger_copy);
    }
    strcat(file_name, "latest.log");


    char file_path[
        strlen(directory_path) * sizeof(char) +
        strlen(file_name) * sizeof(char) + 1
    ];
    file_path[0] = '\0';
    sprintf(file_path, "%s/%s", directory_path, file_name);


    if (os_can_access_file(file_path)) {
        FILE* openedFile = fopen(file_path, "r");
        const bool is_cmount_logger_log =
            move_and_rename_log_file(openedFile, file_path);

        fclose(openedFile);
        if (!is_cmount_logger_log) {
            remove(file_path);
        }
    }
    printf("AGZREYJTYRGHRYES");
    logger->file = fopen(file_path, "a");
}


void logger_write(
    logger_t* logger,
    const logger_significance_t significance,
    const char* format,
    ...
) {
    char* args = NULL;
    va_start(args);

    char message_buffer[
        strlen(format) * sizeof(char) + 360
    ];
    vsprintf(
        message_buffer,
        format, args
    );
    va_end(args);


    size_t meta_of_message_size =
        strlen(logger->name) * sizeof(char) + 24;
    char meta_of_message_buffer[meta_of_message_size];

    time_t raw_time;
    time(&raw_time);
    struct tm* time_info = localtime(&raw_time);

    sprintf(
        meta_of_message_buffer,
        "%d:%d:%d, %d-%d-%d, %s",
        time_info->tm_hour,
        time_info->tm_min,
        time_info->tm_sec,
        time_info->tm_mday,
        time_info->tm_mon + 1,
        time_info->tm_year + 1900,
        logger->name
    );
    logger->time = time_info;

    char significance_of_message_buffer[8];
    significance_of_message_buffer[0] = '\0';
    bool should_print_in_console = false;
    int message_color = 0;

    switch (significance) {
        case error:
            strcat(significance_of_message_buffer, "Error");
            message_color = get_color(red).ansi_color_code;
            should_print_in_console = true;
            break;
        case warning:
            strcat(significance_of_message_buffer, "Warning");
            message_color = get_color(yellow).ansi_color_code;
            should_print_in_console = logger->should_print_in_console;
            break;
        case status:
            strcat(significance_of_message_buffer, "Status");
            message_color = 97;
            should_print_in_console = logger->should_print_in_console;
            break;
        case info:
            strcat(significance_of_message_buffer, "Info");
            should_print_in_console =
                logger->should_print_in_console && logger->verbose;
            break;
        default: break;
    }



    char constructed_message_buffer[
        strlen(message_buffer) * sizeof(char) +
        strlen(meta_of_message_buffer) * sizeof(char) +
        56 * sizeof(char)
    ];

    if (logger->file) {

        sprintf(
            constructed_message_buffer,
            "%s(%s): %s\n",
            significance_of_message_buffer,
            meta_of_message_buffer,
            message_buffer
        );

        fwrite(
            constructed_message_buffer,
            sizeof(char),
            strlen(constructed_message_buffer),
            logger->file
        );

        memset(
            constructed_message_buffer,
            0, sizeof(constructed_message_buffer)
        );
    }

    if (should_print_in_console) {
        sprintf(
            constructed_message_buffer,
            "\x1B[%dm%s\x1B[%dm(%s): %s\n",
            message_color,
            significance_of_message_buffer,
            none, meta_of_message_buffer,
            message_buffer
        );

        fprintf(stdout, constructed_message_buffer);
        fflush(stdout);
    }
}

void logger_cleanup_logs(
    const char* logs_directory_path
) {
    char* file_paths_buffer[1024];
    const int found_files_count = os_get_directory_files(
        &file_paths_buffer,
        logs_directory_path
    );

    for (int i = 0; i < found_files_count; i++) {
        printf(
            "\n%d. File found: %s\n",
            i, file_paths_buffer[i]
        );
        free(file_paths_buffer[i]);
    }
    free(file_paths_buffer);
}