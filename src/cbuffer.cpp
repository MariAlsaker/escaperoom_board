#include "cbuffer.h"

cbuf_t init_cbuffer(uint8_t* static_array, uint32_t type_size, uint32_t type_count) {
    cbuf_t cbuf;
    cbuf.a = static_array;
    cbuf.type_count = type_count;
    cbuf.type_size = type_size;
    cbuf.length = type_count * type_size;

    cbuf.in_ptr = cbuf.a;
    cbuf.out_ptr = cbuf.a;

    return cbuf;
}

uint32_t size_cbuffer(cbuf_t* c) {
    if(c->in_ptr < c->out_ptr){ //If data is wrapped around
        return (c->out_ptr - c->in_ptr) - (c->type_count * c->type_size);
    }else{
        return c->in_ptr - c->out_ptr;
    }
}

uint32_t bytes_left_cbuffer(cbuf_t* c) {
    return (c->length) - size_cbuffer(c);
}

void push_cbuffer(cbuf_t* c, void* data) {
    uint8_t* a_end = c->a + c->length;

    //if(c->in_ptr + c->length)
}