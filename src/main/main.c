
#include <parse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "schema/messages.h"
#include "schema/paths.h"
#include "tasks/create.h"

static void test(
    const char* appRootPath,
    const Paths paths,
    const Messages messages,
    char* arguments[],
    int argumentsLength
    ) {
    char* value = malloc(sizeof(char) * 6);

    ParseEntry entries[] = {
        "test", string, value
    };

    parse_resolveString(
        "test: hello",
        yaml,
        entries,
        sizeof(entries)
    );
    printf("test: %s", value);
    fflush(stdout);
    free(value);
}





typedef void (TaskFunction)(
    const char* appRootPath,
    Paths paths,
    const Messages messages,
    char* arguments[],
    int argumentsLength
);


struct {
    char* name;
    TaskFunction* taskFunction;
} typedef TaskEntry;


const TaskEntry tasks[] = {
    "create", create,
    "test", test
};

int compare(const void* a, const void* b) {
    return strcmp(((TaskEntry*)a)->name, ((TaskEntry*)b)->name);
}

int main(const int argc, char** argv) {

    const char* appRootPath = schema_resolveAppRootPath(argv[0]);

    const Paths paths = schema_getPaths(
        schema_joinPaths(
                appRootPath, INTERNAL_CONFIGURATION_PATH, ""));

    const char* messagesPath = schema_joinPaths(
            appRootPath, paths.internal, "/messages.yaml");


    const Messages messages =
        schema_getMessages(messagesPath);

    if (!argv[1]) {
        schema_printMessage(messages.help);
        return EXIT_SUCCESS;
    }

    const TaskEntry taskKey = {argv[1]};

    const TaskEntry* foundTask = bsearch(
            &taskKey, &tasks,
            sizeof(tasks) / sizeof(tasks[0]),
            sizeof(tasks[0]), &compare);

    if (!foundTask) {
        const MessagePlaceholder variables[] = {
            "prefix", "",
            "argument", argv[1],
            "suffix", ""
        };

        schema_printMessageWithVariables(
            variables, 3, messages.not_found);
        return EXIT_FAILURE;
    }

    char** arguments = malloc(argc * sizeof(char*));
    if (argc >= 3) {
        free(arguments);
        arguments = argv + 2;
    }

    foundTask->taskFunction(
        appRootPath, paths, messages, arguments, argc - 2);

    if (arguments != argv + 2) free(arguments);
    return EXIT_SUCCESS;
}