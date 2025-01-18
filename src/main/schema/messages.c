#include "messages.h"

#include <yaml.h>
#include "klib/kvec.h"

static Messages parseMessages(const char* fileBuffer)
{
    yaml_parser_t parser;
    if (!yaml_parser_initialize(&parser)) {
        fprintf(stderr, "Failed to initialize parser\n");
    }

    Messages messages = {};
    kv_init(messages.help);

    yaml_parser_set_input_string(&parser, fileBuffer, strlen(fileBuffer));
    yaml_token_t currentToken, furtherToken;

    char* lastKey = NULL;

    while (yaml_parser_scan(&parser, &currentToken)) {
        if (currentToken.type == YAML_STREAM_END_TOKEN) break;

        if (currentToken.type == YAML_KEY_TOKEN) {
            if (yaml_parser_scan(&parser, &furtherToken)) {
                if (furtherToken.type == YAML_SCALAR_TOKEN) {
                    if (lastKey) free(lastKey);
                    lastKey = strdup((char*)furtherToken.data.scalar.value);
                } else {
                    fprintf(stderr, "Unexpected token after key\n");
                }
                yaml_token_delete(&furtherToken);
            }

        } else if ((currentToken.type == YAML_FLOW_SEQUENCE_START_TOKEN ||
                    currentToken.type == YAML_BLOCK_SEQUENCE_START_TOKEN)) {
            if (lastKey && !strcmp(lastKey, "help")) {
                while (yaml_parser_scan(&parser, &furtherToken) &&
                       (furtherToken.type != YAML_FLOW_SEQUENCE_END_TOKEN &&
                        furtherToken.type != YAML_BLOCK_END_TOKEN)) {
                    if (furtherToken.type == YAML_SCALAR_TOKEN) {
                        kv_push(char*, messages.help, strdup((char*)furtherToken.data.scalar.value));
                    }
                    yaml_token_delete(&furtherToken);
                }
            }
        }

        yaml_token_delete(&currentToken);
    }

    if (lastKey) free(lastKey);
    yaml_parser_delete(&parser);
    return messages;
}


Messages schema_getMessages(const char* filePath)
{
    char* fileBuffer = NULL;
    FILE* file = fopen(filePath, "r");

    if (!file)
    {
        perror("Failed to open file");
    } else {
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
    if (fileBuffer)
    {
        messages = parseMessages(fileBuffer);
    }
    free(fileBuffer);
    return messages;
}

void schema_printMessage(const kvec_str message) {
    for (size_t i = 0; i < message.n; i++)
    {
        const char* line = kv_A(message, i);
        printf("%s\n", line);
        fflush(stdout);
    }
}