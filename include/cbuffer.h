#ifndef CBUFFER_H
#define CBUFFER_H
#include <stdint.h>

typedef struct {
    uint8_t* a;
    uint32_t type_size;
    uint32_t type_count;
    uint32_t length;

    uint8_t* in_ptr;
    uint8_t* out_ptr;
} cbuf_t;

cbuf_t init_cbuffer(void* static_array, uint32_t type_size, uint32_t type_count);
void push_cbuffer(cbuf_t* c, void* data);

#endif