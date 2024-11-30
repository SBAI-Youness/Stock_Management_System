# Compiler and flags
CC = gcc
CFLAGS = -Iinclude
RC = windres

# Directories and source files
SRC := $(wildcard source/*.c)
BUILD = build
RESOURCE = source/resource.rc
RESOURCE_OBJ = $(BUILD)/resource.o

# Target executable
TARGET = $(BUILD)/main.exe

# Default target
all: $(TARGET)

# Create the build directory if it doesn't exist
$(BUILD):
	@if not exist $(BUILD) mkdir $(BUILD)

#Compile resource file with windres
$(RESOURCE_OBJ): $(RESOURCE) | $(BUILD)
	$(RC) $< -o $@

# Build target for app
$(TARGET): $(SRC) $(RESOURCE_OBJ) | $(BUILD)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rmdir /S /Q $(BUILD)

.PHONY: all clean