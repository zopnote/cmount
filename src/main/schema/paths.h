#pragma once
#include <parse.h>

static parse_entry_t paths_entries[] = {
    "binary", string, NULL, 0,
    "internal", string, NULL, 0,
    "cache", string, NULL, 0,
    "library", string, NULL, 0,
    "include", string, NULL, 0,
    "runtime", string, NULL, 0,
    "config", string, NULL, 0,
    "scripts", string, NULL, 0,
    "templates", string, NULL, 0
};

static const size_t paths_entries_count =
    sizeof(paths_entries) / sizeof(paths_entries[0]);

inline bool paths_load(
    const char* buffer, logger_t* logger
) {
    return parse_resolve(
        buffer, paths_entries, paths_entries_count, logger
    );
}

inline char* paths_get(const char* name) {
    for (size_t i = 0; i < paths_entries_count; i++)
        if (strcmp(name, paths_entries[i].key))
            if (paths_entries[i].buffer)
                return paths_entries[i].buffer;
    return NULL;
}