ifeq ($(origin CC),default)
	CC = gcc
endif

CFLAGS ?= -Wall -Wpedantic -g -std=c99 -pedantic
LDFLAGS ?= -lraylib -lm

BINARY ?= minesweeper
ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
RESOURCES_DIR:=$(ROOT_DIR)/resources
BUILD_DIR ?= build
SOURCE_DIR = src

SOURCES = $(shell find $(SOURCE_DIR) -name '*.c')
OBJECTS := $(SOURCES:%.c=$(BUILD_DIR)/%.o)

INCLUDE_DIRS = $(shell find $(SOURCE_DIR) -type d)
INCLUDE_FLAGS = $(addprefix -I,$(INCLUDE_DIRS))
DEPS := $(OBJECTS:%.o=%.d)

# Path to resources via MACRO
CUSTOM_CFLAGS = -DRESOURCES_DIR=\"$(RESOURCES_DIR)\"

override CFLAGS += $(INCLUDE_FLAGS) $(CUSTOM_CFLAGS)

# If raylib path was passed
ifdef RAYLIB_PATH
	LDFLAGS += -L$(RAYLIB_PATH)
endif

.PHONY: all
all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJECTS): $(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(DEPS): $(BUILD_DIR)/%.d: %.c
	@mkdir -p $(@D)
	$(CC) -E $(CFLAGS) $< -MM -MT $(@:.d=.o) > $@


.PHONY: clean
clean:
	rm -rf $(OBJECTS) $(DEPS) $(BINARY)

.PHONY: run
run: all
	@./$(BINARY)

.PHONY: release
release: CFLAGS := -O2 $(INCLUDE_FLAGS)
release: all

NODEPS = clean

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
include $(DEPS)
endif
