#include <core.h>

#ifdef _WIN64 || _WIN32
#include <direct.h>
#include <io.h>
#include <stdbool.h>
#include <windows.h>

void os_get_current_working_directory(
    char* buffer,
    const size_t buffer_size
) {
    _getcwd(buffer, buffer_size);
}

void os_get_executable_directory(
    char* buffer,
    const size_t buffer_size
) {

    const DWORD len = GetModuleFileName(
        NULL,
        buffer,
        buffer_size
    );

    if (len > 0) {
        char* last_slash = strrchr(buffer, '\\');
        if (last_slash) {
            *last_slash = '\0';
        }
    }
}

bool os_can_access_file(const char* file_path) {
    return _access(file_path, 4) == 0;
}

bool os_make_directory(const char* directory_path) {
    return _mkdir(directory_path) == 0;
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

#endif
