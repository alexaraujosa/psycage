#include "clock.h"

Clock defaultClock() {
    Clock clock = (Clock)malloc(sizeof(CLOCK));
    if (clock == NULL) return NULL;

    clock->ticks = 0;
    clock->maxTicks = 1000 / TICK_DURATION_MS;
    clock->blocked = 0;

    return clock;
}

void destroyClock(Clock clock) {
    list_remove(g_gamestate->clocks, clock);
    free(clock);
}

LinkedList init_clocks() {
    // Clock clock = defaultClock();
    // ListNode node = list_create_node(clock);
    // return node;

    LinkedList ll = list_create();
    Clock clock = defaultClock();

    list_add(ll, clock);

    return ll;
}

void destroy_clocks() {
    LinkedList ll = g_gamestate->clocks;
	if (ll == NULL) return;

    ListNode current = ll->head;
    while (current != NULL) {
        void* data = current->data;

        destroyClock((Clock)data);
        
        current = current->next;
    }

    // ListNode clk = g_gamestate->clocks;

	// while (clk != NULL) {
	// 	Clock node_clock = (Clock)clk->data;
	// 	destroyClock(node_clock);

	// 	clk = clk->next;
	// }
}

ListNode addClock(Clock clock) {
    if (!list_has(g_gamestate->clocks, clock)) {
        ListNode node = list_add(g_gamestate->clocks, clock);
        return node;
    } else {
        return NULL;
    }
}

void removeClock(Clock clock) {
    if (list_has(g_gamestate->clocks, clock)) {
        list_remove(g_gamestate->clocks, clock);
    }
}

void resetClock(Clock clock) {
    clock->ticks = 0;
    clock->blocked = 0;
}

void stopClock(Clock clock) {
    clock->blocked = TRUE;
}

void resumeClock(Clock clock) {
    clock->blocked = FALSE;
}

void tickClock(Clock clock) {
    clock->ticks++;

    if (clock->ticks >= clock->maxTicks) clock->ticks = 0;
}