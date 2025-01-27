#pragma once

void hello();
struct dependency
{
    char* name;
    char* version;
    char* relativeIncludePath;

};

struct environment
{
    bool dependenceReferences;
    bool downloadSharedLibraries;
};


struct
{
    char* name;
    char* description;
    float* version;
    struct environment environment;
} typedef Project;