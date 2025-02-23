#include <core.h>
#include <stdlib.h>
#include <string.h>

int main(const int argc, char** argv) {
    logger_t* main_logger = logger_create(
        "cm", true, true, logger_write
    );
    logger_t* parser_logger = logger_create(
        "parser", true, true, logger_write
    );

    char buffer[256];

    os_get_executable_directory(buffer, 256);

    logger_create_file_target(main_logger, false, buffer);
    logger_add_file_target(parser_logger, main_logger->file);

    main_logger->logger_log_function(
        main_logger,
        status,
        "Hello %s!",
        "world"
    );

    parser_logger->logger_log_function(
        parser_logger,
        status,
        "asd %s!",
        "asd"
    );
    parser_logger->logger_log_function(
        parser_logger,
        status,
        "asd %s!",
        "asd"
    );

    main_logger->logger_log_function(
        main_logger,
        status,
        "asdaasd %s!",
        "asd"
    );


    strcat(buffer, "/logs");
    logger_cleanup_logs(buffer, 25);
    logger_dispose(parser_logger);
    logger_dispose(main_logger);
    return EXIT_SUCCESS;
}
