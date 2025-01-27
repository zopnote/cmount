#include "messages.h"

#include <yaml.h>

#define KEYS_END "G86KF47PT65I8"
char* keys[] {
    "help",
    "not_found",
    KEYS_END
};

static Messages parserMessagesScan(yaml_parser_t parser) {

    yaml_token_t event, value;

    char* lastKey = NULL;

    Message rawMessages[255];
    int messageIndex = 0;

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

        for (int i = 0; strcmp(KEYS_END, keys[i]); i++) {
            if (lastKey && !strcmp(lastKey, keys[i])) {
                int lineLength = 1;
                char* lines[255];
                lines[0] = keys[i];
                while (true) {

                    if (!yaml_parser_scan(&parser, &value)) {
                        break;
                    }

                    if (value.type == YAML_BLOCK_END_TOKEN) {
                        break;
                    }

                    if (value.type == YAML_SCALAR_TOKEN)
                        {
                        lines[lineLength] =
                            strdup(value.data.scalar.value);

                        lineLength++;
                    }
                }
                Message message = {
                    .lines = lines,
                    .length = lineLength
                };
                rawMessages[messageIndex] = message;
                messageIndex++;
            }
        }

        yaml_token_delete(&event);
        yaml_token_delete(&value);
    }
    Messages messages = {

    };
    if (lastKey) free(lastKey);
}


static Messages parseMessages(const char* fileBuffer) {

    yaml_parser_t parser;
    if (!yaml_parser_initialize(&parser)) {
        fprintf(stderr, "Failed to initialize parser\n");
    }

    yaml_parser_set_input_string(
        &parser, fileBuffer, strlen(fileBuffer));


    parserMessagesScan(parser);

    yaml_parser_delete(&parser);

    Messages messages = {};

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
    for (size_t i = 0; i < message.n; i++)
    {
        const char* line = kv_A(message, i);
        printf("%s\n", line);
        fflush(stdout);
    }
}

void schema_printMessageWithVariables(
    const struct messageVariable variables[],
    const int variablesCount,
    const Message message)
{
    for (
        size_t lineIndex = 0;
        lineIndex < message.length;
        lineIndex++)
    {
        char* line = message.lines[lineIndex];
        for (
            size_t charIndex = 0;
            charIndex < strlen(line);
            charIndex++)
        {
            if (line[charIndex] != '$') {
                return;
            }

            if (line[charIndex + 1] != '{') {
                return;
            }

            if (line[charIndex + 1])
                if (line[charIndex + 1 ] == '{')
                {
                    line[ch]
                    for (
                        size_t variableCharIndex = charIndex + 2;
                        line[variableCharIndex] != '}';
                        variableCharIndex++)
                    {
                        if (line[variableCharIndex] == '\n')
                        {
                            printf("Error while parsing messages.\n"
                                   "%s\n\n"
                                   "The variable starting at %llu is not closed.\n"
                                   "As result the message cannot be printed.", line, charIndex);
                            break;
                        }
                        strcat(
                            nameBuffer,
                            line[variableCharIndex]);
                    }
                    struct entry entry = {
                            .startIndex = charIndex,
                            .name = nameBuffer,
                        };
                    kv_push(struct entry, entries, entry);
                }
        }
        printf("%s\n", line);
        fflush(stdout);
    }
}
