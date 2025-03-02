#include <core.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

static bool str_of_time(const struct tm* time, char* buffer) {
    return sprintf(
        buffer, "%02d:%02d:%02d, %02d-%02d-%d",
        time->tm_hour, time->tm_min, time->tm_sec,
        time->tm_mday, time->tm_mon, time->tm_year + 1900
    );
}

static struct tm* time_of_str(const char* str) {
    struct tm* time = malloc(sizeof(struct tm));
    if (!time) {
        perror("Error while parsing time for the logger");
        return NULL;
    }

    if (sscanf(
        str, "%d-%d-%d__%d-%d-%d_",
        &time->tm_hour, &time->tm_min, &time->tm_sec,
        &time->tm_mday, &time->tm_mon, &time->tm_year
    ) == 6) return time;

    if (sscanf(
        str, "%d:%d:%d, %d-%d-%d,",
        &time->tm_hour, &time->tm_min, &time->tm_sec,
        &time->tm_mday, &time->tm_mon, &time->tm_year
    ) == 6) return time;

    perror("Error while parsing time for the logger");
    free(time);
    return NULL;
}

static void replace_unknown_chars(char* buffer) {
    for (size_t i = 0; i < strlen(buffer); i++) {
        if (buffer[i] == ',' || buffer[i] == ' ') buffer[i] = '_';
        else if (buffer[i] == ':') buffer[i] = '-';
    }
}



logger_t* logger_create(
    const char* name,
    const bool verbose,
    const bool print_stdout,
    const logger_callback_t log_func
) {

    time_t raw_time;
    time(&raw_time);
    const struct tm* time_info = localtime(&raw_time);
    if (!time_info) {
        perror("Can't create logger. Can't get the local time");
        return NULL;
    }

    logger_t* logger = malloc(sizeof(logger_t));
    if (!logger) {
        perror("Failed to create logger");
        return NULL;
    }

    logger->time = malloc(sizeof(struct tm));
    if (!logger->time) {
        perror("Failed to create logger time");
        free(logger);
        return NULL;
    }

    *logger->time = *time_info;
    *logger = (logger_t) {
        .name = name,
        .verbose = verbose,
        .print_out = print_stdout,
        .log = log_func,
        .own_file = false,
        .file = NULL
    };

    return logger;
}

void logger_add_file(logger_t* logger, FILE* file) {
    logger->own_file = false;
    logger->file = file;
}

static bool get_file_meta(
    FILE* file,
    char* buffer,
    const size_t buffer_length
) {
    char cur;
    int end = -1, start = -1;
    while ((cur = fgetc(file)) != EOF) {
        if (cur == '(') start = ftell(file);
        else if (cur == ')') end = ftell(file);
    }
    if (start == -1 || end == -1 ||
        start >= end
    ) return false;

    fseek(file, start, SEEK_SET);
    const size_t size = end - start;
    if (size > buffer_length) return false;

    fread(buffer, sizeof(char), size, file);
    buffer[size] = '\0';

    sprintf(buffer, "%s", str_lwr(buffer));
    return true;
}

static bool archive_log(const char* file_path) {
    FILE* log_file = fopen(file_path, "r");
    if (!log_file) return fprintf(
        stderr, "Can't open log file to archive its content."
    );
    char meta[64];
    if (!get_file_meta(log_file, meta, 64)) {
        fprintf(stderr, "No valid meta in %s.\n", file_path);
        fclose(log_file);
        return false;
    }
    char* dir_path = malloc(strlen(file_path) + 1);
    if (!parent_path(file_path, dir_path)) {
        fclose(log_file);
        return false;
    }
    strcat(dir_path, "/logs");
    mk_dir(dir_path);

    replace_unknown_chars(meta);
    strcat(meta, ".log");
    sprintf(dir_path, "/%s", meta);

    if (!cpy_file(log_file, dir_path)) {
        fprintf(stderr, "File %s can't be copied.\n", file_path);
        fclose(log_file);
        return false;
    }
    fclose(log_file);
    return remove(file_path);
}

void logger_mk_file(
    logger_t* logger,
    const bool named,
    const char* dir_path
) {
    const char* unnamed = "latest";
    char* name = strdup(unnamed);

    if (named) {
        free(name);
        name = str_lwr(logger->name);
    }
    strcat(name, ".log");

    char* path = malloc(strlen(dir_path) + strlen(name) + 1);
    sprintf(path, "%s/%s", dir_path, name);

    bool archived = false;
    if (can_access(path)) archived = archive_log(path);
    if (!archived) if (!remove(path)) {
        if (!named) logger_mk_file(logger, true, dir_path);
        free(path);
        return;
    }
    logger->own_file = true;
    logger->file = fopen(path, "a");
    free(path);
    logger->log(logger, info, "Logger mounted to file.", 0);
}


