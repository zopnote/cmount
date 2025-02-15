#include <core.h>
#include <stdlib.h>
#include <string.h>

logger_t* logger_create(
    const char* name,
    const bool verbose,
    const bool should_print_in_console,
    const logger_callback_t logger_log_function
) {

    time_t raw_time;
    time(&raw_time);
    struct tm* time_info = localtime(&raw_time);
    logger_t* logger = malloc(sizeof(logger_t));

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

void logger_create_file_target(
    logger_t* logger,
    const bool name_file_after_logger_name,
    const char* directory_path
) {

    const char* standardLogName = "latest.log";
    char fileName[
        strlen(standardLogName) * sizeof(char) +
        strlen(logger->name) * sizeof(char) + 1
    ]; fileName[0] = '\0';


    if (name_file_after_logger_name) {
        char prefix[
            strlen(logger->name) * sizeof(char) + 1];

        char* name = strlwr(strdup(logger->name));
        sprintf(prefix, "%s.", strlwr(name));
        free(name);

        strcat(fileName, prefix);
    }
    strcat(fileName, "latest.log");


    char file_path[
        strlen(directory_path) * sizeof(char) +
        strlen(fileName) * sizeof(char) + 1
    ]; file_path[0] = '\0';

    sprintf(file_path, "%s/%s", directory_path, fileName);
    printf("File path is %s\n", file_path);

    if (os_can_access_file(file_path)) {
        FILE* openedFile = fopen(file_path, "r");

        char current;
        long lastFoundBracketOpeningIndex = -1;
        long lastFoundBracketClosingIndex = -1;
        while ((current = fgetc(openedFile)) != EOF) {
            if (current == '(') {
                lastFoundBracketOpeningIndex = ftell(openedFile);
            }
            else if (current == ')') {
                lastFoundBracketClosingIndex = ftell(openedFile);
            }
        }

        if (lastFoundBracketOpeningIndex == -1 ||
            lastFoundBracketClosingIndex == -1 ||
            lastFoundBracketOpeningIndex >= lastFoundBracketClosingIndex
        ) {
            return;
        }

        fseek(openedFile, lastFoundBracketOpeningIndex, SEEK_SET);
        const size_t contentSize = lastFoundBracketClosingIndex -
            lastFoundBracketOpeningIndex - 1;
        char buffer[contentSize + 12];

        fread(buffer, 1, contentSize, openedFile);
        buffer[contentSize] = '\0';
        sprintf(buffer, strlwr(buffer));
        strcat(buffer, ".txt");
        for (size_t i = 0; i < contentSize; i++) {
            if (buffer[i] == ':') {
                buffer[i] = '-';
            }
            if (buffer[i] == ',') {
                buffer[i] = '_';
            }
            if (buffer[i] == ' ') {
                buffer[i] = '_';
            }
        }
        printf("%s\n", buffer);
        fclose(openedFile);

        char new_file_path[
            strlen(directory_path) * sizeof(char) +
            strlen(buffer) * sizeof(char) + 7
        ]; new_file_path[0] = '\0';

        sprintf(new_file_path, "%s/logs", directory_path);
        os_make_directory(new_file_path);
        strcat(new_file_path, "/");
        strcat(new_file_path, buffer);
        rename(file_path, new_file_path);
    }
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
    char message_buffer[strlen(format) * sizeof(char) + 360];
    vsprintf(message_buffer, format, args);
    va_end(args);


    size_t properties_buffer_size =
        strlen(logger->name) * sizeof(char) + 24;
    char properties_buffer[properties_buffer_size];



    time_t raw_time;
    time(&raw_time);
    struct tm* time_info = localtime(&raw_time);

    sprintf(
        properties_buffer,
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





    char significance_name_buffer[8];
    significance_name_buffer[0] = '\0';
    bool print_to_console = false;
    int color_code = 0;

    switch (significance) {
        case error:
            strcat(significance_name_buffer, "Error");
            color_code = get_color(red).ansi_color_code;
            print_to_console = true;
            break;
        case warning:
            strcat(significance_name_buffer, "Warning");
            color_code = get_color(yellow).ansi_color_code;
            print_to_console = logger->should_print_in_console;
            break;
        case status:
            strcat(significance_name_buffer, "Status");
            color_code = 97;
            print_to_console = logger->should_print_in_console;
            break;
        case info:
            strcat(significance_name_buffer, "Info");
            print_to_console =
                logger->should_print_in_console && logger->verbose;
            break;
        default: break;
    }



    char final_message_buffer[
        strlen(message_buffer) * sizeof(char) +
        strlen(properties_buffer) * sizeof(char) +
        56 * sizeof(char)
    ];

    if (logger->file) {

        sprintf(
            final_message_buffer,
            "%s(%s): %s\n",
            significance_name_buffer,
            properties_buffer,
            message_buffer
        );

        fwrite(
            final_message_buffer,
            sizeof(char),
            strlen(final_message_buffer),
            logger->file
        );

        memset(
            final_message_buffer,
            0,
            sizeof(final_message_buffer)
        );
    }

    if (print_to_console) {

        sprintf(
            final_message_buffer,
            "\x1B[%dm%s\x1B[%dm(%s): %s\n",
            color_code,
            significance_name_buffer,
            none,
            properties_buffer,
            message_buffer
        );

        fprintf(stdout, final_message_buffer);
        fflush(stdout);
    }
}