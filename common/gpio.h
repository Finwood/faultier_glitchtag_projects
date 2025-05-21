#pragma once
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define LED0_NODE DT_ALIAS(led0)
extern const struct gpio_dt_spec led;
#define TRIGGER_NODE DT_ALIAS(trigger)
extern const struct gpio_dt_spec trigger;
#define TRIGGER2_NODE DT_ALIAS(trigger2)
extern const struct gpio_dt_spec trigger2;

int setup_gpio();
