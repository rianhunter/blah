// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2023 Rene Wolf

#include "dbg.h"
#include "pcm1802.h"
#include "pcm1802_fmt00.pio.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "usb_audio_format.h"

// see also https://www.pjrc.com/pcm1802-breakout-board-needs-hack/
#define PCM1802_POWER_DOWN_PIN 17


// NOTE GPIOs must be consecutive for the PIO to work and in the order of DATA, BITCLK, LRCLK
#define PCM_PIO_ADC0_DATA   18
#define PCM_PIO_ADC0_BITCLK 19
#define PCM_PIO_ADC0_LRCLK  20
// not connected / outputs debug info from PIO
#define PCM_PIO_ADC0_DEBUG  21

static_assert((PCM_PIO_ADC0_DATA + pcm1802_index_data)   == PCM_PIO_ADC0_DATA,   "ADC0 DATA GPIO not where it should be");
static_assert((PCM_PIO_ADC0_DATA + pcm1802_index_bitclk) == PCM_PIO_ADC0_BITCLK, "ADC0 BITCLK GPIO not where it should be");
static_assert((PCM_PIO_ADC0_DATA + pcm1802_index_lrclk)  == PCM_PIO_ADC0_LRCLK,  "ADC0 LRCLK GPIO not where it should be");
static_assert((PCM_PIO_ADC0_DATA + pcm1802_index_dbg)    == PCM_PIO_ADC0_DEBUG,  "ADC0 DEBUG GPIO not where it should be");




static PIO pio;
static uint32_t pio_program_offset;
static uint32_t pio_sm;

static uint32_t setup_pio(uint32_t pin)
{
	// https://github.com/raspberrypi/pico-examples/blob/a7ad17156bf60842ee55c8f86cd39e9cd7427c1d/pio/clocked_input/clocked_input.pio#L24
	// https://medium.com/geekculture/raspberry-pico-programming-with-pio-state-machines-e4610e6b0f29
	uint32_t sm = pio_claim_unused_sm(pio, true);

	pio_sm_config cfg = pcm1802_fmt00_program_get_default_config(pio_program_offset);


	// Set and initialize the input pins
	sm_config_set_in_pins(&cfg, pin);
	pio_sm_set_consecutive_pindirs(pio, sm, pin, (pcm1802_index_lrclk-pcm1802_index_data)+1, false);
	sm_config_set_jmp_pin(&cfg, pin + pcm1802_index_data);
	
	// Set and initialize the output pins
	sm_config_set_set_pins(&cfg, pin + pcm1802_index_dbg, 1);
	pio_sm_set_consecutive_pindirs(pio, sm, pin + pcm1802_index_dbg, 1, true);
	
	// we shift LEFT as we have MSB first on PCM interface
	sm_config_set_in_shift(&cfg, false, false, 32);
	
	// Connect these GPIOs to this PIO block
	pio_gpio_init(pio, pin + pcm1802_index_data);
	pio_gpio_init(pio, pin + pcm1802_index_bitclk);
	pio_gpio_init(pio, pin + pcm1802_index_lrclk);
	pio_gpio_init(pio, pin + pcm1802_index_dbg);

	// We only receive, so disable the TX FIFO to make the RX FIFO deeper.
	sm_config_set_fifo_join(&cfg, PIO_FIFO_JOIN_RX);

	// Load our configuration
	pio_sm_init(pio, sm, pio_program_offset, &cfg);
	
	return sm;
}

void pcm_pio_init()
{
	// https://github.com/raspberrypi/pico-examples/blob/a7ad17156bf60842ee55c8f86cd39e9cd7427c1d/pio/clocked_input/clocked_input.c#L45
	pio = pio0;
	pio_program_offset = pio_add_program(pio, &pcm1802_fmt00_program);
	
	pio_sm = setup_pio(PCM_PIO_ADC0_DATA);
	
	pio_sm_set_enabled(pio, pio_sm, true);
}

void pcm1802_init()
{
	gpio_init(PCM1802_POWER_DOWN_PIN);
	gpio_set_dir(PCM1802_POWER_DOWN_PIN, GPIO_OUT);
	pcm1802_power_down();
	pcm_pio_init();
}

void pcm1802_power_up()
{
	dbg_say("pcm1802_power_up\n");
	gpio_put(PCM1802_POWER_DOWN_PIN, 1);
}

void pcm1802_power_down()
{
	gpio_put(PCM1802_POWER_DOWN_PIN, 0);
	dbg_say("pcm1802_power_down\n");
}


void pcm1802_rx_24bit_uac_pcm_type1(uint8_t* l_3byte, uint8_t* r_3byte)
{
	while(pcm1802_try_rx_24bit_uac_pcm_type1(l_3byte, r_3byte) == false);
}

#define void_cast(ptr)   ((void*)(ptr))

bool pcm1802_try_rx_24bit_uac_pcm_type1(uint8_t* l_3byte, uint8_t* r_3byte)
{
	if( pio_sm_is_rx_fifo_empty(pio, pio_sm) )
		return false;
	
	uint32_t ch_l = pio_sm_get_blocking(pio, pio_sm);
	if( ch_l & 0x01000000 )
	{
		// we got a sample for the right channel -> out of sync, drop sample wait for next one
		dbg_say("pcm1802 out of sync, drop!!\n");
		ch_l = pio_sm_get_blocking(pio, pio_sm);
	}

	usb_audio_pcm24_host_to_usb(l_3byte, ch_l);

	uint32_t ch_r = pio_sm_get_blocking(pio, pio_sm);
	usb_audio_pcm24_host_to_usb(r_3byte, ch_r);

	return true;
}

#undef void_cast
