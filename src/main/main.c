#include <core.h>
#include <stdlib.h>

int main(const int argc, char** argv) {

    logger_t* main_logger = logger_create(
        "Development", true, true, NULL
    );

    main_logger->logger_log_function(
        main_logger,
        status,
        "Hello %s!",
        "world"
    );

    return EXIT_SUCCESS;
}
