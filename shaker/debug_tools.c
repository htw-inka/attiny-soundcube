#include "debug_tools.h"

void initSoftUART() {
	softuart_init();
	softuart_turn_rx_off();

	sei();
}

void printInt(int32_t i) {
	char buf[PRINT_INT_BUFSIZE];
	int8_t sign = 1;
	int8_t len = 0;

	if (i < 0) { // look for the sign
		sign = -1;
		i = -i;
	}

	// fill buffer with digits (in reverse order)
	do {
		buf[len++] = 48 + i % 10;	// ASCII digits start at 48
	} while ((i /= 10) > 0 && len < PRINT_INT_BUFSIZE);

	if (sign < 0) {	// don't forget to add the sign
		buf[len] = '-';
	} else {
		len--;
	}

	// reverse output of the buffer
	do {
		softuart_putchar(buf[len--]);
	} while (len >= 0);
}

void printUInt(uint32_t i) {
	char buf[PRINT_INT_BUFSIZE];
	int8_t len = 0;

	// fill buffer with digits (in reverse order)
	do {
		buf[len++] = 48 + i % 10;	// ASCII digits start at 48
	} while ((i /= 10) > 0 && len < PRINT_INT_BUFSIZE);

	// reverse output of the buffer
	do {
		softuart_putchar(buf[--len]);
	} while (len > 0);
}