// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2023 Rene Wolf

#include "dbg.h"
#include "pico/stdlib.h"
#include "main1.h"
#include "fifo.h"
#include "usb_audio_format.h"
#include "pcm1802.h"
#include "head_switch.h"

static void fill_buffer(usb_audio_buffer* buffer)
{
	for(int i=0; i<USB_AUDIO_SAMPLES_PER_BUFFER; ++i)
	{
		uint8_t* current_frame = buffer->data + ( i * USB_AUDIO_CHANNELS * USB_AUDIO_BYTES_PER_SAMPLE );

		// left goes into ch0, right into ch1
		pcm1802_rx_24bit_uac_pcm_type1(current_frame, current_frame + USB_AUDIO_BYTES_PER_SAMPLE );

		// head switch / sync pin goes into ch2
		uint32_t pin_pcm_value = head_switch_sample_pin() ? USB_AUDIO_PCM24_MAX : USB_AUDIO_PCM24_MIN;
		usb_audio_pcm24_host_to_usb(current_frame + (2*USB_AUDIO_BYTES_PER_SAMPLE), pin_pcm_value);
	}
}

void main1()
{
	dbg_say("main1()\n");
	
	head_switch_init();
	pcm1802_init();
	pcm1802_power_up();
	
	while(1)
	{
		usb_audio_buffer* buffer = fifo_take_empty();
		fill_buffer(buffer);
		fifo_put_filled(buffer);
	}
}
