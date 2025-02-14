#include "create.h"


#include <core.h>
#include <stdio.h>
#include <string.h>

#include "../schema/messages.h"
#include "../schema/paths.h"




static bool createDirectory(
    const char* path, const char* name) {
    char fullPath[
        strlen(path) + strlen(name) + 2]; // 2 for null terminator

    strcpy(fullPath, path);
    strcat(fullPath, name);
    printf("%s", fullPath);
    mkdir(fullPath);
    return true;
}


static bool copyDirectoryContent(
    const char* directory, const char* destination) {

    return true;
}

static bool checkProjectNameSyntax(const char* source) {

    const char allowed[] =
        "_etaoinshrdlcumwfgypbvkjxqz0123456789";

    for (int i = 0; i < strlen(source); i++) {
        bool found = false;
        for (int s = 0; s < strlen(allowed); s++) {
            if (allowed[s] == source[i]) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
    }
    return true;
}

void create(
    const char* appRootPath,
    const Paths paths,
    const Messages messages,
    char* arguments[],
    const int argumentsLength
    ) {
    const char* appTemplatePath =
        schema_joinPaths(
            appRootPath, paths.templates, "/app/");

    char workingDirectory[256];

    getcwd(workingDirectory, sizeof(workingDirectory));

    bool result =
        copyDirectoryContent(appTemplatePath, workingDirectory);

    if (argumentsLength < 1) {
        schema_printMessage(messages.create_no_project_name);
        return;
    }

    bool syntaxRight = checkProjectNameSyntax(arguments[0]);

    if (!syntaxRight) {

        const MessagePlaceholder variables[] = {
            "name", arguments[0]
        };

        schema_printMessageWithVariables(
            variables, 1, messages.create_invalid_project_name);
        return;
    }

    char buffer[256];
    sprintf(buffer, "/%s", arguments[0]);
    createDirectory(workingDirectory, buffer);

}
