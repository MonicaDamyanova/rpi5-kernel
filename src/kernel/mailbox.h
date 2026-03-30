#pragma once

#include <stdint.h>

#define MAILBOX_BASE (MMIO_BASE + 0x13880)
//const uintptr_t MAILBOX_BASE = MMIO_BASE + 0x13880;

#define MBOX_READ           (MAILBOX_BASE + 0x00)
#define MBOX_STATUS         (MAILBOX_BASE + 0x18)
#define MBOX_WRITE          (MAILBOX_BASE + 0x20)
#define MBOX_FULL           0x80000000
#define MBOX_EMPTY          0x40000000
#define MBOX_CLKRATE_TAG    0x30002

#define MBOX_CH_PROP        8

void mailbox_write(uint8_t channel, uint64_t data);
uint32_t mailbox_read(uint8_t channel);

