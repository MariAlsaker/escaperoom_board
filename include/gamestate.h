#ifndef GAMESTATE_H
#define GAMESTATE_H

#define B_TIMEOUT_MS 5000
#define D_TIMEOUT_MS 5000
#define E_TIMEOUT_MS 5000

#define D_GCS_RANGE_LOWER 150
#define D_GCS_RANGER_UPPER 200

enum gamestates
{
    GS_A, // Airway Obstruction
    GS_B, // Pneumothorax
    GS_B_TIMEOUT,
    GS_C, // Bleeding
    GS_D, // Consciousness (keypad)
    GS_D_TIMEOUT,
    GS_E, // Exposure (keypad)
    GS_E_TIMEOUT,
    GS_FINISHED
};

enum handler_returns
{
    RET_INPROGRESS,
    RET_ERROR,
    RET_DONE
};

void init_gamestate(void);
void tick_gamestate(void);

#endif