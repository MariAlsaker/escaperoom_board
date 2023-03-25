#ifndef COM_H
#define COM_H

#include <stdint.h>

uint16_t fletcher16(void* data, uint16_t len);
void init_com();
uint8_t send_pack(uint8_t cmd, void* data, uint8_t len);
void tick_com();

#endif