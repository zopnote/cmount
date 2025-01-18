#pragma once

#include <stdlib.h>
#include "klib/kvec.h"

typedef kvec_t(char*) kvec_str;

struct
{
    kvec_str help;
} typedef Messages;

Messages schema_getMessages(const char* filePath);

void schema_printMessage(const kvec_str message);