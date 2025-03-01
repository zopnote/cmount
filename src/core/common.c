#include <core.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

char* str_lwr(char* string)
{
    for (char* tmp = string; *tmp; ++tmp) {
        *tmp = tolower((unsigned char)*tmp);
    }
    return string;
}

bool parent_path(const char* path, char* buffer) {
    bool terminated = false;
    size_t i = strlen(path) + 1;
    while (i > 0) {
        i--;
        buffer[i] = path[i];

        if (terminated) {
            continue;
        }

        if (buffer[i] == '\\' || buffer[i] == '/') {
            buffer[i] = '\0';
            terminated = true;
        }

        if (i == 1) {
            return false;
        }
    }
    return true;
}

bool cpy_file(FILE* file, const char* new_file) {
    FILE* target_file = fopen(new_file, "w");
    if (!target_file) {
        return false;
    }

    const int result = fseek(file, 0, SEEK_SET);

    if (result != 0) {
        return false;
    }
    char read;
    while ((read = fgetc(file)) != EOF) {
        fputc(read, target_file);
    }

    fclose(target_file);
    return true;
}