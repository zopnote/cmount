
#include "schema/messages.h"
#include "schema/paths.h"


int main(int argc, char** argv) {
    const char* appRootPath = schema_resolveAppRootPath(argv[0]);
    const Paths paths = schema_getPaths(
        schema_joinPaths(
            appRootPath,
            INTERNAL_CONFIGURATION_PATH,
            ""));
    const char* messagesPath = schema_joinPaths(
            appRootPath,
            paths.internal,
            "/messages.yaml");

    const Messages messages = schema_getMessages(messagesPath);
    schema_printMessage(messages.help);

    return 0;
}