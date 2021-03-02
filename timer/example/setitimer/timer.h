#ifndef TIMER_H_
#define TIMER_H_

#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <queue>
#include <vector>
#include <utility>
#include <sys/time.h>
#include <time.h>

using std::vector;
using std::priority_queue;
using std::pair;

typedef void (*handler) ();

priority_queue<
    pair<u_int32_t, handler>, 
    vector<pair<u_int32_t, handler>>, 
    std::greater<typename vector<pair<u_int32_t, handler>>::value_type>
> queue;

u_int32_t getTime() {
    struct timespec time = {0, 0};
    clock_gettime(CLOCK_REALTIME, &time);

    return time.tv_sec * 1000 + time.tv_nsec / 1000000;
}

void __callback__(int sig) {
    vector<handler> wait;
    u_int32_t t = getTime();
    while (queue.size() > 0 && queue.top().first <= t) {
        wait.push_back(queue.top().second);
        queue.pop();
    }

    if (queue.size() > 0) {
        auto next = queue.top();
        struct itimerval tick;
        tick.it_value.tv_sec = (next.first - t) / 1000;
        tick.it_value.tv_usec = ((next.first - t) % 1000) * 1000;

        auto res = setitimer(ITIMER_REAL, &tick, NULL);
        if (res) {
            printf("setitimer error\n");
        }
    }

    for (auto fn : wait) {
        fn();
    }
}

void setTimeout(handler fn, u_int32_t second) {
    struct itimerval tick;
    u_int32_t t = getTime() + second;

    if (queue.size() > 0) {
        auto recentTask = queue.top();
        if (t < recentTask.first) {
            // 取消当前任务，重新执行任务
            setitimer(ITIMER_REAL, &tick, NULL);

            tick.it_value.tv_sec = second / 1000;
            tick.it_value.tv_usec = (second % 1000) * 1000;
            setitimer(ITIMER_REAL, &tick, NULL);
        }
    } else {
        tick.it_value.tv_sec = second / 1000;
        tick.it_value.tv_usec = (second % 1000) * 1000;
        auto res = setitimer(ITIMER_REAL, &tick, nullptr);
        if (res) {
            printf("setitimer error\n");
        }
    }

    queue.push({t, fn});
    signal(SIGALRM, __callback__);
}

#endif