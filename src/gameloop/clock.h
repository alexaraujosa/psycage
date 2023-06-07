/***************************************************************
 * CLOCK MODULE                                                *
 *                                                             *
 * This module handles the creation and handling of the dynamic*
 * clock system.                                               *
 *                                                             *
 * NOTE: During the creation of this module, we discovered     *
 * first hand that time does indeed move at different speeds   *
 * cause how the fuck did time go by so fast while making this *
 * thing?                                                      *
 ***************************************************************/

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
void resetAllClocks();
void resetAllClockTicks();
void stopClock(Clock clock);
void resumeClock(Clock clock);
void tickClock(Clock clock);

#endif