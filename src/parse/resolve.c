#include <limits.h>
#include <parse.h>
#include <yaml.h>


/**
 * @brief Representation of the state superordinate of the iterations through an entries array.
 *
 * The state consists of the entries array pointer,
 * the length of the array as size and the last token
 * the yaml parser should iterate to in the case of scanning.
 */
typedef struct {
    parse_entry_t* const entries;
    const size_t size;
    const yaml_token_type_t end;
} parse_state_t;


/**
 * @brief Scans a yaml document stored in the parser and inserts the awaited data in the entries array given by the state structure.
 */
static void scan_recursive(
    yaml_parser_t* parser,
    parse_state_t state
);


/**
 * @brief Parses the scalar value string into the corresponding type and set the buffer of the entry by the key in the entries array.
 */
static void scalar(
    parse_entry_t* const entries,
    const size_t length,
    const char* key,
    const char* value
) {
    for (size_t i = 0; i < length; i++) {
        if (!strcmp(entries[i].key, key)) {

            if (entries[i].type == integer) {
                const long parsed_value =
                    strtol(value, NULL, 10);

                if (parsed_value >= INT_MIN || INT_MAX >= parsed_value) {
                    int* finalValue = malloc(sizeof(int));

                    *finalValue = parsed_value;
                    entries[i].buffer = finalValue;

                    entries[i].size = sizeof(int);
                }

                break;
            }

            if (entries[i].type == string) {
                entries[i].size =
                    sizeof(char) * strlen(value);
                entries[i].buffer = strdup(value);
                break;
            }

            if (entries[i].type == floating) {

                double* const value_ptr =
                    malloc(sizeof(double));

                entries[i].size = sizeof(double);

                *value_ptr = strtod(key, NULL);
                entries[i].buffer = value_ptr;
                break;
            }
        }
    }
}


/**
 * @brief Continues the scan by the parser until the map ends and insert the awaited data in the entries array of the state structure.
 */
static void parse_further_entries_in_map(
    const parse_state_t* state,
    const char* key,
    yaml_parser_t* parser
) {

    size_t i = 0;
    for (i = 0; i < state->size; i++) {
        if (!strcmp(key, state->entries[i].key)) {
            break;
        }
    }

    if (state->entries[i].type != map) {
        return;
    }

    scan_recursive(parser, (parse_state_t) {
        .entries = state->entries[i].buffer,
        .size = state->entries[i].size,
        .end = YAML_BLOCK_END_TOKEN
    });
}


/**
 * @brief Either, if the list already exists adds the value to it, or if not, a new array will be created with the value.
 */
static void add_to_or_create_list(
    const parse_state_t* state,
    const char* key,
    const char* value
) {

    for (size_t i = 0; i < state->size; i++) {
        if (!strcmp(key, state->entries[i].key)) {

            if (state->entries[i].type != list) {
                return;
            }

            if (state->entries[i].buffer) {

                state->entries[i].size =
                    state->entries[i].size + 1;

                char** entries = realloc(
                    state->entries[i].buffer,
                    sizeof(char*) * state->entries[i].size
                );

                entries[state->entries[i].size - 1] =
                    strdup(value);

                state->entries[i].buffer = entries;

                return;
            }

            char** entries = malloc(
                sizeof(char*)
            );

            entries[0] = strdup(value);

            state->entries[i].size = 1;

            state->entries[i].buffer = entries;
        }
    }
}


static const char* get_yaml_token_name(
    const int token_identifier
) {
    static const char* restrict token_names[] = {
        "NO_TOKEN",
        "STREAM_START_TOKEN",
        "STREAM_END_TOKEN",
        "VERSION_DIRECTIVE_TOKEN",
        "TAG_DIRECTIVE_TOKEN",
        "DOCUMENT_START_TOKEN",
        "DOCUMENT_END_TOKEN",
        "BLOCK_SEQUENCE_START_TOKEN",
        "BLOCK_MAPPING_START_TOKEN",
        "BLOCK_END_TOKEN",
        "FLOW_SEQUENCE_START_TOKEN",
        "FLOW_SEQUENCE_END_TOKEN",
        "FLOW_MAPPING_START_TOKEN",
        "FLOW_MAPPING_END_TOKEN",
        "BLOCK_ENTRY_TOKEN",
        "FLOW_ENTRY_TOKEN",
        "KEY_TOKEN",
        "VALUE_TOKEN",
        "ALIAS_TOKEN",
        "ANCHOR_TOKEN",
        "TAG_TOKEN",
        "SCALAR_TOKEN"
    };
    if (token_identifier >= 0 && token_identifier <= YAML_SCALAR_TOKEN) {
        return token_names[token_identifier];
    }
    return "UNKNOWN_TOKEN";
}


static void scan_recursive(
    yaml_parser_t* parser,
    const parse_state_t state
) {

    yaml_token_t token, next;
    char* lastKey = NULL;

    while (token.type != state.end && next.type != state.end) {
        if (&token) yaml_token_delete(&token);
        if (&next) yaml_token_delete(&next);
        yaml_parser_scan(parser, &token);

        if (token.type == YAML_KEY_TOKEN) {
            yaml_parser_scan(parser, &next);
            if (lastKey) free(lastKey);
            lastKey = strdup((char*)next.data.scalar.value);
            continue;
        }

        if (token.type == YAML_BLOCK_MAPPING_START_TOKEN) {
            parse_further_entries_in_map(
                &state, lastKey, parser);
            continue;
        }

        if (token.type == YAML_VALUE_TOKEN) {
            yaml_parser_scan(parser, &next);

            if (!next.data.scalar.value) {
                parse_further_entries_in_map(
                    &state, lastKey, parser);
                continue;
            }

            scalar(
                state.entries,
                state.size,
                lastKey,
                (char*)next.data.scalar.value
            );
            continue;
        }

        if (token.type == YAML_BLOCK_ENTRY_TOKEN) {
            yaml_parser_scan(parser, &next);

            if (!next.data.scalar.value) {
                continue;
            }

            add_to_or_create_list(&state, lastKey, (char*)next.data.scalar.value);
        }

    }

    if (lastKey) free(lastKey);
}


void parse_resolve_yaml_string(
    const char* string,
    parse_entry_t* entries,
    size_t entries_length,
    const logger_t* logger
) {

    yaml_parser_t parser;
    if (!yaml_parser_initialize(&parser)) {
        return;
    }

    yaml_parser_set_input_string(
        &parser, (unsigned char*)string, sizeof(char) * strlen(string));

    yaml_token_t event;
    while (event.type != YAML_BLOCK_MAPPING_START_TOKEN) {
        yaml_parser_scan(&parser, &event);

        if (event.type == YAML_NO_TOKEN) {
            return;
        }
    }

    scan_recursive(&parser, (parse_state_t) {
        .entries = entries,
        .size = entries_length,
        .end = YAML_STREAM_END_TOKEN
    });

    yaml_parser_delete(&parser);
}