bool logger_write(
    logger_t* logger,
    const logger_significance_t sign,
    const char* format, ...
) {
    va_list args;
    va_start(args, format);
    const int size = vsnprintf(NULL, 0, format, args) + 1;
    char* msg = malloc(size);
    vsnprintf(msg, size, format, args);
    va_end(args);

    char* meta = malloc(strlen(logger->name) + 24);
    time_t raw_time;
    time(&raw_time);
    struct tm* time = localtime(&raw_time);
    if (!time) {
        perror("Time cannot be initialized");
        free(msg);
        free(meta);
        return false;
    }
    if (!str_of_time(time, meta)) {
        perror("Time cannot be converted to valid string");
        free(msg);
        free(meta);
        return false;
    }
    sprintf(meta, "%s, %s", meta, logger->name);
    if (logger->time) free(logger->time);
    logger->time = malloc(sizeof(struct tm));
    if (!logger->time) {
        perror("Allocation of logger time failed");
        free(msg);
        free(meta);
        return false;
    }
    *logger->time = *time;


    const struct name_s {
        int sign;
        char* name;
        bool print_out;
    } names[] = {
        {error, "Error", true},
        {warning, "Warning", logger->print_out},
        {status, "Status", logger->print_out},
        {info, "Info", logger->print_out && logger->verbose}
    };

    if (names[sign].print_out) printf("%s\n", msg);

    if (!logger->file) {
        free(msg);
        free(meta);
        return sign != error;
    }

    char* final_msg = malloc(
        strlen(msg) + strlen(meta) +
        strlen(names[sign].name) + 6
    );
    sprintf(
        final_msg, "%s(%s): %s\n",
        names[sign].name, meta, msg
    );
    free(meta);
    free(msg);

    fwrite(
        final_msg, sizeof(char),
        strlen(final_msg), logger->file
    );
    free(final_msg);
    return sign != error;
}


static void compute_time_stamp(
    const char* log_path,
    unsigned long long* stamp_buffer
) {
    size_t start;
    for (start = strlen(log_path); start > 0; start--)
        if (log_path[start - 1] == '/' ||
            log_path[start - 1] == '\\'
        ) break;

    size_t end;
    for (end = strlen(log_path); end > 0; end--)
        if (log_path[end - 1] == '_') break;

    if (end <= start) {
        fprintf(stderr, "Error: end_index <= start_index.\n");
        return;
    }

    char* file_name = malloc(end - start + 1);
    strncpy(file_name, log_path + start, end - start);
    file_name[end - start] = '\0';

    struct tm* time = time_of_str(file_name);
    if (!time) {
        free(file_name);
        free(time);
        fprintf(stderr, "An error occurred while parsing the time.\n");
        return;
    }
    const unsigned long long time_stamp =
        time->tm_year * pow(10, 10) +
        time->tm_mon * pow(10, 8) +
        time->tm_mday * pow(10, 6) +
        time->tm_hour * pow(10, 4) +
        time->tm_min * pow(10, 2) +
        time->tm_sec;
    free(time);

    *stamp_buffer = time_stamp;
}

void logger_clean_logs(
    const char* log_dir_path,
    const int max_log_files
) {
    char** files = NULL;
    const int file_count = get_dir_files(
        log_dir_path, &files
    );
    if (!files) {
        if (file_count == 0) return;
        perror("Error while reading files of log directory");
        return;
    }

    if (max_log_files >= file_count) {
        for (int i = 0; i < file_count; i++) free(files[i]);
        free(files);
        return;
    }

    struct log_s {
        unsigned long long time_stamp;
        char* name;
        bool deletable;
    }* logs = malloc(file_count * sizeof(struct log_s));

    if (!logs) {
        perror("Error while allocation of logs list");
        return;
    }

    for (int i = 0; i < file_count; i++) {
        logs[i].deletable = false;
        logs[i].name = malloc(sizeof(char) * (
            strlen(files[i]) + strlen(log_dir_path) + 2
        ));

        if (!logs[i].name) {
            perror("Error while allocation of log name");
            free(logs);
            return;
        }
        sprintf(logs[i].name, "%s/%s", log_dir_path, files[i]);

        compute_time_stamp(files[i], &logs[i].time_stamp);
        if (files[i]) free(files[i]);

        if (i != 0 && i < file_count - i) {
            const size_t prev = i - 1;
            if (logs[prev].time_stamp < logs[i].time_stamp) {
                logs[prev].deletable = true;
            }
        }
    }

    for (int i = 0; i < file_count; i++) {
        if (!logs[i].deletable) free(logs[i].name);
        else {
            remove(logs[i].name);
            free(logs[i].name);
        }
    }
    free(files);
}

void logger_del(logger_t* logger) {
    if (!logger) return;
    if (!logger->time) free(logger->time);
    logger->log(
        logger, status, "Disposal of logger %s.", 1, logger->name
    );
    if (logger->file && logger->own_file) fclose(logger->file);
    logger = NULL;
}
