BUILD_DIR ?= build
LIB_DIR ?= lib
SOURCE_DIR ?= src
INCLUDE_DIR ?= include

SOURCES := $(shell find $(SOURCE_DIR) -name '*.c')
OBJECTS := $(SOURCES:%.c=%.o)
OUTPUTS := $(OBJECTS:src%=build%)

HEADERS := $(shell find $(SOURCE_DIR) -name '*.h')

main: driver $(LIB_DIR)/libsimg.a

driver: driver.c $(LIB_DIR)/libsimg.a
	$(CC) -Wall -Wpedantic -Werror -Wno-newline-eof -Wno-error=parentheses -O3 -I$(SOURCE_DIR) -I$(INCLUDE_DIR) -o $@ $^

$(LIB_DIR)/libsimg.a: $(OUTPUTS)
	ar -cr $@ $^

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c $(HEADERS) | $(BUILD_DIR)
	$(CC) -Wall -Wpedantic -Werror -Wno-newline-eof -Wno-error=parentheses -O3 -I$(SOURCE_DIR) -I$(INCLUDE_DIR) -c -o $@ $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(LIB_DIR)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(LIB_DIR)
	rm -rf driver