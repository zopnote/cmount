#include <core.h>
#include <stdlib.h>
#include <string.h>

int main(const int argc, char** argv) {
    logger_t* logger = logger_create(
        "main", true, true, logger_write
    );

    logger_t* parse_logger = logger_create(
        "parse", true, true, logger_write
    );

    char buffer[256];

    get_exe_dir(buffer, 256);

    logger_mk_file(logger, false, buffer);
    logger_add_file(parse_logger, logger->file);

    logger->func(
        logger,
        status,
        "bvn %s!",
        "world"
    );

    parse_logger->func(
        parse_logger,
        status,
        "asd %s!",
        "asd"
    );

    parse_logger->func(
        parse_logger,
        status,
        "asd %s!",
        "asd"
    );

    logger->func(
        logger,
        status,
        "asdaasd %s!",
        "asd"
    );

    strcat(buffer, "/logs");
    logger_clean_logs(buffer, 25);
    logger_del(parse_logger);
    logger_del(logger);
    return EXIT_SUCCESS;
}
