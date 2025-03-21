#pragma once
#include <parse.h>

static parse_entry_t paths[] = {
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

static const size_t paths_count =
    sizeof(paths) / sizeof(paths[0]);

inline bool paths_load(
    const char* buffer, logger_t* logger
) {
    return parse_resolve(
        buffer, paths, paths_count, logger
    );
}

inline char* paths_get(const char* name) {
    for (size_t i = 0; i < paths_count; i++)
        if (strcmp(name, paths[i].key))
            if (paths[i].buffer)
                return paths[i].buffer;
    return NULL;
}