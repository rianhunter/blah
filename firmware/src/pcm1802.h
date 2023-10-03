// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2023 Rene Wolf

#ifndef _PCM1802_H
#define _PCM1802_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "pico/stdlib.h"


void pcm1802_init();
void pcm1802_power_up();
void pcm1802_power_down();
// Blocking     receive of one sample on L+R channels in USB UAC PCM Type I format
void pcm1802_rx_24bit_uac_pcm_type1(uint8_t* l_3byte, uint8_t* r_3byte);
// Non-blocking receive of one sample on L+R channels in USB UAC PCM Type I format, returns true if successful
bool pcm1802_try_rx_24bit_uac_pcm_type1(uint8_t* l_3byte, uint8_t* r_3byte);

#ifdef __cplusplus
}
#endif

#endif /* _PCM1802_H */
