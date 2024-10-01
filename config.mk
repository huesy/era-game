# config.mk - Common configuration for all Makefiles

# --------------------------------------------------------------------
# Platform detection
# --------------------------------------------------------------------
ifeq ($(OS),Windows_NT)
    # Windows settings
    RM = cmd /C del /Q /F
    MKDIR_P = mkdir $(subst /,\,$(1))
    PATH_SEP = \\
    EXE_EXT = .exe
    SHARED_LIB_EXT = .dll
    SHARED_LIB_FLAG = -shared
    ECHO = @echo
else
    # Unix/Linux/MacOS settings
    RM = rm -rf
    MKDIR_P = mkdir -p $(1)
    PATH_SEP = /
    EXE_EXT =
    ECHO = @echo
    UNAME_S := $(shell uname)
    ifeq ($(UNAME_S),Darwin)
        # macOS specific settings
        SHARED_LIB_EXT = .dylib
        SHARED_LIB_FLAG = -dynamiclib
    else
        # Linux/Unix settings
        SHARED_LIB_EXT = .so
        SHARED_LIB_FLAG = -shared
    endif
endif

# --------------------------------------------------------------------
# Compiler and flags
# --------------------------------------------------------------------
CC ?= clang  # Use clang compiler
CFLAGS ?= -std=c11 -Wall -Wextra -Werror -Wpedantic
IFLAGS ?=
LFLAGS ?=

# Debug and optimization flags
ifeq ($(DEBUG),1)
    CFLAGS += -DDEBUG -g -O0
else
    CFLAGS += -DNDEBUG -O2
endif

# --------------------------------------------------------------------
# Build directories
# --------------------------------------------------------------------
BUILD_DIR ?= $(abspath build)
BIN_DIR ?= $(BUILD_DIR)/bin
OBJ_DIR ?= $(BUILD_DIR)/obj
LIB_DIR ?= $(BUILD_DIR)/lib

# --------------------------------------------------------------------
# Include and source directories
# --------------------------------------------------------------------
INCLUDE_DIRS ?=
SRC_DIRS ?=

# --------------------------------------------------------------------
# Utilities
# --------------------------------------------------------------------
ECHO ?= @echo
