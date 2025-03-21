#include <core.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

char* str_to_lower(const char* string)
{
    char* temp = strdup(string);
    for (char* cur = temp; *cur; ++cur) *cur = tolower(*cur);
    return temp;
}

bool superior_path(char* buffer, const char* path) {
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

bool fcontent(
    char* buffer,
    const size_t buffer_size,
    FILE* file
) {
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

    return true;
}

bool fcopy(
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

void str_replace(char* target, const char* needle, const char* replacement)
{
    char buffer[1024] = { 0 };
    char *insert_point = &buffer[0];
    const char *tmp = target;
    const size_t needle_len = strlen(needle);
    const size_t repl_len = strlen(replacement);

    while (1) {
        const char* p = strstr(tmp, needle);

        if (p == NULL) {
            strcpy(insert_point, tmp);
            break;
        }

        memcpy(insert_point, tmp, p - tmp);
        insert_point += p - tmp;

        memcpy(insert_point, replacement, repl_len);
        insert_point += repl_len;

        tmp = p + needle_len;
    }
    strcpy(target, buffer);
}