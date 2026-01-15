#include <stddef.h>
#include <stdint.h>

#define read32(addr) (*(volatile uint32_t*)(addr))
#define write32(addr, value) (*(volatile uint32_t*)(addr) = (value))

const uintptr_t MMIO_BASE = 0x107c000000UL;
const uintptr_t UART0_BASE = MMIO_BASE + 0x1001000;
const uintptr_t MAILBOX_BASE = MMIO_BASE + 0x13880;

#define UART0_DR        (UART0_BASE + 0x00)
#define UART0_RSRECR    (UART0_BASE + 0x04)
#define UART0_FR        (UART0_BASE + 0x18)
#define UART0_ILPR      (UART0_BASE + 0x20)
#define UART0_IBRD      (UART0_BASE + 0x24)
#define UART0_FBRD      (UART0_BASE + 0x28)
#define UART0_LCRH      (UART0_BASE + 0x2C)
#define UART0_CR        (UART0_BASE + 0x30)

#define MBOX_READ       (*(volatile uint32_t*)(MAILBOX_BASE + 0x00))
#define MBOX_STATUS     ((uint32_t*)(MAILBOX_BASE + 0x18))
#define MBOX_WRITE      (*(volatile uint32_t*)(MAILBOX_BASE + 0x20))
#define MBOX_FULL       0x80000000
#define MBOX_EMPTY      0x40000000
#define MBOX_CLKRATE_TAG 0x30002

volatile uint32_t uart_mbox[9] __attribute__((aligned(16))) = {
    36, 0, MBOX_CLKRATE_TAG, 12, 8, 2, 0, 0, 0
};

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

void kernel_main() {
    write32(UART0_CR, 0x0);

    uint32_t uart_ibrd = 23;
    uint32_t uart_fbrd = 56;
    //uint32_t uart_baud = 115200;

    //uint32_t addr = ((uint32_t)(uintptr_t)uart_mbox) & ~0xF;
    //while (read32(MBOX_STATUS) & 0x80000000) { }
    //write32(MBOX_WRITE, addr);
    //while ( (read32(MBOX_STATUS) & 0x40000000) || read32(MBOX_READ) != addr ) { }
    
    //if (mailbox_call(uart_mbox,8)){
    //uint32_t uart_clk = uart_mbox[6];
    //uart_ibrd = uart_clk / (16 * uart_baud);
    //uint32_t rem = uart_clk % (16 * uart_baud);
    //uart_fbrd = (rem * 64 + uart_baud/2) / uart_baud;
    //}

    write32(UART0_IBRD, uart_ibrd);
    write32(UART0_FBRD, uart_fbrd);

    write32(UART0_LCRH, (1 << 4) | (0b11 << 5));
    write32(UART0_CR, (1<<0) | (1<<8) | (1<<9) );

    //uart_putc('H');
    uart_puts("Hello, Kernel!\r\n");
    
    while (1) {}
}
