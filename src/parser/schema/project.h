#pragma once
#include <klib/kvec.h>

void hello();
typedef kvec_t(char*) kvec_str;
struct dependency
{
    char* name;
    char* version;
    char* relativeIncludePath;

};

struct environment
{
    kvec_str dependenceReferences;
    bool downloadSharedLibraries;
};


struct
{
    char* name;
    char* description;
    float* version;
    struct environment environment;
} typedef Project;