#include <core.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static void format_digit(const int a, char* buffer) {
    sprintf(buffer, "0%d", a);
    if (a >= 10) {
        strncpy(buffer, buffer + 1, 2);
    }
    buffer[2] = '\0';
}

static void str_of_time(const struct tm* time, char* buffer) {

    char hour_buffer[4];
    format_digit(time->tm_hour, hour_buffer);

    char min_buffer[4];
    format_digit(time->tm_min, min_buffer);

    char sec_buffer[4];
    format_digit(time->tm_sec, sec_buffer);

    char day_buffer[4];
    format_digit(time->tm_mday, day_buffer);

    char mon_buffer[4];
    format_digit(time->tm_mon + 1, mon_buffer);

    sprintf(
        buffer,
        "%s:%s:%s, %s-%s-%d",
        hour_buffer,
        min_buffer,
        sec_buffer,
        day_buffer,
        mon_buffer,
        time->tm_year + 1900
    );
}

static void str_to_fstr(char* str) {
    for (size_t i = 0; i < strlen(str); i++) {
        if (str[i] == ':') {
            str[i] = '-';
            continue;
        }

        if (
            str[i] == ',' ||
            str[i] == ' '
        ) {
            str[i] = '_';
        }
    }
}

static struct tm* time_of_fstr(const char* fstr) {

    struct tm* time = {0};

    if (sscanf(
        fstr, "%d-%d-%d__%d-%d-%d__",
        &time->tm_hour, &time->tm_min, &time->tm_sec,
        &time->tm_mday, &time->tm_mon, &time->tm_year
    ) != 6) {
        printf("Error while parsing time for the logger!\n");
        return NULL;
    }
    return time;
}

logger_t* logger_create(
    const char* name,
    const bool verbose,
    const bool print_stdout,
    const logger_callback_t log_func
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
        .print_out = print_stdout,
        .log_func = log_func
    };

    logger->time = time_info;

    return logger;
}

void logger_add_file_target(logger_t* logger, FILE* file) {
    logger->own_file = false;
    logger->file = file;
}

static bool get_meta_of_last_message(FILE* file, char* buffer) {
    char current_char;
    long last_bracket_opening = -1;
    long last_bracket_closing = -1;
    while ((current_char = fgetc(file)) != EOF) {

        if (current_char == '(') {
            last_bracket_opening = ftell(file);
            continue;
        }

        if (current_char == ')') {
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

    const size_t content_size =
        last_bracket_closing - last_bracket_opening - 1;

    fread(
        buffer, 1,
        content_size,
        file
    );
    buffer[content_size] = '\0';

    sprintf(buffer, "%s", str_lwr(buffer));
    return true;
}

static bool move_log(const char* file_path) {
    FILE* file = fopen(file_path, "r");

    char dir_path[strlen(file_path) + 76];
    if (!parent_path(file_path, dir_path)) {
        printf("parent_path");
        return false;
    }

    char bracket_buffer[64];
    if (!get_meta_of_last_message(file, bracket_buffer)) {
        printf("get_meta_of_last_message");
        return false;
    }
    printf("%s", dir_path);
    str_to_fstr(bracket_buffer);
    strcat(bracket_buffer, ".log");
    strcat(dir_path, "/logs");

    mk_dir(dir_path);

    strcat(dir_path, "/");
    strcat(dir_path, bracket_buffer);
    printf("%s", bracket_buffer);
    if (!cpy_file(file, dir_path)) {
        printf("cpy_file");
        return false;
    }

    fclose(file);
    if (!remove(file_path)) {
        printf("remove");
        return false;
    }
    return true;
}


void logger_create_file_target(
    logger_t* logger,
    const bool name_file,
    const char* dir_path
) {

    const char* standard_log_name = "latest.log";
    const char* name_of_logger = logger->name;
    char file_name[
        strlen(standard_log_name) * sizeof(char) +
        strlen(name_of_logger) * sizeof(char) + 1
    ];
    file_name[0] = '\0';


    if (name_file) {

        char prefix[
            strlen(name_of_logger) * sizeof(char) + 1
        ];
        char* name_of_logger_copy = str_lwr(
            strdup(name_of_logger)
        );

        sprintf(prefix, "%s.", str_lwr(name_of_logger_copy));
        strcat(file_name, prefix);

        free(name_of_logger_copy);
    }
    strcat(file_name, "latest.log");


    char file_path[
        strlen(dir_path) * sizeof(char) +
        strlen(file_name) * sizeof(char) + 1
    ];
    file_path[0] = '\0';
    sprintf(file_path, "%s/%s", dir_path, file_name);


    if (can_access(file_path)) {
        move_log(file_path);
    }
    logger->own_file = true;
    logger->file = fopen(file_path, "a");
}


void logger_write(
    logger_t* logger,
    const logger_significance_t significance,
    const char* format,
    ...
) {
    #if _WIN32
        char* args = NULL;
    #else
        va_list args;
    #endif
    va_start(args, format);
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


    str_of_time(time_info, meta_of_message_buffer);
    sprintf(
        meta_of_message_buffer,
        "%s, %s",
        meta_of_message_buffer,
        logger->name
    );
    logger->time = time_info;

    char significance_of_message_buffer[8];
    significance_of_message_buffer[0] = '\0';
    bool should_print_in_console = false;

    switch (significance) {
        case error:
            strcat(
                significance_of_message_buffer, "Error");
            should_print_in_console = true;

            break;
        case warning:
            strcat(
                significance_of_message_buffer, "Warning");
            should_print_in_console =
                logger->print_out;

            break;
        case status:
            strcat(
                significance_of_message_buffer, "Status");
            should_print_in_console =
                logger->print_out;

            break;
        case info:
            strcat(
                significance_of_message_buffer, "Info");
            should_print_in_console =
                logger->print_out &&
                    logger->verbose;

            break;
        default: break;
    }


    if (logger->file) {
        char constructed_message_buffer[
            strlen(message_buffer) * sizeof(char) +
            strlen(meta_of_message_buffer) * sizeof(char) +
            56 * sizeof(char)
        ];

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
    }

    if (should_print_in_console) {
        printf(
            "\n%s",
            message_buffer
        );
    }
}

struct file_name_time_map_s {
    char* key;
    int* value;
};

void logger_cleanup_logs(
    const char* logs_dir_path,
    const int max_allowed_log_files
) {
    char** files = NULL;
    const int file_count = get_dir_files(
        &files,
        logs_dir_path
    );

    if (file_count > 0 && files != NULL) {
        for (int i = 0; i < file_count; i++) {
            if (!files[i]) {
                continue;
            }
            for (size_t k = strlen(files[i]); k > 0; k--) {
                if (files[i][k] == '/' || files[i][k] == '\\') {

                }
            }
            free(files[i]);
        }
        free(files);
    }

}

void logger_dispose(logger_t* logger) {
    if (!logger) return;

    if (!logger->time) {
        free(logger->time);
    }

    if (logger->file) {
        logger->log_func(
            logger,
            info,
            "Disposal of logger %s.",
            logger->name
        );
        if (logger->own_file) {
            fclose(logger->file);
        }
    }

    logger = NULL;
}
