#include <stdio.h>
#include "yaml.h"

void* read_yaml_file(const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (!file) {
        perror("Error opening yaml file.");
        return nullptr;
    }
    yaml_parser_t parser;
    yaml_event_t event;
    if (!yaml_parser_initialize(&parser)) {
        perror("Error while initializing yaml parser.");
        fclose(file);
        return nullptr;
    }

}
