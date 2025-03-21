#include <core.h>
#include <parse.h>
#include <stdlib.h>
#include <string.h>

int main() {
    logger_t* logger = logger_create(
        "main", false, true, logger_write
    );
    logger_t* parse_logger = logger_create(
        "parser", false, true, logger_write
    );
    char bin_path[256];
    get_exe_dir(bin_path, 256);
    logger_mk_file(logger, false, bin_path);
    logger_mk_file(parse_logger, true, bin_path);
    logger->log(logger, info, "CMount binaries located at %s", bin_path);
    logger->log(logger, info, "CMount binaries located at %s", bin_path);

    const char* ending = "logs";
    const size_t log_dir_size = strlen(bin_path) + strlen(ending) + 1;
    char* log_dir = malloc(log_dir_size);
    sprintf(log_dir, "%s/%s", bin_path, ending);
    logger_clean_logs(log_dir, 15);

    logger_del(parse_logger);
    logger_del(logger);
    return EXIT_SUCCESS;
}
