#include "uart.h"
#include "gpio.h"

#pragma GCC push_options
#pragma GCC optimize("O0")

uint32_t load_data = 0x12345678;

__ramfunc void glitch_target() {
	//  Indicate reset to our glitcher
	print_uart("RST");

	uint32_t register_output[14] = {0};

	// Generate a short trigger signal on IO 2
	gpio_pin_set_dt(&trigger, 1);
	k_msleep(1);
	gpio_pin_set_dt(&trigger, 0);

	// 1 setup instruction + 20 NOPs
	__asm(
		"ldr r0, =load_data;"
		"NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;"
		"NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;"
		"ldm r0, {r1};"
		"NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;"
		"NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;"
		"stm %[register_output], {r0-r12, lr};"
		: 
		: [register_output]"r"(register_output)
		: "r0", "r1", "memory");
	
	print_uart("OKAY");

	// Send all 14 registers to the host
	write_uart(register_output, sizeof(register_output));

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
