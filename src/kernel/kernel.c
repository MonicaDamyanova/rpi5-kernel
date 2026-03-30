#include <stddef.h>
#include <stdint.h>

#include "mailbox.h"
#include "uart.h"

//volatile uint32_t uart_mbox[9] __attribute__((aligned(16))) = {
//    36, 0, MBOX_CLKRATE_TAG, 12, 8, 2, 0, 0, 0
//};

static volatile uint32_t mbox[7] __attribute__((aligned(16)));

void mailbox_test(void) {
    mbox[0] = 7 * 4;          // total size in bytes (28)
    mbox[1] = 0x00000000;     // request
    mbox[2] = 0x00010002;     // GET_BOARD_REVISION
    mbox[3] = 4;              // value buffer size
    mbox[4] = 0;              // tag request
    mbox[5] = 0;              // value placeholder
    mbox[6] = 0;              // end tag
}

void mailbox_call(void) {
    mailbox_write(MBOX_CH_PROP, (uint64_t)mbox);
    mailbox_read(MBOX_CH_PROP);
}

void uart_putc(unsigned char c) {
	while (read32(UART0_FR) & (1 << 5));
	write32(UART0_DR, c);
}

void uart_puts(const char* s) {
    while (*s != '\0') {
        uart_putc(*s);
        s++;
    }
}

void uart_put_u32(uint32_t value) {
    char buf[11]; // max 10 digits + '\0'
    int i = 10;
    buf[i] = '\0';

    if (value == 0) {
        uart_putc('0');
        return;
    }

    while (value > 0) {
        buf[--i] = '0' + (value % 10);
        value /= 10;
    }

    uart_puts(&buf[i]);
}

void uart_put_hex(uint64_t value) {
    uart_puts("0x");

    for (int i = 60; i >= 0; i -= 4) {
        uint8_t digit = (value >> i) & 0xF;
        uart_putc(digit < 10 ? '0' + digit : 'A' + digit - 10);
    }
}

void kernel_main() {
    write32(UART0_CR, 0x0);

    uint32_t uart_ibrd = 23;
    uint32_t uart_fbrd = 56;

    write32(UART0_IBRD, uart_ibrd);
    write32(UART0_FBRD, uart_fbrd);

    write32(UART0_LCRH, (1 << 4) | (0b11 << 5));
    write32(UART0_CR, (1<<0) | (1<<8) | (1<<9) );

    //uart_putc('H');
    uart_puts("Hello, Kernel!\r\n");
    mailbox_test();
    mailbox_call();
    uart_puts("Firmware rev: ");
    uart_put_hex(mbox[5]);
    uart_putc('\r');
    uart_putc('\n');
    
    while (1) {}
}
