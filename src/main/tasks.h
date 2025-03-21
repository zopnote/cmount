#pragma once
#include <core.h>
#include <string.h>

bool get(char** args, size_t args_len, logger_t* logger);
bool config(char** args, size_t args_len, logger_t* logger);
bool install(char** args, size_t args_len, logger_t* logger);
bool create(char** args, size_t args_len, logger_t* logger);
bool build(char** args, size_t args_len, logger_t* logger);
bool doctor(char** args, size_t args_len, logger_t* logger);
bool run(char** args, size_t args_len, logger_t* logger);
bool upgrade(char** args, size_t args_len, logger_t* logger);
bool version(char** args, size_t args_len, logger_t* logger);

typedef bool (*task_callback)(
    char** args, size_t args_len, logger_t* logger
);

static struct {
    const char* name;
    const task_callback task;
} const tasks[]  = {
    "get", get,
    "config", config,
    "install", install,
    "create", create,
    "build", build,
    "doctor", doctor,
    "run", run,
    "upgrade", upgrade,
    "version", version
};

static const int task_count =
    sizeof(tasks) / sizeof(tasks[0]);

inline bool task_exists(const char* name) {
    for (int i = 0; i < task_count; i++)
        if (!strcmp(tasks[i].name, name)) return true;
    return false;
}

inline bool task(
    const char* name, char** args,
    const size_t args_len, logger_t* logger
) {
    for (int i = 0; i < task_count; i++)
        if (!strcmp(tasks[i].name, name))
            return tasks[i].task(args, args_len, logger);
    return false;
}