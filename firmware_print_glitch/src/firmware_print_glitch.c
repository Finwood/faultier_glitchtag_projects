#include "uart.h"
#include "gpio.h"

#pragma GCC push_options
#pragma GCC optimize("O0")

uint32_t load_data = 0x12345678;
const char my_string[] __attribute__((section(".gnu.linkonce.irq_vector_table.zzz"))) = "GlitchTag Bootloader\n";

__ramfunc void glitch_target() {
	write_uart(my_string, sizeof(my_string));
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
