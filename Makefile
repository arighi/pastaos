TARGET=pastaos
OBJS=boot.o init.o panic.o irq.o interrupt.o console.o clock.o sched.o main.o

CFLAGS=-m32 -O2 -g -Wall -Wextra -fno-builtin -fomit-frame-pointer -fno-stack-protector -I.
LDFLAGS=-g -nostdlib -X -lc

%.o: %.S
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(OBJS)
	$(LD) -m elf_i386 -T linker.ld $(OBJS) -o $(TARGET) -nostdlib

iso: all
	rm -f $(TARGET).iso
	mkdir -p isodir/boot/grub
	cp $(TARGET) isodir/boot/
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(TARGET).iso isodir
	rm -rf isodir

run: iso
	qemu-system-x86_64 -cdrom $(TARGET).iso -boot d

tags:
	ctags -R

clean:
	rm -rf isodir
	rm -f $(OBJS) $(TARGET) *.iso tags
