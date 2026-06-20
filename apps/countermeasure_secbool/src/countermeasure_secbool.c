#include "uart.h"
#include "gpio.h"

#include "secbool.h"

#pragma GCC push_options
#pragma GCC optimize("O0")

void __fatal_error(char *msg, int line, char *filename) {
	printf_uart("FATAL ERROR: %s:%d - %s\r\n", filename, line, msg);
}

#define ensure(expr, msg) \
  (((expr) == sectrue) ? (void)0 : __fatal_error(msg, __LINE__, __FILE__))

secbool return_secfalse() {
	return secfalse;
}

__ramfunc void glitch_target() {
	ensure(return_secfalse(), "Not true!!");

	print_uart("Successfully bypassed Trezor's secbool!\r\n");

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
