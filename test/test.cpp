#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

int main() {
    int data[5] = { rand() % 100, rand() % 100, rand() % 100, rand() % 100, rand() % 100 };
    for (int i = 0; i < 5; ++i) {
        if (i % 5 == 0) {
            printf("Rank %d: ", i / 5);
        }
        printf("%d ", data[i]);
        if ((i + 1) % 5 == 0) {
            printf("\n");
        }
    }
}