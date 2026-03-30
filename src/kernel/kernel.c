#include <stddef.h>
#include <stdint.h>

#include "mailbox.h"
#include "uart.h"

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

    uart_puts("Hello, Kernel!\r\n");    

    mailbox_message msg;
    property_init(&msg);
    uint32_t *rev = property_add_get_tag(&msg, MBOX_TAG_BOARD_REVISION, 4);
    uint32_t *mem = property_add_get_tag(&msg, MBOX_TAG_ARM_MEMORY, 8);
    property_end(&msg);
    property_call(&msg);

    uint32_t board_revision = rev[0];
    uint32_t mem_size = mem[1];

    uart_puts("Firmware rev: ");
    uart_put_hex(board_revision);
    uart_putc('\r');
    uart_putc('\n');

    uart_puts("Memory Size: ");
    uart_put_hex(mem_size);
    uart_putc('\r');
    uart_putc('\n');

    mailbox_message fb;
    property_init(&fb);
    property_add_set_tag(&fb, MBOX_TAG_SET_PHYS_WH, 8, (uint32_t[]){640, 480});
    property_add_set_tag(&fb, MBOX_TAG_SET_VIRT_WH, 8, (uint32_t[]){640, 480});
    property_add_set_tag(&fb, MBOX_TAG_SET_DEPTH, 4, (uint32_t[]){32});
    uint32_t * fb_data = property_add_set_tag(&fb, MBOX_TAG_ALLOCATE_BUFFER, 8, (uint32_t[]){16, 0});
    uint32_t * pitch_data = property_add_get_tag(&fb, MBOX_TAG_GET_PITCH, 4);
    property_end(&fb);
    property_call(&fb);
    
    uint32_t * fb_ptr = (uint32_t*)((uintptr_t)fb_data[0] & ~0xC0000000);
    uint32_t fb_size = fb_data[1];
    uint32_t pitch = pitch_data[0];

    for (uint32_t y = 0; y < 480; y++) {
        for (uint32_t x = 0; x < 640; x++) {
            fb_ptr[y * (pitch / 4) + x] = 0x009900; // green screen
        }
    }
    
    while (1) {}
}
