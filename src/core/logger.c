#include <core.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


static bool str_of_time(const struct tm* time, char* buffer) {

    return sprintf(
        buffer, "%02d:%02d:%02d, %02d-%02d-%d",
        time->tm_hour, time->tm_min, time->tm_sec,
        time->tm_mday, time->tm_mon, time->tm_year + 1900
    );
}

static void replace_illegal_chars_of(char* str) {
    for (size_t i = 0; i < strlen(str); i++) {
        if (str[i] == ':') {
            str[i] = '-';
            continue;
        }

        if (str[i] == ',' || str[i] == ' ') {
            str[i] = '_';
        }
    }
}

static struct tm* time_of_fstr(const char* fstr) {
    struct tm* time = malloc(sizeof(struct tm));
    if (!time) return NULL;

    if (sscanf(
        fstr, "%d-%d-%d__%d-%d-%d_",
        &time->tm_hour, &time->tm_min, &time->tm_sec,
        &time->tm_mday, &time->tm_mon, &time->tm_year
    ) != 6) {
        printf("Error while parsing time for the logger!\n");
        free(time);
        return NULL;
    }

    time->tm_mon -= 1;
    time->tm_year -= 1900;

    return time;
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
    logger_t* logger = malloc(
        sizeof(logger_t)
    );

    if (!logger) {
        return NULL;
    }

    *logger = (logger_t) {
        .name = name,
        .verbose = verbose,
        .print_out = print_stdout,
        .log = log_func
    };

    logger->time = malloc(sizeof(struct tm));
    if (!logger->time) {
        free(logger);
        return NULL;
    }
    *logger->time = *time_info;

    return logger;
}

void logger_add_file(logger_t* logger, FILE* file) {
    logger->own_file = false;
    logger->file = file;
}

static bool meta_of_file(FILE* file, char* buffer) {
    char cur_char;
    long bracket_start = -1;
    long bracket_end = -1;
    while ((cur_char = fgetc(file)) != EOF) {

        if (cur_char == '(') {
            bracket_start = ftell(file);
            continue;
        }
        if (cur_char == ')') {
            bracket_end = ftell(file);
        }
    }

    if (bracket_start == -1 || bracket_end == -1 ||
        bracket_start >= bracket_end
    ) {
        return false;
    }

    fseek(file, bracket_start, SEEK_SET);

    const size_t content_size =
        bracket_end - bracket_start - 1;

    fread(buffer, 1, content_size, file);
    buffer[content_size] = '\0';

    sprintf(buffer, "%s", str_lwr(buffer));
    return true;
}

static bool move_log(const char* file_path) {
    FILE* file = fopen(file_path, "r");

    char dir_path[strlen(file_path) + 76];
    if (!parent_path(file_path, dir_path)) {
        return false;
    }

    char meta_str[64];
    if (!meta_of_file(file, meta_str)) return false;
    replace_illegal_chars_of(meta_str);
    strcat(meta_str, ".log");

    strcat(dir_path, "/logs");
    mk_dir(dir_path);

    strcat(dir_path, "/");
    strcat(dir_path, meta_str);
    if (!cpy_file(file, dir_path)) return false;

    fclose(file);
    return !remove(file_path);
}


void logger_mk_file(
    logger_t* logger,
    const bool name_file,
    const char* dir_path
) {

    const char* std_name = "latest";
    char name[
        strlen(std_name) * sizeof(char) +
        strlen(logger->name) * sizeof(char) + 1
    ];
    name[0] = '\0';
    strcat(name, std_name);

    if (name_file) {

        char prefix[strlen(logger->name) * sizeof(char) + 1];
        char* lwr_name = str_lwr(strdup(logger->name));

        sprintf(prefix, "%s", str_lwr(lwr_name));
        strcpy(name, prefix);

        free(lwr_name);
    }

    strcat(name, ".log");
    char path[
        strlen(dir_path) * sizeof(char) +
        strlen(name) * sizeof(char) + 1
    ];
    path[0] = '\0';
    sprintf(path, "%s/%s", dir_path, name);

    if (can_access(path)) {
        const bool result = move_log(path);
        if (!result) {
            perror("Maybe you create the main logger twice? Cannot move latest.log file");
            logger_mk_file(logger, true, dir_path);
            return;
        }
    }

    logger->own_file = true;
    logger->file = fopen(path, "a");
}


