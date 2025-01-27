#include "paths.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <yaml.h>

static Paths parsePaths(const char* fileBuffer)
{;
    yaml_parser_t parser;
    if (!yaml_parser_initialize(&parser)) {
        fprintf(stderr, "Failed to initialize parser\n");
    }

    Paths paths = {};

    yaml_parser_set_input_string(&parser, fileBuffer, strlen(fileBuffer));
    yaml_token_t currentToken, furtherToken;

    char* lastKey;
    while (yaml_parser_scan(&parser, &currentToken)) {
        if (currentToken.type == YAML_STREAM_END_TOKEN) break;

        if (currentToken.type == YAML_KEY_TOKEN) {
            if (yaml_parser_scan(&parser, &furtherToken)) {
                if (furtherToken.type == YAML_SCALAR_TOKEN) {
                    if (lastKey) free(lastKey);
                    lastKey = strdup(furtherToken.data.scalar.value);
                } else {
                    fprintf(stderr, "Unexpected token after key\n");
                }
                yaml_token_delete(&furtherToken);
            }

        } else if (currentToken.type == YAML_SCALAR_TOKEN) {
            if (!strcmp(lastKey, "binary"))
                paths.binary = strdup(currentToken.data.scalar.value);

            else if (!strcmp(lastKey, "internal"))
                paths.internal = strdup(currentToken.data.scalar.value);

            else if (!strcmp(lastKey, "cache"))
                paths.cache = strdup(currentToken.data.scalar.value);

            else if (!strcmp(lastKey, "library"))
                paths.library = strdup(currentToken.data.scalar.value);

            else if (!strcmp(lastKey, "include"))
                paths.include = strdup(currentToken.data.scalar.value);

            else if (!strcmp(lastKey, "runtime"))
                paths.runtime = strdup(currentToken.data.scalar.value);

            else if (!strcmp(lastKey, "config"))
                paths.config = strdup(currentToken.data.scalar.value);

            else if (!strcmp(lastKey, "scripts"))
                paths.scripts = strdup(currentToken.data.scalar.value);

            else if (!strcmp(lastKey, "templates"))
                paths.templates = strdup(currentToken.data.scalar.value);

        }
        yaml_token_delete(&currentToken);
    }
    if (lastKey) free(lastKey);
    return paths;
}


char* schema_joinPaths(
    const char* appDirectoryPath,
    const char* relativeDirectory,
    const char* filename)
{
    char pathFile[
        strlen(appDirectoryPath)
        + strlen(relativeDirectory)
        + strlen(filename)
        + 1];
    strcpy(pathFile, appDirectoryPath);
    strcat(pathFile, relativeDirectory);
    strcat(pathFile, filename);
    return strdup(pathFile);
}

char* schema_resolveAppRootPath(const char* executablePath)
{
    int slashCutIndex = 0;
    int foundSlash = 0;
    for (size_t i = strlen(executablePath) - 1; i > 0; i--) {
        if (executablePath[i] == '\\' || executablePath[i] == '/') {
            foundSlash++;
            if (foundSlash == 2)
            {
                slashCutIndex = (int)i + 1;
                break;
            }
        }
    }

    char buffer[ strlen(executablePath) + 1 ];

    strncpy(buffer, executablePath, slashCutIndex);
    buffer[slashCutIndex] = '\0';

    return strdup(buffer);
}

Paths schema_getPaths(const char* filePath)
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
    Paths paths = {};
    if (fileBuffer)
    {
        paths = parsePaths(fileBuffer);
    }
    free(fileBuffer);
    return paths;
}
