// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2023 Rene Wolf

#include "dbg.h"
#include "pico/stdlib.h"
#include "main1.h"
#include "fifo.h"
#include "usb_audio_format.h"

static void set_s24_sample(uint8_t* buffer, int sample_offset, int32_t value)
{
	// shifts on negative numbers are implementation dep. https://stackoverflow.com/questions/1857928/right-shifting-negative-numbers-in-c
	// so we'll have to do sign extension ourselves
	uint32_t v = *((uint32_t*)(&value));
	uint32_t sign_extension = (v & 0x80000000) ? 0xff000000 : 0;
	
	// buffers is in usb pcm format, which is msb first
	uint8_t* t =  buffer + (sample_offset * 3);
	
	v >>= 8;
	v |= sign_extension;
	t[2] = (v & 0xff);
	
	v >>= 8;
	v |= sign_extension;
	t[1] = (v & 0xff);
	
	v >>= 8;
	v |= sign_extension;
	t[0] = (v & 0xff);
}

static int32_t dummy_adc_value = 0;

static void fill_buffer(usb_audio_buffer* buffer)
{
	for(int i=0; i<USB_AUDIO_SAMPLES_PER_BUFFER; ++i)
	{
		int off = i*USB_AUDIO_CHANNELS;
		for(int c=0; c<USB_AUDIO_CHANNELS; ++c)
		{
			// USB PCM Type I interleaves samples from all channels
			set_s24_sample(buffer->data, off+c, dummy_adc_value);
		}
		
		// TODO this will generate a saw tooth, as its constantly rolling over.
		//  This should have a period of about 2^24/(48000 * 10) => 35 sec.
		//  Shift up 8 as the sample data is expected in the upper 24 bits
		dummy_adc_value+=(10<<8);
	}
}

void main1()
{
	bool run = false;
	dbg_say("main1()\n");
	
	while(1)
	{
		usb_audio_buffer* buffer = fifo_take_empty();
		//fill_buffer(buffer);
		fifo_put_filled(buffer);
	}
}
