#ifndef LOCK_CONTROL_H
#define LOCK_CONTROL_H

#include <stdint.h>

enum lock_index {
    LOCK_A = 0,
    LOCK_B = 1,
    LOCK_C = 2,
    LOCK_D = 3,
    LOCK_E = 4
};

enum hithold_states {
    HH_OFF,
    HH_REQ,
    HH_HIT,
    HH_HOLD
};

void init_lockcontrol(void);
void open_lock(enum lock_index idx);
void reset_locks(void);
void tick_locks(void);

#endif