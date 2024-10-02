# Engine Makefile

# Include common configuration
include ../config.mk
include ../config.lib.mk

# --------------------------------------------------------------------
# Directories
# --------------------------------------------------------------------
SRC_DIR := src
INCLUDE_DIR := include
SRC_DIRS := $(call fixpath,$(SRC_DIR))
INCLUDE_DIRS := $(call fixpath,$(INCLUDE_DIR))

# --------------------------------------------------------------------
# Target shared library
# --------------------------------------------------------------------
LIB_NAME := libengine$(LIB_EXT)
LIB_TARGET := $(call fixpath,$(LIB_DIR)/$(LIB_NAME))

# --------------------------------------------------------------------
# Sources and objects
# --------------------------------------------------------------------
SOURCES := $(wildcard $(SRC_DIR)/**/*.c) $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/engine/%.o,$(SOURCES))
DEPS := $(OBJECTS:.o=.d)

# --------------------------------------------------------------------
# Compiler flags
# --------------------------------------------------------------------
IFLAGS += $(addprefix -I,$(INCLUDE_DIRS))

# --------------------------------------------------------------------
# Phony targets
# --------------------------------------------------------------------
.PHONY: all clean

# --------------------------------------------------------------------
# Default target
# --------------------------------------------------------------------
all: $(LIB_TARGET)
	@echo Engine shared library built at $(LIB_TARGET)

# --------------------------------------------------------------------
# Build the shared library
# --------------------------------------------------------------------
$(LIB_TARGET): $(OBJECTS)
	@echo  - Creating engine shared library...
	@$(call MKDIR_P,$(@D))
	@$(CC) $(OBJECTS) -o $@ $(LFLAGS)

# --------------------------------------------------------------------
# Compile source files into object files and generate dependencies
# --------------------------------------------------------------------
$(call fixpath,$(OBJ_DIR)/engine/%.o): $(call fixpath,$(SRC_DIR)/%.c)
	@echo  - Compiling $<...
	@$(call MKDIR_P,$(@D))
	@$(CC) $(CFLAGS) $(IFLAGS) $(DEPFLAGS) -c $< -o $@

-include $(DEPS)

# --------------------------------------------------------------------
# Clean target
# --------------------------------------------------------------------
clean:
	@echo --- Cleaning engine... ---
	@$(RM) -r $(call fixpath,$(OBJ_DIR)/engine) $(LIB_TARGET)
