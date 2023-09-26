// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2023 Rene Wolf

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/unique_id.h"
#include "tusb.h"

#include "clock_gen.h"
#include "dbg.h"
#include "main1.h"
#include "fifo.h"
#include "usb_descriptors.h"

int main(void)
{
	const uint led_pin = PICO_DEFAULT_LED_PIN;
	gpio_init(led_pin);
	gpio_set_dir(led_pin, GPIO_OUT);
	
	gpio_put(led_pin, 1);
	dbg_init();
	gpio_put(led_pin, 0);

	clock_gen_init();
	clock_gen_default();
	
	fifo_init();
	
	dbg_say("multicore launch\n");
	multicore_launch_core1(main1);
	
	char serial[USB_DESCRIPTOR_SERIAL_LEN+1];
	pico_get_unique_board_id_string(serial, sizeof(serial));
	usb_descriptor_set_serial(serial);

	tusb_init();
	dbg_say("tusb_init()\n");

	while (true)
	{
		// tinyusb device task
		tud_task(); 
		
		uint32_t t = time_us_32() >> 18;
		
		if( (t & 0x3) == 0 )
		{
			gpio_put(led_pin, 1);
		}
		else
		{
			gpio_put(led_pin, 0);
		}
	}

	return 0;
}

//--------------------------------------------------------------------+
// USB Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb()
{
	// NOOP
	dbg_say("mount\n");
}

// Invoked when device is unmounted
void tud_umount_cb()
{
	// NOOP
	dbg_say("unmount\n");
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
	// NOOP
	dbg_say("suspend\n");
}

// Invoked when usb bus is resumed
void tud_resume_cb()
{
	// NOOP
	dbg_say("resume\n");
}
