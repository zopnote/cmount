#include <parse.h>
#include <yaml.h>


typedef struct iteration {
    ParseEntry* entries;
    size_t size;
    yaml_token_type_t end;
} Iteration;



static void scalar(
    ParseEntry* const entries,
    const size_t entriesSize,
    const char* key,
    const char* value
) {
    for (size_t i = 0; i < entriesSize; i++) {
        if (!strcmp(entries[i].key, key)) {
            if (entries[i].type == integer) {
                const long parsedValue = strtol(value, NULL, 10);
                if (parsedValue >= INT_MIN || INT_MAX >= parsedValue) {
                    int* finalValue = malloc(sizeof(int));
                entries[i].bufferSize = sizeof(int);
                    *finalValue = parsedValue;
                    entries[i].buffer = finalValue;
                }
                break;
            }
            if (entries[i].type == string) {
                entries[i].bufferSize = sizeof(char) * strlen(key);
                entries[i].buffer =
                    strdup(key);
                break;
            }
            if (entries[i].type == floating) {
                double* const valuePtr = malloc(sizeof(double));
                *valuePtr = strtod(key, NULL);
                entries[i].buffer = valuePtr;
                entries[i].bufferSize = sizeof(double);
                break;
            }
            fprintf(
                stderr,
                "scalar-event that was found "
                "for key %s is not the wanted type %d.",
                entries[i].key,
                entries[i].type
            );
        }
    }
}
#define RESET       "\033[0m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define BOLD        "\033[1m"
#define UNDERLINE   "\033[4m"

const char* get_yaml_token_name(int token_type) {
    static const char* token_names[] = {
        "YAML_NO_TOKEN",
        "YAML_STREAM_START_TOKEN",
        "YAML_STREAM_END_TOKEN",
        "YAML_VERSION_DIRECTIVE_TOKEN",
        "YAML_TAG_DIRECTIVE_TOKEN",
        "YAML_DOCUMENT_START_TOKEN",
        "YAML_DOCUMENT_END_TOKEN",
        "YAML_BLOCK_SEQUENCE_START_TOKEN",
        "YAML_BLOCK_MAPPING_START_TOKEN",
        "YAML_BLOCK_END_TOKEN",
        "YAML_FLOW_SEQUENCE_START_TOKEN",
        "YAML_FLOW_SEQUENCE_END_TOKEN",
        "YAML_FLOW_MAPPING_START_TOKEN",
        "YAML_FLOW_MAPPING_END_TOKEN",
        "YAML_BLOCK_ENTRY_TOKEN",
        "YAML_FLOW_ENTRY_TOKEN",
        "YAML_KEY_TOKEN",
        "YAML_VALUE_TOKEN",
        "YAML_ALIAS_TOKEN",
        "YAML_ANCHOR_TOKEN",
        "YAML_TAG_TOKEN",
        "YAML_SCALAR_TOKEN"
    };

    if (token_type >= 0 && token_type <= YAML_SCALAR_TOKEN) {
        return token_names[token_type];
    }
    return "UNKNOWN_TOKEN";
}


static void scanRecursive(
    yaml_parser_t* parser, Iteration iteration);

static void mapping(
    const Iteration iteration,
    const char* lastKey,
    yaml_parser_t* parser
) {
    size_t i = 0;
    for (i = 0; i < iteration.size; i++) {
        if (!strcmp(lastKey, iteration.entries[i].key)) {
            break;
        }
    }

    if (iteration.entries[i].type != entries) {
        return;
    }
    scanRecursive(parser, (Iteration) {
        .entries = iteration.entries[i].buffer,
        .size = iteration.entries[i].bufferSize,
        .end = YAML_BLOCK_END_TOKEN
    });
}

