#include "mailbox.h"

#include <stdint.h>

#include "mmio.h"

void mailbox_write(uint8_t channel, uint64_t data) {
    while (read32(MBOX_STATUS) & MBOX_FULL);
    write32(MBOX_WRITE, (data & ~0xFULL) | (channel & 0xF));
}

uint32_t mailbox_read(uint8_t channel) {
    uint32_t val;

    while (1) {
        while (read32(MBOX_STATUS) & MBOX_EMPTY);

        val = read32(MBOX_READ);
        if ((val & 0xF) == channel)
            return val;
    }
}
