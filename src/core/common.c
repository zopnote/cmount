#include <core.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

char* str_lwr(const char* str)
{
    char* temp = strdup(str);
    for (char* cur = temp; *cur; ++cur) *cur = tolower(*cur);
    return temp;
}

bool parent_path(char* buffer, const char* path) {
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

bool read_file(
    const char* path,
    char* buffer,
    const size_t buffer_size
) {
    FILE* file = fopen(path, "r");
    if (!file) {
        perror("File cannot be opened");
        return false;
    }

    const int result = fseek(file, 0, SEEK_SET);
    if (result != 0) {
        perror("File cannot be read");
        return false;
    }

    char read;
    size_t i = 0;
    while ((read = fgetc(file)) != EOF) {
        if (i >= buffer_size) break;
        buffer[i] = read;
        i++;
    }
    buffer[i] = '\0';

    fclose(file);
    return true;
}

bool cpy_file(
    FILE* source_file, const char* destination_path
) {
    FILE* target_file = fopen(destination_path, "w");
    if (!target_file) {
        return false;
    }

    const int result = fseek(source_file, 0, SEEK_SET);

    if (result != 0) {
        return false;
    }
    char read;
    while ((read = fgetc(source_file)) != EOF) {
        fputc(read, target_file);
    }

    fclose(target_file);
    return true;
}
