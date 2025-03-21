#pragma once
#include <parse.h>
struct {
    const char* key;
    const char* value;
} typedef placeholder_t;

struct {
    char** lines;
    const size_t lines_count;
    placeholder_t* placeholder;
    size_t placeholder_count;
} typedef message_t;

static parse_entry_t messages[] = {
    "help", list, NULL, 0
};

static const size_t messages_count =
    sizeof(messages) / sizeof(messages[0]);

inline bool messages_load(
    const char* buffer, logger_t* logger
) {
    return parse_resolve(
        buffer, messages, messages_count, logger
    );
}

inline message_t messages_get(const char* key) {
    for (size_t i = 0; i < messages_count; i++)
        if (strcmp(key, messages[i].key))
            if (messages[i].type == list || messages[i].type == string)
                if (messages[i].buffer) return (message_t) {
                    .lines = messages[i].buffer,
                    .lines_count = messages[i].size,
                    .placeholder = NULL,
                    .placeholder_count = 0
                };
    return (message_t) {
        .lines = NULL,
        .lines_count = 0,
        .placeholder = NULL,
        .placeholder_count = 0
    };
}

inline bool messages_add_placeholder(
    message_t* message, placeholder_t* placeholder
) {
    if (message->placeholder) {
        message->placeholder_count++;
        message->placeholder = realloc(
            message->placeholder,
            message->placeholder_count *
            sizeof(placeholder_t)
        );

    }
}

inline bool messages_log(
    message_t* message, logger_t* logger
) {
    logger_write_sequence()
}


inline bool messages_free(message_t* message) {
    logger_write_sequence()
}
