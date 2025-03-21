#include <core.h>
#include <parse.h>

int main() {
    logger_t* logger = logger_create(
        "parse_test", false, true, logger_write
    );
    char bin_path[128];
    get_exe_dir(bin_path, 128);
    logger_mk_file(logger, true, bin_path);

}