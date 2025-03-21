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

static parse_entry_t* get_entry(
    const char* key,
    parse_entry_t* const entries,
    const size_t length
) {
    for (size_t i = 0; i < length; i++) {
        if (!strcmp(key, entries[i].key)) {
            return &entries[i];
        }
    }
    return NULL;
}

/**
 * @brief Scans a yaml document stored in the parser and inserts the awaited data in the entries array given by the state structure.
 */
static bool scan_recursive(
    yaml_parser_t* parser,
    const parse_state_t state,
    logger_t* logger
);


/**
 * @brief Parses the scalar value string into the corresponding type and set the buffer of the entry by the key in the entries array.
 */
static bool scalar(
    parse_entry_t* const entry,
    const char* value,
    logger_t* logger
) {
    const char* fmt = "Found scalar<%s> %s: %s";
    
    if (entry->type == integer) {
        const long parsed_value = strtol(value, NULL, 10);

        if (parsed_value >= INT_MIN || INT_MAX >= parsed_value) {
            int* finalValue = malloc(sizeof(int));

            *finalValue = parsed_value;
            entry->buffer = finalValue;

            entry->size = sizeof(int);
        }
        const char* fmt_name = "integer";
        char* msg = malloc(
            strlen(fmt) + strlen(fmt_name) +
            strlen(entry->key) + strlen(value) + 1
        );
        sprintf(msg, fmt, fmt_name, entry->key, value);
        logger->log(logger, info, msg, 0);
        free(msg);
        return true;
    }
    
    if (entry->type == string) {
        entry->size = sizeof(char) * strlen(value);
        entry->buffer = strdup(value);
        return logger->log(
            logger, info,
            "Found (scalar)<string> %s: %s",
            2, entry->key, value
        );
    }

    if (entry->type == floating) {
        double* const value_ptr = malloc(sizeof(double));
        entry->size = sizeof(double);

        *value_ptr = strtod(entry->key, NULL);
        entry->buffer = value_ptr;
        return logger->log(
            logger, info,
            "Found (scalar)<floating> %s: %s",
            2, entry->key, value
        );
    }
    return logger->log(
        logger, error,
        "Parser decided for scalar value "
        "but the type<%d> isn't a scalar type. At %s:%s",
        entry->type, entry->key, value
    );
}


/**
 * @brief Continues the scan by the parser until the map ends and insert the awaited data in the entries array of the state structure.
 */
static bool further_entries(
    const parse_entry_t* entry,
    yaml_parser_t* parser,
    logger_t* logger
) {
    // The following will return true because there are
    // at first no difference between a yaml block list and a block map.
    // Therefore, the parser will always call the entries scan for block list.
    if (entry->type != map) return true;
    logger->log(logger, status,
        "Start scan for %s...",
        1, entry->key
    );
    const bool result = scan_recursive(parser,
        (parse_state_t) {
            .entries = entry->buffer,
            .size = entry->size,
            .end = YAML_BLOCK_END_TOKEN
        }, logger);

    if (!result) return logger->log(logger, error,
        "Recursive scan for key %s has failed.",
        1, entry->key
    );

    return logger->log(logger, status,
        "Recursive scan for key %s is done.",
        1, entry->key
    );
}


/**
 * @brief Either, if the list already exists adds the value to it, or if not, a new array will be created with the value.
 */
static bool follow_list(
    parse_entry_t* entry,
    const char* value,
    logger_t* logger
) {
    if (entry->type != list) return logger->log(
        logger, error,
        "Wrong type for %s was found. Type list was awaited.",
        1, entry->key
    );

    if (entry->buffer) {
        entry->size++;
        char** entries = realloc(
            entry->buffer,
            sizeof(char*) * entry->size + 1
        );

        entries[entry->size - 1] = strdup(value);
        entry->buffer = entries;
        return logger->log(
            logger, info,
            "Added \"%s\" to %s",
            2, value, entry->key
        );
    }

    char** entries = malloc(sizeof(char*));
    entry->buffer = entries;
    logger->log(
        logger, info,
        "Allocated list: %s",
        1, entry->key
    );
    follow_list(entry, value, logger);
    return true;
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


static bool scan_recursive(
    yaml_parser_t* parser,
    const parse_state_t state,
    logger_t* logger
) {
    logger->log(logger, info,
        "Scan for %d entries at level %d...",
        state.size, parser->flow_level
    );
    yaml_token_t token, next;
    char* key = NULL;
    while (token.type != state.end && next.type != state.end) {
        if (
            token.type == YAML_ALIAS_TOKEN ||
            token.type == YAML_TAG_TOKEN ||
            token.type == YAML_ANCHOR_TOKEN
        ) logger->log(logger, error,
            "Aliases, tags and anchors are not supported.",
        0);

        if (&token) yaml_token_delete(&token);
        if (&next) yaml_token_delete(&next);
        yaml_parser_scan(parser, &token);

        if (token.type == YAML_KEY_TOKEN) {
            yaml_parser_scan(parser, &next);
            if (key) free(key);
            key = strdup((char*)next.data.scalar.value);
            bool found = false;
            for (size_t i = 0; i < state.size; i++) {
                if (!strcmp(key, state.entries[i].key)) {
                    found = true;
                }
            }
            if (!found) logger->log(
               logger, info,
               "Unknown entry: %s", 1, key
            );
            continue;
        }

        parse_entry_t* entry = get_entry(
            key, state.entries, state.size
        );
        if (!entry) continue;

        if (token.type == YAML_BLOCK_MAPPING_START_TOKEN) {
            further_entries(entry, parser, logger);
        }

        else if (token.type == YAML_VALUE_TOKEN) {
            yaml_parser_scan(parser, &next);
            if (!next.data.scalar.value) further_entries(entry, parser, logger);

            else scalar(entry, (char*)next.data.scalar.value, logger);
        }

        else if (token.type == YAML_BLOCK_ENTRY_TOKEN) {
            yaml_parser_scan(parser, &next);

            if (!next.data.scalar.value) continue;
            const bool result = follow_list(
                entry, (char*)next.data.scalar.value, logger
            );

            if (!result) logger->log(
                logger, error,
                "Can't put the value in list context of %s: %s",
                2, entry->key, (char*)next.data.scalar.value
            );
        }
    }

    if (key) free(key);
    return logger->log(logger, info,
        "Scan of level %d is done.",
        parser->flow_level
    );
}


bool parse_resolve_yaml_string(
    const char* string,
    parse_entry_t* entries,
    size_t entries_length,
    logger_t* logger
) {

    yaml_parser_t parser;
    if (!yaml_parser_initialize(&parser)) return logger->log(
        logger, error,
        "Yaml parser cannot be initialized."
    );

    char* input = strdup(string);
    yaml_parser_set_input_string(
        &parser, (unsigned char*)input, sizeof(char) * strlen(string)
    );

    yaml_token_t event;
    while (event.type != YAML_BLOCK_MAPPING_START_TOKEN) {
        yaml_parser_scan(&parser, &event);

        if (event.type == YAML_NO_TOKEN) {
            free(input);
            return logger->log(logger, error,
                "The following string cannot be resolved by"
                "the parser: %s", string
            );
        }
    }

    const bool result = scan_recursive(&parser,
        (parse_state_t) {
            .entries = entries,
            .size = entries_length,
            .end = YAML_STREAM_END_TOKEN
        }, logger);

    free(input);
    if (!result) return logger->log(
        logger, error,
        "Parsing process done."
    );

    yaml_parser_delete(&parser);
    return logger->log(
        logger, info,
        "Parsing process done."
    );
}






