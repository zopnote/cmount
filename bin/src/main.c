#include <cthreads/cthreads.h>
#include <stdio.h>
#include <stdlib.h>


void* thread_function(void* arg) {
    printf("Thread läuft\n");
    return NULL;
}

int main() {
    printf("Executed successfull.\n");
    struct cthreads_thread my_thread;

    struct cthreads_args args = {0};

    int result = cthreads_thread_create(&my_thread, NULL, thread_function, NULL, &args);

    if (result != 0) {
        printf("Fehler beim Erstellen des Threads\n");
        return 1;
    }
    cthreads_thread_detach(my_thread);
    while (1);
    return 1;
} 