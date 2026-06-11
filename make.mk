CC       := gcc
CFLAGS   := -ggdb -Wall -Wextra -Iinclude -MMD -MP
LDFLAGS  := -lm

SRC_DIR  := src
INC_DIR  := include
BUILD_DIR:= build
OBJ_DIR  := $(BUILD_DIR)/obj
BIN_DIR  := $(BUILD_DIR)/bin

TARGET   := $(BIN_DIR)/pcm-generator

SOURCES  := $(shell find $(SRC_DIR) -name "*.c")

OBJECTS  := $(SOURCES:%.c=$(OBJ_DIR)/%.o)
DEPS     := $(OBJECTS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean

-include $(DEPS)
