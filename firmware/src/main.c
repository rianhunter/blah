#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "libsi5351/si5351.h"

int main(void)
{
	const uint LED_PIN = PICO_DEFAULT_LED_PIN;
	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);


	// Initialize I2C port at 100 kHz
	i2c_init(i2c0, 100 * 1000);
	
	// Pins
	const uint sda_pin = 12;
	const uint scl_pin = 13;
	
	// Initialize I2C pins
	gpio_set_function(sda_pin, GPIO_FUNC_I2C);
	gpio_set_function(scl_pin, GPIO_FUNC_I2C);
	gpio_pull_up(sda_pin);
	gpio_pull_up(scl_pin);
	
	si5351_init();
	si5351_begin(i2c0);
	

	si5351_setup_pll_int(SI5351_PLL_A, 32); // pll @ 800 MHz

	si5351_setup_multisynth(0, SI5351_PLL_A, 20, 0, 1); // output 0 => 800 / 20 => 40 MHz
	si5351_setup_rdiv(0, SI5351_R_DIV_1);

	si5351_setup_multisynth(1, SI5351_PLL_A, 16, 0, 1); // output 1 => 800 / 16 => 50 MHz
	si5351_setup_rdiv(1, SI5351_R_DIV_1);

	si5351_setup_multisynth(2, SI5351_PLL_A, 32, 0, 1); // output 2 => 800 / 32 => 25 MHz
	si5351_setup_rdiv(2, SI5351_R_DIV_1);
	
	si5351_enable_outputs(true);
	
	while (true)
	{
        gpio_put(LED_PIN, 1);
        sleep_ms(250);
        gpio_put(LED_PIN, 0);
        sleep_ms(750);
	}

	return 0;
}
