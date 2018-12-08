PREFIX  = h8300-elf-

CC      = $(PREFIX)gcc
STRIP   = $(PREFIX)strip
OBJCOPY = $(PREFIX)objcopy

TARGET = os

BIN_DIR   = ./bin
BUILD_DIR = ./build

CFLAGS  := -Wall -mh -nostdinc -nostdlib -fno-builtin -I. -Os \
		   -Wno-builtin-declaration-mismatch $(CFLAGS)
LDFLAGS := -static -T ld.scr -L$(BUILD_DIR) $(LDFLAGS)

OBJS  = startup.s.o main.c.o lib.c.o serial.c.o
OBJS := $(addprefix $(BUILD_DIR)/, $(OBJS) )

.PHONY: all clean

all: $(BIN_DIR) $(BUILD_DIR) $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(BUILD_DIR)/$(TARGET)
		cp $^ $@

$(BIN_DIR):
		mkdir -p $(BIN_DIR)

$(BUILD_DIR):
		mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/$(TARGET): $(OBJS)
		$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $(BUILD_DIR)/$(TARGET).elf
		cp $(BUILD_DIR)/$(TARGET).elf $@
		$(STRIP) $@

$(BUILD_DIR)/%.o: $(BUILD_DIR) %
		$(CC) -c $(CFLAGS) $^ -o $@

clean:
	test -d $(BIN_DIR)   && rm -f $(BIN_DIR)/*
	test -d $(BUILD_DIR) && rm -f $(BUILD_DIR)/*
