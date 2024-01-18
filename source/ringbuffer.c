/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "ringbuffer.h"


void ringbuffer_init(ringbuffer_t *fifo, uint8_t *buffer, uint16_t size)
{
    fifo->buffer = buffer;
    fifo->size = size;
    fifo->in = 0;
    fifo->out = 0;
}

uint16_t ringbuffer_getUsedSize(ringbuffer_t *fifo)
{
    if (fifo->in >= fifo->out)
        return (fifo->in - fifo->out);
    else
        return (fifo->size - fifo->out + fifo->in);
}

uint16_t ringbuffer_getRemainSize(ringbuffer_t *fifo)
{
    return (fifo->size - ringbuffer_getUsedSize(fifo) - 1);
}

uint8_t ringbuffer_isEmpty(ringbuffer_t *fifo)
{
    return (fifo->in == fifo->out);
}

void ringbuffer_in(ringbuffer_t *fifo, uint8_t *data, uint16_t len)
{
    for (int i = 0; i < len; i++)
    {
        fifo->buffer[fifo->in] = data[i];
        fifo->in = (fifo->in + 1) % fifo->size;
    }
}

uint8_t ringbuffer_in_check(ringbuffer_t *fifo, uint8_t *data, uint16_t len)
{
    uint16_t remainsize = ringbuffer_getRemainSize(fifo);

    if (remainsize < len)
        return 1;

    ringbuffer_in(fifo, data, len);

    return 0;
}

uint16_t ringbuffer_out(ringbuffer_t *fifo, uint8_t *buf, uint16_t readSize, uint16_t removeSize)
{
    uint16_t remainToread = ringbuffer_getUsedSize(fifo);
    if (removeSize > remainToread)
        removeSize = remainToread;
    
    if (remainToread > readSize) 
    {
        remainToread = readSize;
    }

    uint16_t outNdx = fifo->out;
    for (int i = 0; i < remainToread; i++)
    {
        buf[i] = fifo->buffer[outNdx];
        outNdx = (outNdx + 1) % fifo->size;
    }

    fifo->out = (fifo->out + removeSize) % fifo->size;

    return remainToread;
}


/*******************************END OF FILE************************************/


