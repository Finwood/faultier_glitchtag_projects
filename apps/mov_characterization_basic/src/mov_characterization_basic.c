#include "uart.h"
#include "gpio.h"

#pragma GCC push_options
#pragma GCC optimize("O0")

__ramfunc void glitch_target() {
	// Indicate reset to our glitcher
	print_uart("RST");

	uint32_t output;
	uint32_t register_output[14] = {0};

	// Generate a short trigger signal on IO 2
	gpio_pin_set_dt(&trigger, 1);
	k_msleep(1);
	gpio_pin_set_dt(&trigger, 0);

	// 20 NOPs
	__asm("mov %0, #0xAAAAAAAA;"

		"NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;"
		"NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;"
		"mov %0, #0x55555555;"
		"NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;"
		"NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;"
		: "=r" (output)
		: [register_output]"r"(register_output)
		: "memory");
	
	print_uart("OKAY");
	for(int i=0; i < 14; i++) {
		uint32_t value = register_output[i];
		putchar(value & 0xFF);
		putchar((value >> 8) & 0xFF);
		putchar((value >> 16) & 0xFF);
		putchar((value >> 24) & 0xFF);
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
