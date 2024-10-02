# Game Makefile

# Include common configuration
include ../config.mk
include ../config.exe.mk

# --------------------------------------------------------------------
# Target executable
# --------------------------------------------------------------------
TARGET := game$(EXE_EXT)
BIN_TARGET := $(call fixpath,$(BIN_DIR)/$(TARGET))

# --------------------------------------------------------------------
# Directories
# --------------------------------------------------------------------
SRC_DIR := src
INCLUDE_DIR := include
SRC_DIRS := $(call fixpath,$(SRC_DIR))
INCLUDE_DIRS := $(call fixpath,$(INCLUDE_DIR)) ../engine/include

# --------------------------------------------------------------------
# Sources and objects
# --------------------------------------------------------------------
SOURCES := $(wildcard $(SRC_DIR)/**/*.c) $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(call fixpath,$(OBJ_DIR)/game/%.o),$(SOURCES))
DEPS := $(OBJECTS:.o=.d)

# --------------------------------------------------------------------
# Compiler flags
# --------------------------------------------------------------------
IFLAGS += $(addprefix -I,$(INCLUDE_DIRS))
LFLAGS += -L$(LIB_DIR) -lengine

# --------------------------------------------------------------------
# Phony targets
# --------------------------------------------------------------------
.PHONY: all clean

# --------------------------------------------------------------------
# Default target
# --------------------------------------------------------------------
all: $(BIN_TARGET)
	@echo Game executable built at $(BIN_TARGET)

# --------------------------------------------------------------------
# Build executable
# --------------------------------------------------------------------
$(BIN_TARGET): $(OBJECTS) $(call fixpath,$(LIB_DIR)/libengine$(SHARED_LIB_EXT))
	@echo --- Linking game executable... ---
	@$(call MKDIR_P,$(@D))
	@$(CC) $(OBJECTS) -o $@ $(LFLAGS)

# --------------------------------------------------------------------
# Compile source files into object files and generate dependencies
# --------------------------------------------------------------------
$(call fixpath,$(OBJ_DIR)/game/%.o): $(call fixpath,$(SRC_DIR)/%.c)
	@echo --- Compiling $<... ---
	@$(call MKDIR_P,$(@D))
	@$(CC) $(CFLAGS) $(IFLAGS) $(DEPFLAGS) -c $< -o $@

-include $(DEPS)

# --------------------------------------------------------------------
# Clean target
# --------------------------------------------------------------------
clean:
	@echo --- Cleaning game ---
	@$(RM) -r $(call fixpath,$(OBJ_DIR)/game) $(BIN_TARGET)
