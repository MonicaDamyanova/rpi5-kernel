#pragma once

#include <stdint.h>

#define MMIO_BASE 0x107c000000UL

#define read32(addr) (*(volatile uint32_t*)(addr))
#define write32(addr, value) (*(volatile uint32_t*)(addr) = (value))

//const uintptr_t MMIO_BASE = 0x107c000000UL;
