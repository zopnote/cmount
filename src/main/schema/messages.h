#pragma once
#include <parse.h>
struct {
    const char* key;
    const char* value;
} typedef placeholder_t;

struct {
    char** lines;
    const size_t lines_count;
} typedef message_t;

static parse_entry_t messages_entries[] = {
    "help", list, NULL, 0
};

static const size_t messages_entries_count =
    sizeof(messages_entries) / sizeof(messages_entries[0]);

inline bool messages_load(
    const char* buffer, logger_t* logger
) {
    return parse_resolve(
        buffer, messages_entries, messages_entries_count, logger
    );
}

inline message_t messages_get(const char* key) {
    for (size_t i = 0; i < messages_entries_count; i++) {
        if (strcmp(key, messages_entries[i].key)) continue;
        if (messages_entries[i].buffer) return (message_t) {
            .lines = messages_entries[i].buffer,
            .lines_count = messages_entries[i].size
        };
    }
    return (message_t) {
        .lines = NULL,
        .lines_count = 0
    };
}

inline bool messages_log(
    message_t* message, placeholder_t* placeholder,
    const size_t placeholder_count
) {
    char** messages = malloc(
        message->lines_count * sizeof(message->lines[0])
    );
    for (size_t i = 0; i < message->lines_count; i++) {
        for (size_t p = 0; p < placeholder_count; p++) {

        }
    }
    free(messages);

}

