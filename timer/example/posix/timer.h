#ifndef TIMER_H_
#define TIMER_H_

#include <unistd.h>
#include <stdio.h>
#include <utility>
#include <vector>
#include <signal.h>
#include <time.h>
#include <pthread.h>

typedef void (*handler) (sigval_t sig);

u_int32_t getTime() {
    struct timespec time = {0, 0};
    clock_gettime(CLOCK_REALTIME, &time);
    return time.tv_sec * 1000 + time.tv_nsec / 1000000;
}

void setTimeout(handler fn, u_int32_t second) {
    struct sigevent evp;
    timer_t timer;

    evp.sigev_notify = SIGEV_THREAD;
    evp.sigev_value.sival_ptr = &timer;
    evp.sigev_notify_function = fn;
    evp.sigev_notify_attributes = nullptr;

    int ret = timer_create(CLOCK_REALTIME, &evp, &timer);
    if (ret) {
        printf("timer_create error\n");
    }

    struct itimerspec ts;
    ts.it_value.tv_sec = second / 1000;
    ts.it_value.tv_nsec = (second % 1000) * 1000000;
    ts.it_interval.tv_sec = 0;
    ts.it_interval.tv_nsec = 0;

    ret = timer_settime(timer, CLOCK_REALTIME, &ts, nullptr);
    if (ret) {
        printf("timer_settime error\n");
    }
}

#endif