TOOLCHAIN=aarch64-none-elf-
AS = ${TOOLCHAIN}as
LD = ${TOOLCHAIN}ld
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
CFLAGS = -Wall -O2 -ffreestanding
CC=$(TOOLCHAIN)gcc

all: clean kernel8.img

boot.o: boot.s
	${CC} ${CFLAGS} -c boot.s -o boot.o 

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

kernel8.img: boot.o ${OBJS}
	${LD} boot.o ${OBJS} -T linker.ld -o kernel8.elf
	${TOOLCHAIN}objcopy -O binary kernel8.elf kernel8.img

clean:
	rm -rf *.o *.img *.elf
