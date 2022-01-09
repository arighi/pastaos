TARGET=pastaos
OBJS=boot.o main.o

CFLAGS=-m32 -Os -g -Wall -Wextra -fno-builtin -fomit-frame-pointer -fno-stack-protector -fno-pie

%.o: %.S
	$(CC) -c -m32 $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJS)
	$(LD) -m elf_i386 -Tlinker.ld -o $@ $(OBJS) -nostdlib

$(TARGET).iso: $(TARGET)
	mkdir -p isodir/boot/grub
	cp grub.cfg isodir/boot/grub/grub.cfg
	cp $(TARGET) isodir/boot/$(TARGET)
	grub-mkrescue -o $(TARGET).iso isodir/
	rm -rf isodir/

all: $(TARGET)

run: $(TARGET).iso
	qemu-system-x86_64 -cdrom pastaos.iso -boot d -curses

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).iso
