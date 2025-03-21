#include <core.h>
#include <parse.h>
#include <stdlib.h>
#include <string.h>

static parse_entry_t paths[] = {
    "binary", string, NULL, 0,
    "internal", string, NULL, 0,
    "cache", string, NULL, 0,
    "library", string, NULL, 0,
    "include", string, NULL, 0,
    "runtime", string, NULL, 0,
    "config", string, NULL, 0,
    "scripts", string, NULL, 0,
    "templates", string, NULL, 0
};

int main() {
    logger_t* logger = logger_create(
        "parse_test", false, true, logger_write
    );
    char bin_path[128];
    exe_dir(bin_path, 128);
    logger_mk_file(logger, true, bin_path);

    strcat(bin_path, "/internal/paths.yaml");
    FILE* yaml_file = fopen(bin_path, "r");
    fseek(yaml_file, 0, SEEK_END);
    const long file_size = ftell(yaml_file);
    char* file_content = malloc(file_size + 1);
    fcontent(file_content, file_size, yaml_file);
    fclose(yaml_file);

    parse_resolve(file_content, paths, 9, logger);
    exe_dir(bin_path, 128);
    const char* ending = "logs";
    const size_t log_dir_size = strlen(bin_path) + strlen(ending) + 1;
    char* log_dir = malloc(log_dir_size);
    sprintf(log_dir, "%s/%s", bin_path, ending);
    logger_clean_logs(log_dir, 5);
    logger_del(logger);

}
