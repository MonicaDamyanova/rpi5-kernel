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

void property_call(mailbox_message * mbox) {
    mailbox_write(MBOX_CH_PROP, (uint64_t)mbox->buf);
    mailbox_read(MBOX_CH_PROP);
}

void property_init(mailbox_message * mbox) {
    mbox->index = 2;

    mbox->buf[0] = 0;
    mbox->buf[1] = 0;
}

uint32_t * property_add_set_tag(mailbox_message * mbox, uint32_t tag_id, uint32_t buffer_size_bytes, uint32_t * values) {
    
    mbox->buf[mbox->index++] = tag_id;
    mbox->buf[mbox->index++] = buffer_size_bytes;
    mbox->buf[mbox->index++] = 0; // tag status

    // Reserve space for the buffer size
    uint32_t words = (buffer_size_bytes + 3) / 4; // Adding 3 to round word count up
    
    // Save the pointer to the response.
    uint32_t * response_ptr = (uint32_t*)&mbox->buf[mbox->index];

    for (uint32_t i = 0; i < words; i++) {
        mbox->buf[mbox->index++] = values[i];
    }

    return response_ptr;
}

uint32_t * property_add_get_tag(mailbox_message * mbox, uint32_t tag_id, uint32_t buffer_size_bytes) {
    
    mbox->buf[mbox->index++] = tag_id;
    mbox->buf[mbox->index++] = buffer_size_bytes;
    mbox->buf[mbox->index++] = 0; // tag status

    // Reserve space for the buffer size
    uint32_t words = (buffer_size_bytes + 3) / 4; // Adding 3 to round word count up
    
    // Save the pointer to the response.
    uint32_t * response_ptr = (uint32_t*)&mbox->buf[mbox->index];

    for (uint32_t i = 0; i < words; i++) {
        mbox->buf[mbox->index++] = 0;
    }

    return response_ptr;
}

void property_end(mailbox_message * mbox) {
    mbox->buf[mbox->index++] = 0;  // end tag
    mbox->buf[0] = mbox->index * 4;  // total size in bytes
}
