#pragma once

#include <stdio.h>
#include <time.h>

char* os_get_current_working_directory();
char* os_get_executable_directory();
char* os_create_directory();


/**
 * @brief The importance of a message.
 */
typedef enum {
    error,
    warning,
    info,
    note
} logger_significance_t;

/**
 * @brief Color for a message that will be
 * used if printing in the command line.
 */
typedef struct {
    const int id;
    const int ansi_color_code;
} logger_color_t;



enum logger_colors_e {
    reset,
    red,
    red_accent,
    green,
    green_accent,
    blue,
    blue_accent,
    light_blue,
    purple,
    pink,
    violet,
    orange,
    yellow
};

inline logger_color_t get_color(
    const enum logger_colors_e color
) {
    static logger_color_t logger_colors[] = {
        reset, 0,
        red, 124,
        red_accent, 196,
        green, 2,
        green_accent, 77,
        blue, 4,
        blue_accent, 38,
        light_blue, 33,
        purple, 207,
        pink, 200,
        violet, 54,
        orange, 202,
        yellow, 11
    };
    return logger_colors[color];
}


typedef struct logger_s logger_t;

typedef void (*logger_callback_t) (
    logger_t* const logger,
    const logger_color_t color,
    const char* format,
    ...
);

struct logger_s {
    const char* name;
    struct tm* time;
    FILE* file;
    bool should_print_in_console;
    bool verbose;
    logger_callback_t logger_write_function;
} typedef logger_t;


logger_t* logger_create(
    const char* prefix,
    bool verbose,
    bool should_print_in_console,
    logger_callback_t logger_write_function
);

void logger_set_file(
    logger_t* logger,
    const char* directory_path
);

void logger_dispose(
    logger_t* logger);

void logger_write(
    logger_t* logger,
    logger_color_t color,
    logger_significance_t significance,
    const char* format,
    ...);

struct message {
    const logger_color_t color;
    const char* format;
    va_list args;
};

void logger_writeSequence(
    logger_t* logger,
    const struct message* messages,
    size_t messages_length
);