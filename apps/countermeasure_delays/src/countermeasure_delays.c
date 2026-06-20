#include "uart.h"
#include "gpio.h"

#include <syscalls/random.h>
#include <zephyr/random/random.h>

#pragma GCC push_options
#pragma GCC optimize("O0")

static inline void random_delay() {
	uint8_t delay = sys_rand8_get();
	for(int i=0; i < delay; i++) {
		__asm("NOP;");
	}
}

__ramfunc void glitch_target() {
	uint32_t foo = 0;
	gpio_pin_set_dt(&trigger, 1);
	k_msleep(1);
	gpio_pin_set_dt(&trigger, 0);

	

	// Add some NOPs to make sure we actually target the assignment
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");

	random_delay();
	foo = 0x55555555;

	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");

	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");
	k_msleep(2);

	if(foo != 0x55555555) {
		printf_uart("SUCC");
	} else {
		printf_uart("OKAY");
	}

	while(1) {}
}

#pragma GCC pop_options

int main(void)
{
	if(setup_gpio()) {
		return 0;
	}

	if(setup_uart()) {
		return 0;
	}

	while(1) {
		glitch_target();
	}
	return 0;
}
