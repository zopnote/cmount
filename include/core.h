#pragma once

// ReSharper disable CppUnusedIncludeDirective
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>


/**
 * @brief Lowercases a string independent of platform.
 * @param string Input string.
 * @return Output string.
 */
char* str_lwr(const char* string);

/**
 * @brief Gets the parent path of a path.
 * @param path Path of entity which parent path will be written to the buffer.
 * @param buffer Buffer that will get the parent path.
 * @return Returns if the process was successful.
 */
bool parent_path(const char* path, char* buffer);


/**
 * @brief Writes the content of the file to the buffer.
 * @param path Path of the file.
 * @param buffer Pointer to the buffer.
 * @param buffer_size Max length of the buffer.
 * @return If the function proceed successful.
 */
bool read_file(
    const char* path, char* buffer, size_t buffer_size
);

/**
 * @brief Copies a file.
 * @param file File content that will be copied.
 * @param new_file Full path of the file that will be created.
 * @return Returns if the process was successful.
 */
bool cpy_file(FILE* file, const char* new_file);

/**
 * @brief Gets the directory the program runs at.
 *
 * @param buffer The buffer the path will be written to.
 * @param buffer_size Size of the buffer.
 * @return Returns if the buffer is set.
 */
bool get_work_dir(char* buffer, size_t buffer_size);


/**
 * @brief Gets the directory the executable is in.
 *
 * @param buffer The buffer the path will be written to.
 * @param buffer_size Size of the buffer.
 * @return Returns if the buffer is set.
 */
bool get_exe_dir(char* buffer, size_t buffer_size);


/**
 * @brief Tests if a file system entity exists.
 *
 * @param path Path of the entity that will be tested for.
 * @return Returns if the entity exists.
 */
bool can_access(const char* path);


/**
 * @brief Creates a directory.
 *
 * @param path Path of the directory that should be created.
 * @return Returns if the creation of the directory succeeded.
 */
bool mk_dir(const char* path);


/**
 * @brief Gets the files by path in a directory.
 *
 * @param buffer Buffer the found file paths will be saved to.
 * @param dir_path Path of the directory that will be scanned.
 * @return Returns the length of the buffer array, which is the count of found files.
 */
int get_dir_files(const char* dir_path, char*** buffer);


/**
 * @brief The importance of a message.
 *
 * Notes will only be printed to verbose targets.
 */
typedef enum {
    error,
    warning,
    status,
    info
} logger_significance_t;


typedef struct logger_s logger_t;


/**
 * @brief Callback for a logger function.
 *
 * Returns if an error occurs, either an error message
 * from the logger or if the callback has experienced an error.
 */
typedef bool (*logger_callback_t) (
    logger_t* logger,
    logger_significance_t significance,
    const char* format, ...);


/**
 * @brief Represents a logger, its conditionals and targets.
 *
 * The time is the last time a message was sent.
 * If the file is set, it will get the messages.
 */
typedef struct logger_s {
    const char* name;
    struct tm* time;
    FILE* file;
    bool own_file;
    bool verbose;
    bool print_out;
    logger_callback_t log;
} logger_t;


/**
 * @brief Structure to save the values for sequences of messages printed in to a logger.
 */
struct message {
    const char* format;
    char* args;
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
 * @param print_stdout If anything should be printed to stdout.
 * @param log_func Function that will receive all messages the logger gets.
 * @return A new logger with the processed values.
 */
logger_t* logger_create(
    const char* name,
    bool verbose,
    bool print_stdout,
    logger_callback_t log_func
);


/**
 * @brief Creates and add a file target to the logger.
 *
 * Note, that it always depends on the
 * logger_log_function() where messages go.
 *
 * This function is simply a wrapper to create a file,
 * checks if the file exists before and then continues with placement
 * as well as sets the logger file field.
 *
 * @param logger The logger which will get a file target.
 * @param named If the file should be named after the name field of the logger structure.
 * @param dir_path The path where the logger file and its predecessors will be placed.
 */
void logger_mk_file(
    logger_t* logger,
    bool named,
    const char* dir_path
);


/**
 * @brief Adds a file target to the logger.
 *
 * Note, that it always depends on the
 * logger_log_function() where messages go.
 *
 * Let the file write down messages to a specific file pointer.
 *
 * @param logger The logger which will get a file target.
 * @param file The file that the logger will write to.
 */
void logger_add_file(logger_t* logger, FILE* file);


/**
 * @brief Cleans the given directory by last time logs was written.
 *
 * @param log_dir_path The directory in which logs will be cleaned up.
 * @param max_log_files How many log files are allowed to exist before the function will clean them up. Default should be around 25.
 */
void logger_clean_logs(
    const char* log_dir_path,
    int max_log_files
);


/**
 * @brief Writes in a formatted string to the logger targets.
 *
 * If the logger file is set, all messages are written to the file.
 * Printing to the stdout depends on should_print_in_console of logger.
 * Verbose messages will always be written into the file,
 * but if it would be printed in the stdout depends on verbose of logger.
 *
 * @param logger Logger which consists of the conditions.
 * @param sign Importance of the messages that will be printed.
 * @param format String that will be formated with the arguments, just like with printf().
 * @param ... Arguments that will be inserted in the print call.
 * @return Returns result for error handling.
 */
bool logger_write(
    logger_t* logger,
    logger_significance_t sign,
    const char* format, ...);


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


/**
 * @brief Disposes the logger and closes the file if set.
 *
 * No values of logger should be used after disposal.
 *
 * @param logger The logger which fields will be freed.
 */
void logger_del(logger_t* logger);
