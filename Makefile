TARGET=pastaos
OBJS=boot.o console.o main.o

CFLAGS=-m32 -O2 -Wall -Wextra

%.o: %.S
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(OBJS)
	$(LD) -m elf_i386 -T linker.ld $(OBJS) -o $(TARGET) -nostdlib
	rm -f $(TARGET).iso
	mkdir -p isodir/boot/grub
	cp $(TARGET) isodir/boot/
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(TARGET).iso isodir
	rm -rf isodir

run: all
	qemu-system-x86_64 -cdrom $(TARGET).iso

tags:
	ctags -R

clean:
	rm -rf isodir
	rm -f $(OBJS) $(TARGET) *.iso tags
