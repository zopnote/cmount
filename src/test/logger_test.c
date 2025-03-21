#include <core.h>
#include <stdlib.h>
#include <string.h>

int main() {
    logger_t* test_logger = logger_create(
        "test_logger", false, true, logger_write
    );
    char bin_path[128];
    exe_dir(bin_path, 128);
    logger_mk_file(test_logger, true, bin_path);
    test_logger->log(test_logger, status, "Hello World!");
    const char* ending = "logs";
    const size_t log_dir_size = strlen(bin_path) + strlen(ending) + 1;
    char* log_dir = malloc(log_dir_size);
    sprintf(log_dir, "%s/%s", bin_path, ending);
    logger_clean_logs(log_dir, 5);
}
