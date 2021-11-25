TARGET=pastaos

# Automatically detect sources in sub-folders
build-dirs = $(wildcard */)
SRCS=$(wildcard $(addsuffix *.c, $(build-dirs))) $(wildcard $(addsuffix *.S, $(build-dirs)))
OBJS=$(patsubst %.c, %.o, $(filter %c, $(SRCS))) $(patsubst %.S, %.o, $(filter %S, $(SRCS)))

# Compiler options
CFLAGS=-m32 -O2 -MD -g -Wall -Wextra -fno-builtin -fomit-frame-pointer -fno-stack-protector -Iinclude
LDFLAGS=-g -nostdlib -X -lc

# Build rules
%.o: %.S
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Makefile targets
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
	rm -f $(OBJS) $(TARGET) $(patsubst %.o, %.d, $(OBJS)) *.iso tags

# Properly support header dependencies
-include $(OBJS:.o=.d)
