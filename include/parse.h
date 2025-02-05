#pragma once
#include <stddef.h>

enum {
    yaml,
    json
} typedef ParseFormat;

enum {
    map,
    integer,
    string,
    floating,
    list
  } typedef ParseType;

struct {
    char* key;
    ParseType type;
    void* buffer;
} typedef ParseEntry;

void parse_resolveString(
    const char* string,
    const ParseFormat format,
    ParseEntry entriesBuffer[],
    size_t entriesBufferSize
);

void parse_emitString(
    char* stringBuffer,
    ParseEntry entriesBuffer[],
    size_t entriesBufferSize
);


