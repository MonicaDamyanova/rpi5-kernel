# Raspberry Pi 5 Kernel

All this is at the moment is some bare metal programming. The goal is to make
a very basic kernel.

# Setup
Using any UART debugger, power on a Raspberry Pi 5 with the kernel8.img file
as compiled using a cross compiler.

How to build a [cross compiler] (https://wiki.osdev.org/GCC_Cross-Compiler).

There are only a total of 4 files needed for boot:
- kernel8.img
- start4.elf
- config.txt
- fixup4.dat
And it is optional to add bcm2712-rpi-5-b.dtb, but I will be using it
eventually.

All these files can be found online or by installing any OS for the Raspberry
Pi using the Raspberry Pi Imager.
