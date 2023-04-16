#include <stdio.h>
#include <omp.h>

int main() {
    int num_threads = 8;
    int* ids = new int[num_threads];

#pragma omp parallel num_threads(num_threads)
    {
        int thread_id = omp_get_thread_num();
        ids[num_threads - thread_id - 1] = thread_id;
#pragma omp barrier

        if (thread_id == 0) {
            for (int i = 0; i < num_threads; i++) {
                printf("Thread ID: %d, Total threads: %d, Hello World!\n",ids[i], num_threads);
            }
        }
    }

    return 0;
}