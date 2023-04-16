#include <stdio.h>
#include <omp.h>

int main() {
    int n = 16000;
    int* arr = new int[n];
    int i;
    float* arr2 = new float[n];

    for (int i = 0; i < n; i++) {
        arr[i] = i;
    }

    // Инициализация крайних элементов массива arr2
    arr2[0] = arr[0];
    arr2[n - 1] = arr[n - 1];

#pragma omp parallel num_threads(8) shared(arr2)
    {
#pragma omp for private(i) schedule(static)
        for (i = 1; i < n - 1; i++) {
            arr2[i] = (arr[i - 1] + arr[i] + arr[i + 1]) / 3.0f;
        }
    }

    float avg = 0.0f;
    for (int i = 0; i < n; i++) {
        avg += arr2[i];
    }
    avg /= n;

    printf("Average value: %f", avg);
    return 0;
}
