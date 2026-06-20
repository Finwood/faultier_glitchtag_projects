#include "uart.h"
#include "gpio.h"

#pragma GCC push_options
#pragma GCC optimize("O0")

__ramfunc void glitch_target() {
	// Indicate reset to our glitcher
	print_uart("RST");

	// Generate a short trigger signal on IO 2
	gpio_pin_set_dt(&trigger, 1);
	k_msleep(1);
	gpio_pin_set_dt(&trigger, 0);


	uint32_t foo = 0;

	// Add some NOPs to make sure we actually target the assignment
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");

	foo = 0x55555555;

	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");

	// Send OKAY (so we know that we actually reached this)
	// and print out the value (in hex)
	// Also print out the value of r7
	register unsigned r7_value __asm("r7");
	printf_uart("OKAY%08X%08X\n", foo, r7_value);

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
