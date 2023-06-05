#ifndef __RL_GAMELOOP_CLOCK_H
#define __RL_GAMELOOP_CLOCK_H

#include "util/list.h"
#include "gameloop_common.h"

typedef struct clock {
    int ticks;
    int maxTicks;
    int blocked;
} CLOCK, *Clock;

/* FUNCTION SIGNATURES */
Clock defaultClock();
void destroyClock(Clock clock);
// ListNode init_clocks();
LinkedList init_clocks();
void destroy_clocks();
ListNode addClock(Clock clock);
void removeClock(Clock clock);
void resetClock(Clock clock);
void stopClock(Clock clock);
void resumeClock(Clock clock);
void tickClock(Clock clock);

#endif