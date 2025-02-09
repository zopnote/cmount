#include <parse.h>
#include <yaml.h>


typedef struct iteration {
    struct iteration* parent;
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
                    *finalValue = parsedValue;
                    entries[i].buffer = finalValue;
                }
                break;
            }
            if (entries[i].type == string) {
                entries[i].buffer =
                    strdup(key);
                break;
            }
            if (entries[i].type == floating) {
                double* const valuePtr = malloc(sizeof(double));
                *valuePtr = strtod(key, NULL);
                entries[i].buffer = valuePtr;
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

static void scanRecursive(
    yaml_parser_t* parser,
    Iteration iteration
) {
    yaml_token_t token, value;
    while (yaml_parser_scan(&parser, &token)) {
        if (token.type == iteration.end) {
            goto bclean;
        }


        if(!yaml_parser_scan(&parser, &value)) {
            goto bclean;
        }

        if (value.type == iteration.end) {
            goto bclean;
        }

        printf("gre");
        if (token.type == YAML_KEY_TOKEN) {
            printf("asd");
            scalar(
                iteration.entries,
                iteration.size,
                token.data.scalar.value,
                value.data.scalar.value
            );
            goto cclean;
        }

        if (value.type == YAML_BLOCK_MAPPING_START_TOKEN) {
            for (size_t i = 0; i < iteration.size; i++) {
                if (!strcmp(
                    iteration.entries[i].key,
                    token.data.scalar.value
                )) {
                    scanRecursive(&parser, (Iteration){
                        .parent = &iteration,
                        .entries = iteration.entries[i].buffer,
                        .size = iteration.entries[i].bufferSize,
                        .end = YAML_BLOCK_END_TOKEN
                    });
                }
            }
        }

        if (value.type == YAML_BLOCK_SEQUENCE_START_TOKEN) {

        }

        cclean:
            yaml_event_delete(&token);
            yaml_event_delete(&value);
            continue;
        bclean:
            yaml_event_delete(&token);
            yaml_event_delete(&value);
            break;
    }
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

    yaml_event_t event;
    while (event.type != YAML_BLOCK_MAPPING_START_TOKEN) {
        yaml_parser_scan(&parser, &event);
        printf("%d\n", event.type);
    }

    scanRecursive(&parser, (Iteration){
        .parent = NULL,
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
