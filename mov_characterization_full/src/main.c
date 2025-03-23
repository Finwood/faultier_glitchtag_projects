#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/gpio.h>

#include <string.h>

/* change this to any other UART peripheral if desired */
#define UART_DEVICE_NODE DT_CHOSEN(zephyr_shell_uart)

#define MSG_SIZE 32

/* queue to store up to 10 messages (aligned to 4-byte boundary) */
K_MSGQ_DEFINE(uart_msgq, MSG_SIZE, 10, 4);

static const struct device *const uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);

/* receive buffer used in UART ISR callback */
static char rx_buf[MSG_SIZE];
static int rx_buf_pos;

#define LED0_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
#define TRIGGER_NODE DT_ALIAS(trigger)
static const struct gpio_dt_spec trigger = GPIO_DT_SPEC_GET(TRIGGER_NODE, gpios);
#define TRIGGER2_NODE DT_ALIAS(trigger2)
static const struct gpio_dt_spec trigger2 = GPIO_DT_SPEC_GET(TRIGGER2_NODE, gpios);

/*
 * Read characters from UART until line end is detected. Afterwards push the
 * data to the message queue.
 */
void serial_cb(const struct device *dev, void *user_data)
{
	uint8_t c;

	if (!uart_irq_update(uart_dev)) {
		return;
	}

	if (!uart_irq_rx_ready(uart_dev)) {
		return;
	}

	/* read until FIFO empty */
	while (uart_fifo_read(uart_dev, &c, 1) == 1) {
		if ((c == '\n' || c == '\r') && rx_buf_pos > 0) {
			/* terminate string */
			rx_buf[rx_buf_pos] = '\0';

			/* if queue is full, message is silently dropped */
			k_msgq_put(&uart_msgq, &rx_buf, K_NO_WAIT);

			/* reset the buffer (it was copied to the msgq) */
			rx_buf_pos = 0;
		} else if (rx_buf_pos < (sizeof(rx_buf) - 1)) {
			rx_buf[rx_buf_pos++] = c;
		}
		/* else: characters beyond buffer size are dropped */
	}
}

/*
 * Print a null-terminated string character by character to the UART interface
 */
void print_uart(char *buf)
{
	int msg_len = strlen(buf);

	for (int i = 0; i < msg_len; i++) {
		uart_poll_out(uart_dev, buf[i]);
	}
}

#pragma GCC push_options
#pragma GCC optimize("O0")
#define OUTER_LOOP_CNT 100
#define INNER_LOOP_CNT 100

#define LOOP_LENGTH 100

void glitch_target() {
	// Indicate reset to the glitcher
	uart_poll_out(uart_dev, 'R');
	uart_poll_out(uart_dev, 'S');
	uart_poll_out(uart_dev, 'T');

	// Prepare variables etc here
	uint32_t dest[14] = {0};
	int output = 0x1234;

	// Generate a short trigger signal on IO 2
	gpio_pin_set_dt(&trigger, 1);
	k_msleep(1);
	gpio_pin_set_dt(&trigger, 0);
	// 20 NOPs
	__asm("mov %[output], #0xAAAAAAAA;"

	" NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; "
	"NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; "
	" NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; "
	"NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; "
	"mov %[output], #0x55555555;"
	"NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; "
	"NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; "
	"NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; "
	"NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; "
	"NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; "
	"NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; "
	"NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; "
	"NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; "
	"stm %[dest], {r0-r12, lr};"
	: [output]"=&r" (output)
	// :
	// :);
	: [dest]"r"(dest)
	: "memory");
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");
	__asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; ");
	

	// uart_poll_out(uart_dev, output & 0xFF);
	// uart_poll_out(uart_dev, (output >> 8) & 0xFF);
	// uart_poll_out(uart_dev, (output >> 16) & 0xFF);
	// uart_poll_out(uart_dev, (output >> 24) & 0xFF);

	uart_poll_out(uart_dev, 'O');
	uart_poll_out(uart_dev, 'K');
	uart_poll_out(uart_dev, 'A');
	uart_poll_out(uart_dev, 'Y');
	for(int i=0; i < 14; i++) {
		uint32_t value = dest[i];
		// Output value via UART
		uart_poll_out(uart_dev, value & 0xFF);
		uart_poll_out(uart_dev, (value >> 8) & 0xFF);
		uart_poll_out(uart_dev, (value >> 16) & 0xFF);
		uart_poll_out(uart_dev, (value >> 24) & 0xFF);
	}

	while(1) {}
}

#pragma GCC pop_options

int main(void)
{
	char tx_buf[MSG_SIZE];

	if (!device_is_ready(uart_dev)) {
		printk("UART device not found!");
		return 0;
	}


	if (!gpio_is_ready_dt(&trigger)) {
		return 0;
	}

	int ret = gpio_pin_configure_dt(&trigger, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) {
		return 0;
	}

	if (!gpio_is_ready_dt(&trigger2)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&trigger2, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) {
		return 0;
	}

	/* configure interrupt and callback to receive data */
	ret = uart_irq_callback_user_data_set(uart_dev, serial_cb, NULL);

	if (ret < 0) {
		if (ret == -ENOTSUP) {
			printk("Interrupt-driven UART API support not enabled\n");
		} else if (ret == -ENOSYS) {
			printk("UART device does not support interrupt-driven API\n");
		} else {
			printk("Error setting UART callback: %d\n", ret);
		}
		return 0;
	}
	uart_irq_rx_enable(uart_dev);

	while(1) {
		glitch_target();
	}
	return 0;
}
