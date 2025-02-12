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
    ParseEntry* const entries;
    const size_t size;
    const yaml_token_type_t end;
} ParseState;


/**
 * @brief Scans a yaml document stored in the parser and inserts the awaited data in the entries array given by the state structure.
 */
static void scanRecursive(
    yaml_parser_t* const parser,
    const ParseState state,
    const bool verbose
);


/**
 * @brief Parses the scalar value string into the corresponding type and set the buffer of the entry by the key in the entries array.
 */
static void scalar(
    ParseEntry* const entries,
    const size_t length,
    const char* const key,
    const char* const value
) {
    for (size_t i = 0; i < length; i++) {
        if (!strcmp(entries[i].key, key)) {

            if (entries[i].type == integer) {
                const long parsedValue =
                    strtol(value, NULL, 10);

                if (parsedValue >= INT_MIN || INT_MAX >= parsedValue) {
                    int* finalValue = malloc(sizeof(int));

                    *finalValue = parsedValue;
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

                double* const valuePtr =
                    malloc(sizeof(double));
                entries[i].size = sizeof(double);

                *valuePtr = strtod(key, NULL);
                entries[i].buffer = valuePtr;
                break;
            }
        }
    }
}


/**
 * @brief Continues the scan by the parser until the map ends and insert the awaited data in the entries array of the state structure.
 */
static void parseFurtherEntriesInMap(
    const ParseState* const state,
    const char* const key,
    yaml_parser_t* const parser,
    const bool verbose
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

    scanRecursive(parser, (ParseState) {
        .entries = state->entries[i].buffer,
        .size = state->entries[i].size,
        .end = YAML_BLOCK_END_TOKEN
    }, verbose);
}


/**
 * @brief Either, if the list already exists adds the value to it, or if not a new array will be created with the value.
 */
static void addToList(
    const ParseState* const state,
    const char* const key,
    const char* const value
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

static const char* getYamlTokenName(const int tokenType) {
    static const char* const tokenNames[] = {
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
    if (tokenType >= 0 && tokenType <= YAML_SCALAR_TOKEN) {
        return tokenNames[tokenType];
    }
    return "UNKNOWN_TOKEN";
}

static void scanRecursive(
    yaml_parser_t* const parser,
    const ParseState state,
    const bool verbose
) {

    yaml_token_t token, next;
    char* lastKey = NULL;


    if (verbose) {
        fprintf(
            stdout, "\n\nFound entries for the current level: ");

        for (size_t i = 0; i < state.size; i++) {
            fprintf(stdout, "%s; ", state.entries[i].key);
        }

        fprintf(
            stdout, "\n\nStart yaml scan and further insertion...\n");
    }

    while (token.type != state.end && next.type != state.end) {
        if (&token) yaml_token_delete(&token);
        if (&next) yaml_token_delete(&next);
        yaml_parser_scan(parser, &token);

        if (verbose) fprintf(stdout, "%s; ", getYamlTokenName(token.type));

        if (token.type == YAML_KEY_TOKEN) {
            yaml_parser_scan(parser, &next);
            if (lastKey) free(lastKey);
            lastKey = strdup(next.data.scalar.value);
            continue;
        }

        if (token.type == YAML_BLOCK_MAPPING_START_TOKEN) {
            parseFurtherEntriesInMap(
                &state, lastKey, parser, verbose);
            continue;
        }

        if (token.type == YAML_VALUE_TOKEN) {
            yaml_parser_scan(parser, &next);

            if (!next.data.scalar.value) {
                parseFurtherEntriesInMap(
                    &state, lastKey, parser, verbose);
                continue;
            }

            scalar(
                state.entries,
                state.size,
                lastKey,
                next.data.scalar.value
            );
            continue;
        }

        if (token.type == YAML_BLOCK_ENTRY_TOKEN) {
            yaml_parser_scan(parser, &next);

            if (!next.data.scalar.value) {
                continue;
            }

            addToList(&state, lastKey, next.data.scalar.value);
        }

    }

    if (verbose) fprintf(
        stdout,
        "\n\n\nFinished the scan until the token %s.\n",
        getYamlTokenName(state.end)
    );

    if (lastKey) free(lastKey);
}


void parse_resolveYamlString(
    const char* const string,
    ParseEntry* const entries,
    const size_t entriesLength,
    const bool verbose
) {
    if (verbose) fprintf(stdout, "\n");

    yaml_parser_t parser;
    if (!yaml_parser_initialize(&parser)) {
        fprintf(stderr, "\nFailed to initialize parser\n");
        return;
    }

    yaml_parser_set_input_string(
        &parser, string, sizeof(char) * strlen(string));

    yaml_token_t event;
    while (event.type != YAML_BLOCK_MAPPING_START_TOKEN) {
        yaml_parser_scan(&parser, &event);

        if (event.type == YAML_NO_TOKEN) {
            fprintf(
                stderr,
                "\nParser data is invalid: %s\n",
                string
            );
            return;
        }
    }

    if (verbose) fprintf(
        stdout, "\nYaml parser is initialized...\n");

    scanRecursive(&parser, (ParseState) {
        .entries = entries,
        .size = entriesLength,
        .end = YAML_STREAM_END_TOKEN
    }, verbose);

    yaml_parser_delete(&parser);
}






