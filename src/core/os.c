#include <stdlib.h>

#ifdef _WIN64
#include <direct.h>
char* os_get_current_working_directory() {
    const size_t path_size = 1256;

    char* path = malloc(path_size * sizeof(char));
    _getcwd(path, path_size);
    return path;
}

char* os_get_executable_directory() {

}

char* os_create_directory() {

}
#else

#endif
