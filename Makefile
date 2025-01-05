# Compiler and flags
CC = gcc
CFLAGS = -Iinclude -Iinclude/openssl -Wall -Werror -Wextra -Wpedantic
LDFLAGS = -Llibraries -lssl -lcrypto
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
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Run the program after building
run: $(TARGET)
	@$(TARGET)

clean:
	rmdir /S /Q $(BUILD)

.PHONY: all clean run