#include <core.h>
#include <parse.h>
#include <stdlib.h>
#include <string.h>

int main(const int argc, char** argv) {
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

    parse_entry_t entries[] = {
        "hello", string, NULL, 0
    };
    parse_resolve_yaml_string("\nhello: hello\n", entries, 1, parse_logger);

    logger->log(logger, info, "Cleanup log files...");
    strcat(bin_path, "/logs");
    logger_clean_logs(bin_path, 15);
    logger->log(logger, info, "Deleting logger...");
    logger_del(parse_logger);
    logger_del(logger);
    return EXIT_SUCCESS;
}
