# Engine Makefile

# Platform detection
ifeq ($(OS),Windows_NT)
	RM = del /Q
	MKDIR_P = if not exist $(subst /,\,$(1)) mkdir $(subst /,\,$(1))
else
	RM = rm -f
	MKDIR_P = mkdir -p $(1)
endif

# Directories
SRC_DIR = src
INCLUDE_DIR = include

# Compiler and flags
CC = clang
CFLAGS = -v -std=c11 -Wall -Wextra -Werror -Wpedantic
IFLAGS = -I$(INCLUDE_DIR)
LFLAGS =

# Sources and objects
SOURCES = $(wildcard $(SRC_DIR)/**/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/engine/%.o,$(SOURCES))
DEPS = $(OBJECTS:.o=.d)

# Targets
all: $(OBJECTS)

# Create build directory and compile object files
$(BUILD_DIR)/engine/%.o: $(SRC_DIR)/%.c
	@$(call MKDIR_P,$(@D))
	$(CC) $(CFLAGS) $(IFLAGS) -MMD -MP -c $< -o $@

# Include dependency files
-include $(DEPS)

clean:
	$(RM) $(BUILD_DIR)/engine

.PHONY: all clean