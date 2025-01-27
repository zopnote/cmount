#include "create.h"
#include "../schema/paths.h"

void create(
    const char* appRootPath,
    const Paths paths,
    char* arguments[]) {

    const char* appTemplatePath =
        schema_joinPaths(
            appRootPath, paths.templates, "/app/");

}