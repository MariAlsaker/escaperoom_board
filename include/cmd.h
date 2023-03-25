#ifndef CMD_H
#define CMD_H

enum commands
{
    CMD_SYNC = 0x00,
    CMD_START = 0x01,
    CMD_OVERRIDE = 0x02,
    CMD_STOP = 0x03
};

enum stage_states
{
    UNSOLVED = 0x00,
    IN_PROGRESS = 0x01,
    TIMEOUT = 0x02,
    SOLVED = 0x03
};

#endif