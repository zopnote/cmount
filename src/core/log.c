
#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "klib/kstring.h"



void write_log(const Importance importance, kstring_t message, kstring_t directory_path) {

    time_t raw_unix_time = time(NULL);

    if (raw_unix_time == -1) {
        perror("Error at requesting time.");
        return;
    }

    struct tm *local_unix_time = localtime(&raw_unix_time);
    if (local_unix_time == NULL) {
        perror("Error while converting the time to local.");
        return;
    }

    kstring_t formatted_date_time;
    memset(&formatted_date_time, 0, sizeof(kstring_t));

    ksprintf(&formatted_date_time, "%02s_%02m_%02_%02_%04",
             local_unix_time->tm_sec,
             local_unix_time->tm_min,
             local_unix_time->tm_hour,
             local_unix_time->tm_mday,
             local_unix_time->tm_mon,
             local_unix_time->tm_year);

    ksprintf(&message, "[%s](%s-%s): %s",
             importance,
             local_unix_time->tm_sec,
             formatted_date_time,
             message);

    FILE* file = fopen(directory_path.s, "r");

    if (file == NULL) {

        kstring_t error;
        memset(&error, 0, sizeof(kstring_t));
        ksprintf(&error, "Error while open file %s",
                 directory_path.s);

        perror(error.s);
        return;
    }

    char line[256];

    bool first_line = false;
    while (!first_line && fgets(line, sizeof(line), file)) {
        
        first_line = true;
    }
    fclose(file);
}