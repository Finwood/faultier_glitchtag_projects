#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/gpio.h>

#include <string.h>
#include <stdio.h>
#include <stdarg.h>


/* change this to any other UART peripheral if desired */
#define UART_DEVICE_NODE DT_CHOSEN(zephyr_shell_uart)

#define RX_BUF_SIZE 8
#define MSG_SIZE 32

/* queue to store up to 10 messages (aligned to 4-byte boundary) */
K_MSGQ_DEFINE(uart_msgq, MSG_SIZE, RX_BUF_SIZE, 4);


// struct k_msgq uart_msgq;
static const struct device *const uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);

/* receive buffer used in UART ISR callback */
static char rx_buf[MSG_SIZE];
static int rx_buf_pos = 0;
// static int rx_buf_index = 0;

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
			uint32_t r = 0;
			if((r = k_msgq_put(&uart_msgq, &rx_buf, K_NO_WAIT)) != 0) {
				/* silently drop the message */
			}

			/* reset the buffer (it was copied to the msgq) */
			rx_buf_pos = 0;
		} else if (rx_buf_pos < MSG_SIZE - 1) {
			rx_buf[rx_buf_pos++] = c;
		}
		/* else: characters beyond buffer size are dropped */
	}
}

/*
 * Print a null-terminated string character by character to the UART interface
 */
void print_uart(const char *buf)
{
	int msg_len = strlen(buf);

	for (int i = 0; i < msg_len; i++) {
		uart_poll_out(uart_dev, buf[i]);
	}
}

void write_uart(const char *buf, size_t length) {
	for (int i = 0; i < length; i++) {
		uart_poll_out(uart_dev, buf[i]);
	}
}

char *read_uart() {
	static char serial_buffer[MSG_SIZE];
	while (k_msgq_get(&uart_msgq, &serial_buffer, K_FOREVER) != 0) {
	}
	return serial_buffer;
}

void printf_uart(const char *format, ...) {
    char buffer[256];
    va_list args;

    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    buffer[sizeof(buffer) - 1] = '\0';  // Ensure null-termination
    print_uart(buffer);
}

void putchar_uart(char c) {
	uart_poll_out(uart_dev, c);
}

int setup_uart() {
	if (!device_is_ready(uart_dev)) {
		printk("UART device not found!");
        return 1;
	}

	// k_msgq_alloc_init(&uart_msgq, MSG_SIZE, RX_BUF_SIZE);

	/* configure interrupt and callback to receive data */
	int ret = uart_irq_callback_user_data_set(uart_dev, serial_cb, NULL);


	if (ret < 0) {
		if (ret == -ENOTSUP) {
			printk("Interrupt-driven UART API support not enabled\n");
		} else if (ret == -ENOSYS) {
			printk("UART device does not support interrupt-driven API\n");
		} else {
			printk("Error setting UART callback: %d\n", ret);
		}
		return 1;
	}
	uart_irq_rx_enable(uart_dev);
    return 0;
}

void deobfuscate(unsigned char *data, unsigned int len, unsigned char key) {
    for (unsigned int i = 0; i < len; i++) {
        data[i] ^= key;
    }
}