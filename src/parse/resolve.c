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
    
    
    if (entry->type == integer) {
        const long parsed_value = strtol(value, NULL, 10);

        if (parsed_value >= INT_MIN || INT_MAX >= parsed_value) {
            int* finalValue = malloc(sizeof(int));

            *finalValue = parsed_value;
            entry->buffer = finalValue;

            entry->size = sizeof(int);
        }
        return logger->log(
            logger, info,
            "(scalar)<integer> %s: %s",
            entry->key, value
        );
    }
    
    if (entry->type == string) {
        entry->size = sizeof(char) * strlen(value);
        entry->buffer = strdup(value);
        return logger->log(
            logger, info,
            "(scalar)<string> %s: %s",
            entry->key, value
        );
    }

    if (entry->type == floating) {
        double* const value_ptr = malloc(sizeof(double));
        entry->size = sizeof(double);

        *value_ptr = strtod(entry->key, NULL);
        entry->buffer = value_ptr;
        return logger->log(
            logger, info,
            "(scalar)<floating> %s: %s",
            entry->key, value
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

    if (entry->type != map) return logger->log(logger, error,
        "Parser found a map entry type(%d) but awaited "
        "by the entries map was type(%d).",
        map, entry->type
    );


    const bool result = scan_recursive(parser,
        (parse_state_t) {
            .entries = entry->buffer,
            .size = entry->size,
            .end = YAML_BLOCK_END_TOKEN
        }, logger);

    if (!result) return logger->log(logger, error,
        "Recursive scan for key %s has failed.",
        entry->key
    );

    return logger->log(logger, status,
        "Recursive scan for key %s is done.",
        entry->key
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
        entry->key
    );

    if (entry->buffer) {
        entry->size = entry->size + 1;
        char** entries = realloc(
            entry->buffer,
            sizeof(char*) * entry->size
        );

        entries[entry->size - 1] = strdup(value);
        entry->buffer = entries;

        return logger->log(
            logger, status,
            "Added %s to list %s successfully.",
            value, entry->key
        );
    }

    char** entries = malloc(sizeof(char*));
    entries[0] = strdup(value);

    entry->size = 1;

    entry->buffer = entries;
    return logger->log(
        logger, status,
        "Added %s to new list %s successfully.",
        value, entry->key
    );
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
    yaml_token_t token, next;
    char* key = NULL;

    while (token.type != state.end && next.type != state.end) {
        if (&token) yaml_token_delete(&token);
        if (&next) yaml_token_delete(&next);
        yaml_parser_scan(parser, &token);

        printf("tokentype: %s; value: %s\n", get_yaml_token_name(token.type), token.data.scalar.value);
        if (token.type == YAML_KEY_TOKEN) {
            yaml_parser_scan(parser, &next);
            if (key) free(key);
            key = strdup((char*)next.data.scalar.value);
            continue;
        }

        parse_entry_t* entry = get_entry(key, state.entries, state.size);
        const char* const value = (char*)next.data.scalar.value;
        if (!entry) logger->log(logger, info,
            "Parsed entry contains key(%s) to value(%s) pair "
            "that isn't awaited by the given entries.",
            key, value
        );

        if (token.type == YAML_BLOCK_MAPPING_START_TOKEN) {
            further_entries(entry, parser, logger);
            continue;
        }

        if (token.type == YAML_VALUE_TOKEN) {
            yaml_parser_scan(parser, &next);
            if (!value) further_entries(entry, parser, logger);

            else scalar(entry, value, logger);
            continue;
        }

        if (token.type == YAML_BLOCK_ENTRY_TOKEN) {
            yaml_parser_scan(parser, &next);

            if (!value) continue;
            const bool result = follow_list(
                entry, value, logger
            );

            if (!result) logger->log(
                logger, error,
                "Can't put the value in list context of %s: %s",
                entry->key, value
            );
        }
    }

    if (key) free(key);
    return logger->log(logger, status,
        "Scan for level %d is done.",
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


    yaml_parser_set_input_string(
        &parser, (unsigned char*)string,
        sizeof(char) * strlen(string)
    );

    yaml_token_t event;
    while (event.type != YAML_BLOCK_MAPPING_START_TOKEN) {
        yaml_parser_scan(&parser, &event);

        if (event.type == YAML_NO_TOKEN) return logger->log(
            logger, error,
            "The following string cannot be resolved by"
            "the parser: %s", string
        );
    }

    const bool result = scan_recursive(&parser,
        (parse_state_t) {
            .entries = entries,
            .size = entries_length,
            .end = YAML_STREAM_END_TOKEN
        }, logger);

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






