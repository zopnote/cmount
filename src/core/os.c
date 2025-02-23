#include <core.h>
#include <stdbool.h>
#include <string.h>

bool os_copy_file_to_new_file(
    FILE* file,
    const char* new_file_path
) {
    FILE* target_file = fopen(
        new_file_path, "w"
    );

    if (!target_file) {
        return false;
    }

    const int result = fseek(
        file, 0, SEEK_SET
    );

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

#ifdef _WIN64 || _WIN32
#include <direct.h>
#include <io.h>
#include <windows.h>

bool os_get_current_working_directory(
    char* buffer,
    const size_t buffer_size
) {
    if (_getcwd(buffer, buffer_size)) {
        return true;
    }
    return false;
}

bool os_get_executable_directory(
    char* buffer,
    const size_t buffer_size
) {
    char directory_name[MAX_PATH];
    const auto strlen = GetModuleFileName(
        NULL,
        directory_name,
        MAX_PATH
    );

    if (strlen <= 0) {
        return false;
    }

    for (size_t i = strlen; i > 0; i--) {
        if (
            directory_name[i] == '\\' ||
            directory_name[i] == '/'
        ) {
            directory_name[i] = '\0';
            break;
        }
    }

    strcpy_s(
        buffer,
        buffer_size,
        directory_name
    );

    return true;
}

bool os_can_access_file(const char* file_path) {
    return _access(file_path, 4) == 0;
}

bool os_make_directory(const char* directory_path) {
    return _mkdir(directory_path) == 0;
}


int os_get_directory_files(
    char*** file_paths_buffer,
    const char* directory_path
) {
    WIN32_FIND_DATA found_file_data;
    HANDLE found_file = INVALID_HANDLE_VALUE;
    int file_count = 0;
    char searchPath[MAX_PATH];

    snprintf(searchPath, MAX_PATH, "%s\\*", directory_path);
    found_file = FindFirstFile(searchPath, &found_file_data);

    if (found_file == INVALID_HANDLE_VALUE) {
        return 0;
    }

    char** temp_buffer = NULL;

    while (FindNextFile(found_file, &found_file_data)) {

        if (!strcmp(found_file_data.cFileName, ".")) {
            continue;
        }

        if (!strcmp(found_file_data.cFileName, "..")) {
            continue;
        }

        if (found_file_data.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY) {
            continue;
        }

        char** new_buffer = realloc(
            temp_buffer,
            sizeof(char*) *
            (file_count + 1)
        );

        if (!new_buffer) {
            free(temp_buffer);
            FindClose(found_file);
            return 0;
        }

        temp_buffer = new_buffer;

        temp_buffer[file_count] = strdup(found_file_data.cFileName);
        if (!temp_buffer[file_count]) {
            free(temp_buffer);
            FindClose(found_file);
            return 0;
        }

        file_count++;

    }

    FindClose(found_file);
    *file_paths_buffer = temp_buffer;
    return file_count;
}


#else

#include <unistd.h>
#include <limits.h>
#include <libgen.h>
#include <sys/stat.h>
#include <stdbool.h>

void os_get_current_working_directory(
    char* buffer,
    const size_t buffer_size
) {
    getcwd(buffer, buffer_size);
}

void os_get_executable_directory(
    char* buffer,
    const size_t buffer_size
) {
    ssize_t len = readlink(
        "/proc/self/exe",
        buffer,
        buffer_size - 1
    );

    if (len != -1) {
        buffer[len] = '\0';
        char* dir = dirname(buffer);
        strncpy(buffer, dir, buffer_size);
    }
}

bool os_can_access_file(const char* file_path) {
    return access(file_path, R_OK) == 0;
}

bool os_make_directory(const char* directory_path) {
    return mkdir(directory_path, 0755) == 0;
}

int os_get_directory_files(
    char*** file_paths_buffer,
    const char* directory_path
) {
    DIR* directory;
    struct dirent* directory_entry;
    int file_count = 0;
    struct stat file_stat;
    char** temp_buffer = NULL;

    directory = opendir(directory_path);
    if (!directory) {
        return 0;
    }

    while ((directory_entry = readdir(directory)) != NULL) {
        char full_path[1024];
        snprintf(
            full_path,
            sizeof(full_path),
            "%s/%s",
            directory_path,
            directory_entry->d_name
        );
        if (
            stat(full_path, &file_stat) == 0 &&
            S_ISREG(file_stat.st_mode)
        ) {
            file_count++;
        }
    }

    temp_buffer = (char**)malloc(file_count * sizeof(char*));
    if (!temp_buffer) {
        closedir(directory);
        return 0;
    }

    rewinddir(directory);
    int index = 0;
    while ((directory_entry = readdir(directory)) != NULL) {
        char full_path[1024];
        snprintf(
            full_path,
            sizeof(full_path),
            "%s/%s",
            directory_path,
            directory_entry->d_name
        );

        if (
            stat(full_path, &file_stat) == 0 &&
            S_ISREG(file_stat.st_mode)
        ) {
            temp_buffer[index] = strdup(full_path);
            index++;
        }
    }

    closedir(directory);
    *file_paths_buffer = temp_buffer;
    return file_count;
}

#endif
