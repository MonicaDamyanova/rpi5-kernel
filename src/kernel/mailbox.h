#pragma once

#include <stdint.h>

#define MAILBOX_BASE        (MMIO_BASE + 0x13880)

#define MBOX_READ           (MAILBOX_BASE + 0x00)
#define MBOX_STATUS         (MAILBOX_BASE + 0x18)
#define MBOX_WRITE          (MAILBOX_BASE + 0x20)
#define MBOX_FULL           0x80000000
#define MBOX_EMPTY          0x40000000

#define MBOX_TAG_BOARD_MODEL    0x10001
#define MBOX_TAG_BOARD_REVISION 0x10002
#define MBOX_TAG_ARM_MEMORY     0x10005
#define MBOX_TAG_VC_MEMORY      0x10006
#define MBOX_TAG_CLKRATE        0x30002

#define MBOX_TAG_ALLOCATE_BUFFER    0x00040001
#define MBOX_TAG_SET_PHYS_WH        0x00048003
#define MBOX_TAG_GET_PHYS_WH        0x00040003
#define MBOX_TAG_SET_VIRT_WH        0x00048004
#define MBOX_TAG_GET_VIRT_WH        0x00040004
#define MBOX_TAG_SET_DEPTH          0x00048005
#define MBOX_TAG_GET_DEPTH          0x00040005
#define MBOX_TAG_SET_PIXEL_ORDER    0x00048006
#define MBOX_TAG_GET_PITCH          0x00040008
#define MBOX_TAG_SET_VIRT_OFFSET    0x00048009

#define MBOX_CH_PROP        8

#define PROP_MAX_WORDS      64

typedef struct { // Refactor but to smth else
    volatile uint32_t buf[PROP_MAX_WORDS] __attribute__((aligned(16)));
    uint32_t index;  // next free slot
} mailbox_message;

void mailbox_write(uint8_t channel, uint64_t data);
uint32_t mailbox_read(uint8_t channel);

void property_call(mailbox_message * mbox);
void property_init(mailbox_message * mbox); 
uint32_t * property_add_set_tag(mailbox_message * mbox, uint32_t tag_id, uint32_t buffer_size_bytes, uint32_t * values);
uint32_t * property_add_get_tag(mailbox_message * mbox, uint32_t tag_id, uint32_t buffer_size_bytes);
void property_end(mailbox_message * mbox);

