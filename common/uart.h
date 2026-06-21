#pragma once

#include <stddef.h>

int setup_uart();
void printf_uart(const char *format, ...);
void print_uart(const char *buf);
void write_uart(const char *buf, size_t length);
void putchar_uart(char c);
char * read_uart();
void deobfuscate(unsigned char *data, unsigned int len, unsigned char key);
