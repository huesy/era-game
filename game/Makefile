# Game Makefile

# Include common configuration
include ../config.mk

# --------------------------------------------------------------------
# Target executable
# --------------------------------------------------------------------
TARGET = game$(EXE_EXT)
BIN_TARGET = $(BIN_DIR)/$(TARGET)

# --------------------------------------------------------------------
# Directories
# --------------------------------------------------------------------
SRC_DIR = src
INCLUDE_DIR = include
SRC_DIRS = $(SRC_DIR)
INCLUDE_DIRS = $(INCLUDE_DIR) ../engine/include

# --------------------------------------------------------------------
# Sources and objects
# --------------------------------------------------------------------
SOURCES = $(wildcard $(SRC_DIR)/**/*.c) $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/game/%.o,$(SOURCES))

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
	$(ECHO) "Game executable built at $(BIN_TARGET)"

# --------------------------------------------------------------------
# Build executable
# --------------------------------------------------------------------
$(BIN_TARGET): $(OBJECTS) $(LIB_DIR)/libengine$(SHARED_LIB_EXT)
	$(ECHO) "Linking game executable..."
	@$(call MKDIR_P,$(@D))
	@$(CC) $(CFLAGS) $(IFLAGS) $^ -o $@ $(LFLAGS)

# --------------------------------------------------------------------
# Compile source files into object files
# --------------------------------------------------------------------
$(OBJ_DIR)/game/%.o: $(SRC_DIR)/%.c
	$(ECHO) "Compiling $<..."
	@$(call MKDIR_P,$(@D))
	@$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

# --------------------------------------------------------------------
# Clean target
# --------------------------------------------------------------------
clean:
	$(ECHO) "Cleaning game build artifacts..."
	@$(RM) $(OBJ_DIR)/game
	@$(RM) $(BIN_TARGET)
