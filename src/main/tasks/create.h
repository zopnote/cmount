#pragma once
#include "../schema/paths.h"
#include "../schema/messages.h"

void create(
    const char* appRootPath,
    const Paths paths,
    const Messages messages,
    char* arguments[],
    int argumentsLength);