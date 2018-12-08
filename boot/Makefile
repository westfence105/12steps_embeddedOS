PREFIX  = h8300-elf-

CC      = $(PREFIX)gcc
STRIP   = $(PREFIX)strip
OBJCOPY = $(PREFIX)objcopy
H8WRITE = h8write

TARGET = bootloader

H8WRITE_SERDEV = com5

BUILD_DIR = ./build

CFLAGS  := -Wall -mh -nostdinc -nostdlib -fno-builtin -I. -Os \
		   -Wno-builtin-declaration-mismatch $(CFLAGS)
LDFLAGS := -static -T ld.scr -L$(BUILD_DIR) $(LDFLAGS)

OBJS  = vector.c.o startup.s.o main.c.o lib.c.o serial.c.o xmodem.c.o elf.c.o
OBJS := $(addprefix $(BUILD_DIR)/, $(OBJS) )

.PHONY: all image write clean

all: $(BUILD_DIR) image

$(BUILD_DIR):
		mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/$(TARGET): $(OBJS)
		$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $(BUILD_DIR)/$(TARGET).elf
		cp $(BUILD_DIR)/$(TARGET).elf $@
		$(STRIP) $@

$(BUILD_DIR)/%.o: %
		$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_DIR)/$(TARGET).mot: $(BUILD_DIR)/$(TARGET)
		$(OBJCOPY) -O srec $^ $@

image: $(BUILD_DIR)/$(TARGET).mot

write: $(BUILD_DIR)/$(TARGET).mot
	$(H8WRITE) -3069 -f20 $^ $(H8WRITE_SERDEV)

clean:
	test -d $(BUILD_DIR) && rm -f $(BUILD_DIR)/*
