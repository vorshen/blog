#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <ctime>

#include "./timer.h"

int main(int argc, char* argv[]) {
    printf("[%u] 利利 0s 时间点\n", getTime());

    setTimeout([]() {
        printf("[%u] 利利第 2s 吨吨吨\n", getTime());

        setTimeout([]() {
            printf("[%u] 利利第 5s 吨吨吨\n", getTime());
        }, 3000);
    }, 2000);

    setTimeout([]() {
        printf("[%u] 利利第 1s 吨吨吨\n", getTime());
    }, 1000);

    setTimeout([]() {
        printf("[%u] 利利喝不动了\n", getTime());
    }, 5000);

    while (1) {
        pause();
    }
    return 0;
}