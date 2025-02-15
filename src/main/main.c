#include <core.h>
#include <stdlib.h>

int main(const int argc, char** argv) {
    logger_t* main_logger = logger_create(
        "main", true, true, logger_write
    );
    logger_t* secondary_logger = logger_create(
        "2nd", true, true, logger_write
    );

    char buffer[256];

    os_get_executable_directory(buffer, 256);

    logger_create_file_target(main_logger, false, buffer);
    logger_add_file_target(secondary_logger, main_logger->file);

    main_logger->logger_log_function(
        main_logger,
        status,
        "Hello %s!",
        "world"
        );

    secondary_logger->logger_log_function(
        secondary_logger,
        status,
        "asd %s!",
        "asd"
        );
    secondary_logger->logger_log_function(
        secondary_logger,
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

    return EXIT_SUCCESS;
}