static void scanRecursive(
    yaml_parser_t* parser,
    const Iteration iteration
) {
    yaml_token_t token, next;
    int index = 0;
    char* lastKey = NULL;
    while (token.type != iteration.end) {
        yaml_parser_scan(parser, &token);
        index++;

        if (token.type == YAML_BLOCK_ENTRY_TOKEN) {
            yaml_parser_scan(parser, &next);
            if (!next.data.scalar.value) {
                yaml_token_delete(&token);
                yaml_token_delete(&next);
                continue;
            }

            for (size_t i = 0; i < iteration.size; i++) {
                if (!strcmp(lastKey, iteration.entries[i].key)) {

                    if (iteration.entries[i].type != list) {
                        yaml_token_delete(&token);
                        yaml_token_delete(&next);
                        continue;
                    }

                    if (iteration.entries[i].buffer) {

                        iteration.entries[i].bufferSize =
                            iteration.entries[i].bufferSize + 1;

                        char** entries = realloc(
                            iteration.entries[i].buffer,
                            sizeof(char*) *
                            iteration.entries[i].bufferSize
                        );

                        entries[iteration.entries[i].bufferSize - 1] =
                            strdup(next.data.scalar.value);

                        iteration.entries[i].buffer =
                            entries;

                        yaml_token_delete(&token);
                        yaml_token_delete(&next);
                        continue;
                    }

                    char** entries = malloc(
                        sizeof(char*)
                    );

                    entries[0] = strdup(
                        next.data.scalar.value);

                    iteration.entries[i].bufferSize = 1;

                    iteration.entries[i].buffer = entries;
                }
            }
        }

        if (token.type == YAML_KEY_TOKEN) {
            yaml_parser_scan(parser, &next);
            if (lastKey) free(lastKey);
            lastKey = strdup(next.data.scalar.value);
            yaml_token_delete(&token);
            yaml_token_delete(&next);
            continue;
        }

        if (token.type == YAML_BLOCK_MAPPING_START_TOKEN) {
            mapping(iteration, lastKey, parser);
            yaml_token_delete(&token);
            yaml_token_delete(&next);
            continue;
        }

        if (token.type == YAML_VALUE_TOKEN) {
            yaml_parser_scan(parser, &next);
            if (!next.data.scalar.value) {
                mapping(iteration, lastKey, parser);
                yaml_token_delete(&token);
                yaml_token_delete(&next);
                continue;
            }

            scalar(
                iteration.entries,
                iteration.size,
                lastKey,
                next.data.scalar.value
            );

            yaml_token_delete(&token);
            yaml_token_delete(&next);
            continue;
        }
    }
    free(lastKey);
    if (&token) yaml_token_delete(&token);
    if (&next) yaml_token_delete(&next);
}


static void resolveYamlString(
    const unsigned char* string,
    ParseEntry entriesBuffer[],
    const size_t entriesBufferSize
) {
    yaml_parser_t parser;

    if (!yaml_parser_initialize(&parser)) {
        fprintf(stderr, "Failed to initialize parser\n");
    }

    yaml_parser_set_input_string(
        &parser, string, sizeof(char) * strlen(string));

    yaml_token_t event;
    while (event.type != YAML_BLOCK_MAPPING_START_TOKEN) {
        yaml_parser_scan(&parser, &event);
    }

    scanRecursive(&parser, (Iteration){
        .entries = entriesBuffer,
        .size = entriesBufferSize,
        .end = YAML_STREAM_END_TOKEN
    });

    yaml_parser_delete(&parser);
}

static void resolveJsonString(
    const char* string,
    ParseEntry entriesBuffer[],
    const size_t entriesBufferSize
) {

}

void parse_resolveString(
    const char* string,
    const ParseFormat format,
    const size_t entriesBufferSize,
    ParseEntry entriesBuffer[]
) {
    switch (format) {
        case yaml: resolveYamlString(
            string,
            entriesBuffer,
            entriesBufferSize); break;
        case json: resolveJsonString(
            string,
            entriesBuffer,
            entriesBufferSize); break;
        default: break;
    }
}
