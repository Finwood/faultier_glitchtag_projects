#include "gpio.h"

#define LED0_NODE DT_ALIAS(led0)
const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
#define TRIGGER_NODE DT_ALIAS(trigger)
const struct gpio_dt_spec trigger = GPIO_DT_SPEC_GET(TRIGGER_NODE, gpios);
#define TRIGGER2_NODE DT_ALIAS(trigger2)
const struct gpio_dt_spec trigger2 = GPIO_DT_SPEC_GET(TRIGGER2_NODE, gpios);

int setup_gpio() {
	if (!gpio_is_ready_dt(&trigger)) {
		return 1;
	}

	int ret = gpio_pin_configure_dt(&trigger, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) {
		return 1;
	}

	if (!gpio_is_ready_dt(&trigger2)) {
		return 1;
	}

	ret = gpio_pin_configure_dt(&trigger2, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) {
		return 1;
    }

    return 0;
}

int setup_led() {
	if (!gpio_is_ready_dt(&led)) {
		return 1;
	}

	int ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) {
		return 1;
	}
}