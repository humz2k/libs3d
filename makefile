BUILD_DIR ?= build
LIB_DIR ?= lib
SOURCE_DIR ?= src
INCLUDE_DIR ?= include
LIBSIMG_PATH ?= ext/libsimg

SOURCES := $(shell find $(SOURCE_DIR) -name '*.c')
OBJECTS := $(SOURCES:%.c=%.o)
OUTPUTS := $(OBJECTS:src%=build%)

HEADERS := $(shell find $(SOURCE_DIR) -name '*.h')

main: driver $(LIB_DIR)/libs3d.a

driver: driver.c $(LIB_DIR)/libs3d.a
	$(CC) -Wall -Wpedantic -Wno-newline-eof -Wno-error=parentheses -O3 -I$(SOURCE_DIR) -I$(INCLUDE_DIR) -I$(LIBSIMG_PATH)/include -L$(LIBSIMG_PATH)/lib -lsimg -o $@ $^

$(LIB_DIR)/libs3d.a: $(OUTPUTS)
	ar -cr $@ $^

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c $(HEADERS) | $(BUILD_DIR)
	$(CC) -Wall -Wpedantic -Wno-newline-eof -Wno-error=parentheses -O3 -I$(SOURCE_DIR) -I$(INCLUDE_DIR) -I$(LIBSIMG_PATH)/include -c -o $@ $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(LIB_DIR)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(LIB_DIR)
	rm -rf driver