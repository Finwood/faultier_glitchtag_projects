#include "uart.h"
#include "gpio.h"
#include <zephyr/drivers/watchdog.h>

#pragma GCC push_options
#pragma GCC optimize("O0")
#define OUTER_LOOP_CNT 100
#define INNER_LOOP_CNT 100

#define LOOP_LENGTH 1000

// Watchdog variables
static const struct device *wdt_dev;
static int wdt_channel_id;

// Watchdog setup function
static int setup_watchdog(void)
{
	int ret;
	struct wdt_timeout_cfg wdt_config;

	// Get the watchdog device
	wdt_dev = DEVICE_DT_GET(DT_ALIAS(watchdog0));
	if (!device_is_ready(wdt_dev)) {
		printf("Watchdog device not ready\n");
		return -1;
	}

	// Configure watchdog timeout for 500ms
	wdt_config.flags = WDT_FLAG_RESET_SOC;
	wdt_config.window.min = 0U;
	wdt_config.window.max = 500U; // 500ms timeout
	wdt_config.callback = NULL;

	// Install the timeout
	wdt_channel_id = wdt_install_timeout(wdt_dev, &wdt_config);
	if (wdt_channel_id < 0) {
		printf("Watchdog install timeout failed: %d\n", wdt_channel_id);
		return -1;
	}

	// Setup and start the watchdog
	ret = wdt_setup(wdt_dev, 0);
	if (ret < 0) {
		printf("Watchdog setup failed: %d\n", ret);
		return -1;
	}

	printf("Watchdog enabled with 500ms timeout\n");
	return 0;
}

__ramfunc void glitch_target() {
	printf_uart("Resetted!\r\n");
	gpio_pin_set_dt(&trigger, 1);
	k_msleep(1);
	gpio_pin_set_dt(&trigger, 0);
	bool led_status = 0;
	while(1) {
		uint32_t cnt = 0, i, j;
		for (i = 0; i < LOOP_LENGTH; i++)
		{
			for (j = 0; j < LOOP_LENGTH; j++)
			{
				cnt++;
			}
		}
		if (i != LOOP_LENGTH || j != LOOP_LENGTH ||
			cnt != (LOOP_LENGTH * LOOP_LENGTH))
		{
			printf_uart("Glitch! %u %u %u\r\n", i, j, cnt);
			printf_uart("Your flag is: HXT{This-Was-Easy}\r\n", i, j, cnt);
			wdt_feed(wdt_dev, wdt_channel_id);
			// cls(false);
			// pprintfxy(5, 10, "Well done!");
			while(1) {
				wdt_feed(wdt_dev, wdt_channel_id);
			}
		} else {
			printf_uart("Normal %u %u %u\r\n", i, j, cnt);
			wdt_feed(wdt_dev, wdt_channel_id);
		}

		led_status = !led_status;
		gpio_pin_set_dt(&led, led_status);

		// gpio_put(26, !gpio_get_out_level (26));
	}
	
	// Endless loop
	while(1) {}
}

#pragma GCC pop_optimizations

int main(void)
{
	if(setup_gpio()) {
		return 0;
	}

	if(setup_led()) {
		return 0;
	}

	if(setup_uart()) {
		return 0;
	}

	if(setup_watchdog()) {
		return 0;
	}

	while(1) {
		glitch_target();
	}
	return 0;
}
