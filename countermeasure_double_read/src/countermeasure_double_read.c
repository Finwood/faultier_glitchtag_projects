#include "uart.h"
#include "gpio.h"

#include <syscalls/random.h>
#include <zephyr/random/random.h>

#pragma GCC push_options
#pragma GCC optimize("O0")
static inline void fatal() 
{ 
    // should trigger a CPU reset or crash in some way 
    *(volatile uint32_t *)0; 
}
#define READ_REG32(addr) *(volatile uint32_t *)(addr) 


static inline void multi_read_reg32(volatile uint32_t *dst, void *src)
{
    *dst = READ_REG32(src);
    if (READ_REG32(src) != *dst || READ_REG32(src) != *dst)
        fatal();
}

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

	
	uint32_t *src = (uint32_t*)0x20000000;
	uint32_t dst;

	// Add some NOPs to make sure we actually target the assignment
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");

	multi_read_reg32(&dst, src);
	
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

	printf_uart("OKAY");
	write_uart(&dst, 4);

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
