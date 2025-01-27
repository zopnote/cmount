#pragma once

#define INTERNAL_CONFIGURATION_PATH "bin/internal/paths.yaml"

struct
{
    char* binary;
    char* internal;
    char* cache;
    char* library;
    char* include;
    char* runtime;
    char* config;
    char* scripts;
    char* templates;
} typedef Paths;

Paths schema_getPaths(const char* filePath);

char* schema_resolveAppRootPath(const char* executablePath);

char* schema_joinPaths(
    const char* appDirectoryPath,
    const char* relativeDirectory,
    const char* filename);