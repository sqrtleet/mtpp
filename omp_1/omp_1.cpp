#include <stdio.h>
#include <omp.h>

int main() {

#pragma omp parallel for num_threads(8)
    for (int i = 0; i < 8; i++) {
        printf("Thread ID: %d, Total threads: %d, Hello World!\n", omp_get_thread_num(), omp_get_num_threads());
    }
    return 0;
}