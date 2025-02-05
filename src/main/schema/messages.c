#include "messages.h"

#include <yaml.h>

#define MESSAGE_KEYS_END "G86KF47PT65I8"

// Private type to save the available messages more efficient
typedef struct {
    char* name;
    Message* message;
} MessageKey;

// Processes the String given by parser in to messages.
static void parserMessagesScan(yaml_parser_t parser, Messages* messages) {

    MessageKey keys[] = {
        {"help", &messages->help},
        {"not_found", &messages->not_found},
        {"create_no_project_name", &messages->create_no_project_name},
        {"create_invalid_project_name", &messages->create_invalid_project_name},
        {MESSAGE_KEYS_END, NULL},
    };

    yaml_token_t event, value;

    char* lastKey = NULL;

    while (yaml_parser_scan(&parser, &event)) {

        if (event.type == YAML_STREAM_END_TOKEN) break;

        if (event.type == YAML_KEY_TOKEN) {

            if (!yaml_parser_scan(&parser, &value)) {
                yaml_token_delete(&event);
                yaml_token_delete(&value);
                continue;
            }

            if (value.type != YAML_SCALAR_TOKEN) {
                fprintf(
                    stderr,
                    "Unexpected token after key\n");
                yaml_token_delete(&event);
                yaml_token_delete(&value);
                continue;
            }

            if (lastKey) {
                free(lastKey);
            }
            lastKey = strdup(value.data.scalar.value);
        }

        if (event.type != YAML_BLOCK_SEQUENCE_START_TOKEN) {
            yaml_token_delete(&event);
            continue;
        }

        for (int i = 0; strcmp(MESSAGE_KEYS_END, keys[i].name); i++) {
            if (lastKey && !strcmp(lastKey, keys[i].name)) {
                int lineLength = 0;

                while (lineLength < MESSAGE_LINES_LENGTH) {
                    if (!yaml_parser_scan(&parser, &value)) {
                        break;
                    }

                    if (value.type == YAML_BLOCK_END_TOKEN) {
                        break;
                    }

                    if (value.type == YAML_SCALAR_TOKEN)
                        {
                        keys[i].message->lines[lineLength] =
                            strdup(value.data.scalar.value);

                        lineLength++;
                    }
                }
                keys[i].message->length = lineLength;
            }
        }
        yaml_token_delete(&event);
        yaml_token_delete(&value);
    }

    if (lastKey) {
        free(lastKey);
    }
}

// Parse the file buffer into messages
static Messages parseMessages(const char* fileBuffer) {

    yaml_parser_t parser;
    if (!yaml_parser_initialize(&parser)) {
        fprintf(stderr, "Failed to initialize parser\n");
    }

    yaml_parser_set_input_string(
        &parser, fileBuffer, strlen(fileBuffer));

    Messages messages = {
        .help = {0},
        .not_found = {0}
    };

    parserMessagesScan(parser, &messages);

    yaml_parser_delete(&parser);
    return messages;
}

Messages schema_getMessages(const char* filePath)
{
    char* fileBuffer = NULL;
    FILE* file = fopen(filePath, "r");

    if (!file) {
        perror("Failed to open file");
    }else {
        fseek(file, 0, SEEK_END);
        const long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);
        fileBuffer = calloc(fileSize + 1, fileSize);
        if (fileBuffer)
        {
            const size_t bytesRead = fread(fileBuffer, 1, fileSize, file);
            fileBuffer[bytesRead] = '\0';
        }
        fclose(file);
    }
    Messages messages = {};
    if (fileBuffer) {
        messages = parseMessages(fileBuffer);
    }
    free(fileBuffer);
    return messages;
}

void schema_printMessage(const Message message) {

    for (size_t i = 0; i < message.length; i++) {

        const char* line = message.lines[i];
        printf("%s\n", line);
        fflush(stdout);
    }
}

// Replaces all Strings, found in the buffer identical
// to the search String, with the replace String.
static char* stringReplace(
    const char* search,
    const char* replace,
    char* string
    ) {
    const char* searchStart =
        strstr(string, search);

    if(searchStart == NULL) {
        return string;
    }


    char* tempString =
        malloc((strlen(string) + 1) * sizeof(char));

    if(tempString == NULL) {
        return NULL;
    }

    int len = 0;

    strcpy(tempString, string);


    len = searchStart - string;
    string[len] = '\0';

    strcat(string, replace);

    len += strlen(search);
    strcat(string, tempString+len);

    free(tempString);
    return string;
}

void schema_printMessageWithVariables(
    const MessagePlaceholder variables[],
    const int variablesCount,
    const Message message)
{
    for (size_t lineI = 0; lineI < message.length; lineI++) {
        char* line = message.lines[lineI];
        for (int i = 0; i < variablesCount; i++) {
            char searchFor[255];
            sprintf(searchFor, "${%s}", variables[i].name);
            line = stringReplace(searchFor, variables[i].value, line);
        }
        printf("%s\n", line);
        fflush(stdout);
    }
}
