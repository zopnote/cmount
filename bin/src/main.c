#include <cthreads/cthreads.h>
#include <stdio.h>

// Die Funktion, die im neuen Thread ausgeführt wird
void* thread_function(void* arg) {
    printf("Thread läuft\n");
    return NULL;
}
int main() {
    // 1. Deklaration einer Thread-Referenz
    struct cthreads_thread my_thread;

    // 2. Erstellen des Threads und Zuweisen der Referenz
    struct cthreads_args args;  // Argumente für den Thread (optional, hier nicht verwendet)
    int result = cthreads_thread_create(&my_thread, NULL, thread_function, NULL, &args);

    if (result != 0) {
        printf("Fehler beim Erstellen des Threads\n");
        return 1;
    }

    // 3. Optional: Den Thread abtrennen, falls du ihn nicht später joinen willst
    cthreads_thread_detach(my_thread);
};