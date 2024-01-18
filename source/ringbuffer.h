/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _RINGBUFFER_H_
#define _RINGBUFFER_H_


#include "stdint.h"

typedef struct
{
    uint8_t  *buffer;
    uint16_t size;
    uint16_t in;
    uint16_t out;
} ringbuffer_t;

void ringbuffer_init(ringbuffer_t *fifo, uint8_t *buffer, uint16_t size);

uint16_t ringbuffer_getUsedSize(ringbuffer_t *fifo);
uint16_t ringbuffer_getRemainSize(ringbuffer_t *fifo);
uint8_t ringbuffer_isEmpty(ringbuffer_t *fifo);

void ringbuffer_in(ringbuffer_t *fifo, uint8_t *data, uint16_t len);
uint8_t ringbuffer_in_check(ringbuffer_t *fifo, uint8_t *data, uint16_t len);
// read readSize bytes, remove removeSize bytes, when removeSize < readSize then consecutive reads have overlap
// and it is useful to support sliding window
uint16_t ringbuffer_out(ringbuffer_t *fifo, uint8_t *buf, uint16_t readSize, uint16_t removeSize);



#endif /* _RINGBUFFER_H_ */

/*******************************END OF FILE************************************/


