// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2023 Rene Wolf

#ifndef _USB_AUDIO_FORMAT_H
#define _USB_AUDIO_FORMAT_H

#include <stdint.h>
#include <assert.h>

#define USB_AUDIO_SAMPLES_PER_BUFFER 48*2
#define USB_AUDIO_BYTES_PER_SAMPLE   3
#define USB_AUDIO_CHANNELS           3
#define USB_AUDIO_PAYLOAD_SIZE       (USB_AUDIO_BYTES_PER_SAMPLE * USB_AUDIO_CHANNELS * USB_AUDIO_SAMPLES_PER_BUFFER)

typedef struct
{
	uint8_t data[USB_AUDIO_CHANNELS];
} usb_audio_buffer;


#endif

