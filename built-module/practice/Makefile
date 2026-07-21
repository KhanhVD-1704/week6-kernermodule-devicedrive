KDIR ?= /lib/modules/$(shell uname -r)/build
SRC_DIR := $(CURDIR)/src
TEST_DIR := $(CURDIR)/test
BUILD_DIR := $(CURDIR)/build
MODULE_NAME := char_device_module
TEST_NAME := char_buffer_ctl
TEST_SOURCE := $(TEST_DIR)/$(TEST_NAME).c
TEST_BINARY := $(BUILD_DIR)/$(TEST_NAME)
CC ?= gcc
CFLAGS ?= -Wall -Wextra -I$(CURDIR)/include

.PHONY: all module test load unload check clean

all: module test

module:
	mkdir -p $(BUILD_DIR)
	ln -sfn $(SRC_DIR)/$(MODULE_NAME).c $(BUILD_DIR)/$(MODULE_NAME).c
	ln -sfn $(SRC_DIR)/Kbuild $(BUILD_DIR)/Makefile
	$(MAKE) -C $(KDIR) M=$(BUILD_DIR) modules

test: $(TEST_BINARY)

$(TEST_BINARY): $(TEST_SOURCE)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $<

load: all
	@if lsmod | grep -q '^$(MODULE_NAME) '; then sudo rmmod $(MODULE_NAME); fi
	sudo $(RM) /dev/char_buffer
	sudo insmod $(BUILD_DIR)/$(MODULE_NAME).ko
	@if command -v udevadm >/dev/null; then sudo udevadm settle; fi

unload:
	@if lsmod | grep -q '^$(MODULE_NAME) '; then sudo rmmod $(MODULE_NAME); fi

check: load
	sudo $(TEST_BINARY) write "hello kernel"
	sudo $(TEST_BINARY) read
	sudo $(TEST_BINARY) size
	sudo $(TEST_BINARY) clear
	sudo $(TEST_BINARY) size
	sudo $(TEST_BINARY) read

clean:
	$(RM) -r $(BUILD_DIR)
