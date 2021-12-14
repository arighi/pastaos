TARGET=pastaos

# Automatically detect sources in sub-folders
build-dirs = $(wildcard */)
SRCS=$(wildcard $(addsuffix *.c, $(build-dirs))) $(wildcard $(addsuffix *.S, $(build-dirs)))
OBJS=$(patsubst %.c, %.o, $(filter %c, $(SRCS))) $(patsubst %.S, %.o, $(filter %S, $(SRCS)))

# Compiler options
CFLAGS=-m32 -O2 -MD -g -Wall -Wextra -fno-builtin -fomit-frame-pointer -fno-stack-protector -Iinclude
LDFLAGS=-g -nostdlib -X -lc

ifeq ("$(origin V)", "command line")
  Q=
else
  Q=@
endif

# Build rules
%.o: %.S
	@echo AS  $<
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.c
	@echo CC  $<
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJS)
	@echo LD  $@
	$(Q)$(LD) -m elf_i386 -T linker.ld $(OBJS) -o $(TARGET) -nostdlib

# Makefile targets
all: $(TARGET)

iso: all
	@echo MKISO  $(TARGET).iso
	$(Q)rm -f $(TARGET).iso
	$(Q)mkdir -p isodir/boot/grub
	$(Q)cp $(TARGET) isodir/boot/
	$(Q)cp grub.cfg isodir/boot/grub/grub.cfg
	$(Q)grub-mkrescue -o $(TARGET).iso isodir
	$(Q)rm -rf isodir

run: iso
	$(Q)qemu-system-x86_64 -cdrom $(TARGET).iso -boot d -curses

tags:
	@echo CTAGS  $@
	$(Q)ctags -R

clean:
	@echo CLEAN
	$(Q)rm -rf isodir
	$(Q)rm -f $(OBJS) $(TARGET) $(patsubst %.o, %.d, $(OBJS)) *.iso tags

# Properly support header dependencies
-include $(OBJS:.o=.d)
