// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2023 Rene Wolf

#ifndef _FIFO_H
#define _FIFO_H

#include <stdint.h>
#include "usb_audio_format.h"

// how much entries of space the fifo should have
#define FIFO_SPACE 8

// initializes the fifo and its buffers. all buffers will be cleared and put into the empty queue
void fifo_init();

usb_audio_buffer* fifo_take_empty();
usb_audio_buffer* fifo_try_take_empty();
void              fifo_put_empty(usb_audio_buffer* buffer);

usb_audio_buffer* fifo_take_filled();
usb_audio_buffer* fifo_try_take_filled();
void              fifo_put_filled(usb_audio_buffer* buffer);

typedef enum
{
	fifo_token_none,
	fifo_token_start,
	fifo_token_stop
} fifo_token;

void       fifo_set_token(usb_audio_buffer* buffer, fifo_token token);
fifo_token fifo_get_token(usb_audio_buffer* buffer);


#endif

