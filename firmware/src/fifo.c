// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2023 Rene Wolf

#include <string.h>
#include "fifo.h"
#include "pico/util/queue.h"
#include "dbg.h"

static usb_audio_buffer buffers[FIFO_SPACE];

static queue_t pipe_empty;
static queue_t pipe_full;


void fifo_init()
{
	queue_init(&pipe_empty, sizeof(usb_audio_buffer*), FIFO_SPACE);
	queue_init(&pipe_full, sizeof(usb_audio_buffer*), FIFO_SPACE);
	
	memset(buffers, 0, sizeof(buffers));
	
	for(int i=0; i<FIFO_SPACE; ++i)
	{
		usb_audio_buffer* tmp = &(buffers[i]);
		fifo_put_empty(tmp);
	}

	dbg_say("fifo init with ");
	dbg_u8(FIFO_SPACE);
	dbg_say(" slots in empty\n");
}

usb_audio_buffer* fifo_take_empty()
{
	usb_audio_buffer* ret;
	queue_remove_blocking(&pipe_empty, &ret);
	return ret;
}

usb_audio_buffer* fifo_take_filled()
{
	usb_audio_buffer* ret;
	queue_remove_blocking(&pipe_full, &ret);
	return ret;
}

usb_audio_buffer* fifo_try_take_empty()
{
	usb_audio_buffer* ret;
	if( queue_try_remove(&pipe_empty, &ret) == true )
		return ret;
	return NULL;
}

usb_audio_buffer* fifo_try_take_filled()
{
	usb_audio_buffer* ret;
	if( queue_try_remove(&pipe_full, &ret) == true )
		return ret;
	return NULL;
}

void fifo_put_empty(usb_audio_buffer* buffer)
{
	queue_add_blocking(&pipe_empty, &buffer);
}

void fifo_put_filled(usb_audio_buffer* buffer)
{
	queue_add_blocking(&pipe_full, &buffer);
}

#define FIFO_TOKEN 0x0C0FFEE0

void fifo_set_token(usb_audio_buffer* buffer, fifo_token token)
{
	uint32_t* w = (uint32_t*)buffer;
	
	if( token == fifo_token_none )
	{
		w[0] = 0;
	}
	else
	{
		w[0] = FIFO_TOKEN;
		w[1] = token;
	}
}

fifo_token fifo_get_token(usb_audio_buffer* buffer)
{
	uint32_t* w = (uint32_t*)buffer;
	if( w[0] != FIFO_TOKEN )
		return fifo_token_none;
	
	return (fifo_token)w[1];
}

#undef FIFO_TOKEN
