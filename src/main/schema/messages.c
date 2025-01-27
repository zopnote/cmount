#include "messages.h"

#include <yaml.h>

#define MESSAGE_KEYS_END "G86KF47PT65I8"

typedef struct {
    char* name;
    Message* message;
} MessageKey;


static void parserMessagesScan(yaml_parser_t parser, Messages* messages) {

    MessageKey keys[] = {
        {"help", &messages->help},
        {"not_found", &messages->not_found},
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
            lastKey =
                strdup(value.data.scalar.value);
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

void schema_printMessageWithVariables(
    const MessagePlaceholder variables[],
    const int variablesCount,
    const Message message)
{
    for (size_t lineI = 0; lineI < message.length; lineI++) {
        char* line = message.lines[lineI];

        for (size_t charI = 0; charI < strlen(line); charI++) {

            if (line[charI] != '$') {
                return;
            }

            if (!line[charI + 1]) {
                return;
            }

            if (line[charI + 1] != '{') {
                return;
            }

            int varEndIndex = charI + 2;
            while (line[varEndIndex] != '}') {
                varEndIndex++;
                if (line[varEndIndex] == '\0') {
                    printf("Error while parsing messages.\n"
                           "%s\n\n"
                           "The variable starting at %llu is not closed.\n"
                           "As result the message cannot be printed.", line, charI);
                    break;
                }
            }
            varEndIndex++;

        }
        printf("%s\n", line);
        fflush(stdout);
    }
}
