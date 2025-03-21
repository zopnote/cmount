#include "tasks.h"

int main(int argc, char** argv) {


    if (task_exists(argv[1])) {
        printf("exists");
        return 0;
    }
    printf("no");
}