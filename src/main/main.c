
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "schema/messages.h"
#include "schema/paths.h"
#include "tasks/create.h"

typedef void (TaskFunction)(
    const char* appRootPath,
    Paths paths,
    char* arguments[]);

struct {
    char* name;
    TaskFunction* taskFunction;
} typedef TaskEntry;


const TaskEntry tasks[] = {
    "create", create,
};

int compare(const void* a, const void* b) {
    return strcmp(((TaskEntry*)a)->name, ((TaskEntry*)b)->name);
}

int main(int argc, char** argv) {

    const char* appRootPath =
        schema_resolveAppRootPath(argv[0]);

    const Paths paths =
        schema_getPaths(
            schema_joinPaths(
                appRootPath, INTERNAL_CONFIGURATION_PATH, ""));

    const char* messagesPath =
        schema_joinPaths(
            appRootPath, paths.internal, "/messages.yaml");


    const Messages messages =
        schema_getMessages(messagesPath);

    if (!argv[1])
    {
        schema_printMessage(messages.help);
        return EXIT_SUCCESS;
    }

    const TaskEntry taskKey = {
        argv[1]
    };

    const TaskEntry* foundTask =
        bsearch(
            &taskKey, &tasks,
            sizeof(tasks) / sizeof(tasks[0]),
            sizeof(tasks[0]), &compare);

    if (!foundTask) {
        const MessagePlaceholder variables[] = {
            "prefix", "cmount ",
            "argument", argv[1],
            "suffix", ""
        };

        schema_printMessageWithVariables(
            variables, 3, messages.not_found);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}