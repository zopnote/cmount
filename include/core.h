#pragma once

#include <stdio.h>
#include <time.h>

/**
 * @brief The importance of a message.
 *
 * Notes will only be printed to verbose targets.
 */
typedef enum {
    error,
    warning,
    status,
    info,
    note
} logger_significance_t;

/**
 * @brief Color for a message that will be used if printing in the command line.
 *
 * The id is regardless, if you use an own color from the ansi color table.
 * The ansi_color_code is required.
 */
typedef struct {
    const int id;
    const int ansi_color_code;
} logger_color_t;


/**
 * Enum values of colors that maps to the actual ansi color codes via get_color().
 */
enum logger_colors_e {
    none,
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

/**
 * @brief Returns the actual logger_color_t values which are predefined for the logger_colors_e enum.
 */
inline logger_color_t get_color(
    const enum logger_colors_e color
) {
    static logger_color_t logger_colors[] = {
        none, 0,
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

/**
 * @brief Callback for a logger function.
 */
typedef void (*logger_callback_t) (
    logger_t* logger,
    logger_significance_t significance,
    const char* format,
    ...);

/**
 * @brief Represents a logger, its conditionals and targets.
 *
 * The time is the last time a message was sent.
 * If the file is set, it will get the messages.
 */
struct logger_s {
    const char* name;
    struct tm* time;
    FILE* file;
    bool verbose;
    bool should_print_in_console;
    logger_callback_t logger_log_function;
} typedef logger_t;


/**
 * @brief Structure to save the values for sequences of messages printed in to a logger.
 */
struct message {
    const logger_color_t color;
    const char* format;
    va_list args;
};

/**
 * @brief Creates a new logger.
 *
 * It is regardless if your set verbose and should_print_in_console,
 * if you create your own logger_log_function that ignores the fields of the
 * structure they are.
 *
 * @param name Name of the logger that will be printed.
 * @param verbose If logger_significance_e::note messages should be printed to stdout.
 * @param should_print_in_console If anything should be printed to stdout.
 * @param logger_log_function Function that will receive all messages the logger gets.
 * @return A new logger with the processed values.
 */
logger_t* logger_create(
    const char* name,
    bool verbose,
    bool should_print_in_console,
    logger_callback_t logger_log_function
);

/**
 * @brief Adds a file target to the logger.
 *
 * Note, that it always depends on the
 * logger_log_function() where messages go.
 *
 * This function is simply a wrapper to create a file,
 * checks if the file exists before and then continues with placement
 * as well as sets the logger file field.
 *
 * @param logger The logger which will get a file target.
 * @param name_file_after_logger_name If the file should be named after the name field of the logger structure.
 * @param directory_path The path where the logger file and its predecessors will be placed.
 */
void logger_set_file(
    logger_t* logger,
    bool name_file_after_logger_name,
    const char* directory_path
);

/**
 * @brief Disposes the logger and closes the file if set.
 *
 * No values of logger should be used after disposal.
 *
 * @param logger The logger which fields will be freed.
 */
void logger_dispose(
    logger_t* logger);

/**
 * @brief Writes in a formatted string to the logger targets.
 *
 * If the logger file is set, all messages are written to the file.
 * Printing to the stdout depends on should_print_in_console of logger.
 * Verbose messages will always be written into the file,
 * but if it would be printed in the stdout depends on verbose of logger.
 *
 * @param logger Logger which consists of the conditions.
 * @param significance Importance of the messages that will be printed.
 * @param format String that will be formated with the arguments, just like with printf().
 * @param ... Arguments that will be inserted in the print call.
 */
void logger_write(
    logger_t* logger,
    logger_significance_t significance,
    const char* format,
    ...);

/**
 * @brief Writes a sequence of strings in the logger.
 *
 * A sequence of messages can be used to print in
 * colored messages or strings from different locations in memory.
 *
 * If the logger file is set, all messages are written to the file.
 * Printing to the stdout depends on should_print_in_console of logger.
 * Verbose messages will always be written into the file,
 * but if it would be printed in the stdout depends on verbose of logger.
 *
 * @param logger Logger which consists of the conditions.
 * @param significance Importance of the messages that will be printed.
 * @param messages Array of format able colored strings.
 * @param messages_length Length of the array.
 */
void logger_write_sequence(
    logger_t* logger,
    logger_significance_t significance,
    const struct message* messages,
    size_t messages_length
);