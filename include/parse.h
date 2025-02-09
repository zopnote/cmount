#pragma once

#include <stdio.h>

#ifdef _WIN64
    #ifdef PARSE_EXPORTS
        #define EXTERN __declspec(dllexport)
    #else
        #define EXTERN __declspec(dllimport)
    #endif
#else
    #define EXTERN __attribute__((visibility("default")))
#endif

enum {
    yaml,
    json
} typedef ParseFormat;

enum {
    entries,
    integer,
    string,
    floating,
    list
} typedef ParseType;

struct {
    char* key;
    ParseType type;
    void* buffer;
    size_t bufferSize;
} typedef ParseEntry;

EXTERN void parse_resolveString(
    const char* string,
    const ParseFormat format,
    const size_t entriesBufferSize,
    ParseEntry entriesBuffer[]
);
EXTERN void parse_emitString(
    char* stringBuffer,
    const ParseFormat format,
    const size_t entriesBufferSize,
    ParseEntry entriesBuffer[]
);