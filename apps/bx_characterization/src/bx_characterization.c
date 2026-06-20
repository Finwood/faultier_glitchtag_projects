#include "uart.h"
#include "gpio.h"

#pragma GCC push_options
#pragma GCC optimize("O0")

void branch_test_function() {
	print_uart("BRAN");
	// Endless loop
	while(1) {
	}
}

__ramfunc void glitch_target() {
	// Indicate reset to our glitcher
	print_uart("RST");

	uint32_t output;

	// Generate a short trigger signal on IO 2
	gpio_pin_set_dt(&trigger, 1);
	k_msleep(1);
	gpio_pin_set_dt(&trigger, 0);

	// 20 NOPs
	__asm("NOP;"

		"NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;"
		"NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;"
		"BX %[branch_test_function];"
		"NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;"
		"NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;"
		: 
		: [branch_test_function]"r"(branch_test_function)
		: "memory");
	
	print_uart("OKAY");

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
