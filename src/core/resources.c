#include "klib/kstring.h"
#include "yaml.h"
#include <stdio.h>

#define INTERNAL_PATH_CONFIGURATION_FILE_PATH "internal/paths.yaml"

typedef void* (*YamlParsingOperation)(yaml_event_t, char*);

static void* parseYaml(const kstring_t filePath, YamlParsingOperation yamlParsingOperation) {

    FILE *file = fopen(filePath.s, "r");
    if (!file) {
        perror("Error opening file");
        return nullptr;
    }

    yaml_parser_t parser;
    yaml_event_t event;

    if (!yaml_parser_initialize(&parser)) {
        fprintf(stderr, "Failed to initialize parser!\n");
        fclose(file);
        return nullptr;
    }

    yaml_parser_set_input_file(&parser, file);

    char buffer[256];
    void* out;

    while (1) {

        if (!yaml_parser_parse(&parser, &event)) {
            fprintf(stderr, "Parser error %d\n", parser.error);
            break;
        }

        if (event.type == YAML_STREAM_END_EVENT) {
            yaml_event_delete(&event);
            break;
        }

        out = yamlParsingOperation(event, buffer);
        if(out) break;

        yaml_event_delete(&event);

    }

    yaml_parser_delete(&parser);
    fclose(file);
    return out;
}