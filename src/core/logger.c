#include <core.h>
#include <stdlib.h>
#include <string.h>

logger_t* logger_create(
    const char* prefix,
    const bool verbose,
    const bool should_print_in_console,
    const logger_callback_t logger_write_function
) {

    const time_t t = time(NULL);

    logger_t* logger = malloc(sizeof logger_t);

    if (!logger) return NULL;

    *logger = (logger_t) {
        .name = prefix,
        .verbose = verbose,
        .should_print_in_console = should_print_in_console,
        .logger_write_function = logger_write_function
    };

    localtime_s(logger->time, &t);

    return logger;
}

void logger_setFile(
    logger_t* logger,
    const bool save_in_independent_file,
    const char* directory_path
) {

    const char* standardLogName = "latest.log";
    char fileName[
        strlen(standardLogName) * sizeof(char) +
        strlen(logger->name) * sizeof(char) + 1
    ]; fileName[0] = '\0';


    if (save_in_independent_file) {
        char prefix[
            strlen(logger->name) * sizeof(char) + 1];

        sprintf(prefix, "%s.", logger->name);

        strcat(fileName, prefix);
    }
    strcat(fileName, "latest.log");


    char filePath[
        strlen(directory_path) * sizeof(char) +
        strlen(fileName) * sizeof(char) + 1
    ]; filePath[0] = '\0';

    sprintf(filePath, "%s/%s", directory_path, fileName);
    printf("File path is %s.", filePath);

    if (!access(directory_path, 0)) {
        FILE* openedFile = fopen(filePath, "r");

        char current;
        long lastFoundBracketOpeningIndex = -1;
        long lastFoundBracketClosingIndex = -1;
        while ((current = fgetc(openedFile)) != EOF) {
            if (current == '(') {
                lastFoundBracketOpeningIndex = ftell(openedFile) - 1;
            }
            else if (current == ')') {
                lastFoundBracketClosingIndex = ftell(openedFile) - 1;
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
        char buffer[contentSize + 1];

        fread(buffer, 1, contentSize, openedFile);
        buffer[contentSize] = '\0';

        printf("%s", buffer);
        fclose(openedFile);
    }
}