bool logger_write(
    logger_t* logger,
    const logger_significance_t sign,
    const char* format,
    ...
) {
    #if _WIN32
        char* args = NULL;
    #else
        va_list args;
    #endif

    va_start(args, format);
    char msg[strlen(format) * sizeof(char) + 360];
    vsprintf(msg, format, args);
    va_end(args);

    size_t msg_meta_size =
        strlen(logger->name) * sizeof(char) + 24;
    char msg_meta_str[msg_meta_size];

    time_t raw_time;
    time(&raw_time);
    struct tm* time = localtime(&raw_time);
    if (!time) {
        perror("Time cannot be initialized");
        return false;
    }
    if (!str_of_time(time, msg_meta_str)) {
        perror("Time cannot be converted to valid string");
        return false;
    }
    sprintf(
        msg_meta_str,
        "%s, %s",
        msg_meta_str,
        logger->name
    );
    logger->time = time;


    struct name_s {
        int sign;
        char* name;
        bool print_out;
    };
    const struct name_s names[] = {
        error, "Error", true,
        warning, "Warning", logger->print_out,
        status, "Status", logger->print_out,
        info, "Info", logger->print_out && logger->verbose
    };

    if (names[sign].print_out) {
        printf("%s\n", msg);
    }

    if (!logger->file) {
        return sign != error;
    }

    char final_msg[
        strlen(msg) * sizeof(char) +
        strlen(msg_meta_str) * sizeof(char) +
        56 * sizeof(char)
    ];

    sprintf(
        final_msg,
        "%s(%s): %s\n",
        names[sign].name,
        msg_meta_str, msg
    );

    fwrite(
        final_msg,
        sizeof(char),
        strlen(final_msg),
        logger->file
    );
    return sign != error;
}

static void compute_time_stamp(
    const char* log_name,
    unsigned long long* stamp_buffer
) {
    for (size_t k = strlen(log_name); k > 0; k--) {
        if (log_name[k] == '_') {
            char fstr[k + 2];
            strncpy_s(fstr, k + 1, log_name, k);
            fstr[k + 1] = '\0';

            struct tm* time = time_of_fstr(fstr);
            const unsigned long long time_stamp =
                time->tm_year * pow(10, 10) +
                time->tm_mon * pow(10, 8) +
                time->tm_mday * pow(10, 6) +
                time->tm_hour * pow(10, 4) +
                time->tm_min * pow(10, 2) +
                time->tm_sec;
            free(time);

            *stamp_buffer = time_stamp;
            break;
        }
    }
}

struct log_s {
    unsigned long long time_stamp;
    char* name;
    bool deletable;
};

void logger_clean_logs(
    const char* log_dir_path,
    const int max_log_files
) {
    char** files = NULL;
    const int file_count = get_dir_files(
        log_dir_path,
        &files
    );
    if (!files) {
        perror("Error while reading files of log directory");
        return;
    }

    if (max_log_files >= file_count) {
        for (int i = 0; i < file_count; i++) free(files[i]);
        free(files);
        return;
    }


    struct log_s logs[file_count];

    for (int i = 0; i < file_count; i++) {
        logs[i].deletable = false;
        logs[i].name = malloc(
            sizeof(char) *
            (strlen(files[i]) + strlen(log_dir_path) + 2)
        );
        if (!logs[i].name) {
            perror("Error while allocation of log name");
            return;
        }
        sprintf(
            logs[i].name, "%s/%s", log_dir_path,
            files[i]
        );
        compute_time_stamp(files[i], &logs[i].time_stamp);
        if (files[i]) free(files[i]);

        if (i == 0) continue;
        if (i < file_count - i) {
            const size_t prev = i - 1;
            if (logs[prev].time_stamp < logs[i].time_stamp) {
                logs[prev].deletable = true;
            }
        }
    }
    for (size_t i = 0; i < file_count; i++) {
        if (!logs[i].deletable) {
            free(logs[i].name);
            continue;
        }
        if (remove(logs[i].name)) {
            perror("Error while cleanup previous logs");
        }
        free(logs[i].name);
    }
    free(files);
}

void logger_del(logger_t* logger) {
    if (!logger) return;

    if (!logger->time) {
        free(logger->time);
    }
    if (logger->file) {
        logger->log(
            logger,
            info,
            "Disposal of logger %s.",
            logger->name
        );
        if (logger->own_file) {
            fclose(logger->file);
        }
    }
    logger = NULL;
}
