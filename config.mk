# config.mk - Common configuration for all Makefiles

# --------------------------------------------------------------------
# Platform detection
# --------------------------------------------------------------------
ifeq ($(OS),Windows_NT)
    # Windows settings
    BUILD_PLATFORM := windows
    fixpath = $(subst /,\,$1)
    MKDIR_P := if not exist "$(call fixpath,$(1))" mkdir "$(call fixpath,$(1))"
    RM := del /q
    PATH_SEP := \\
    EXE_EXT := .exe
    SHARED_LIB_EXT := .dll
    SHARED_LIB_FLAG := -shared
    PWD := $(shell cd)
else
    # Unix/Linux/MacOS settings
    UNAME_S := $(shell uname)
    ifeq ($(UNAME_S),Darwin)
        BUILD_PLATFORM := macos
        SHARED_LIB_EXT := .dylib
        SHARED_LIB_FLAG := -dynamiclib
    else
        BUILD_PLATFORM := linux
        SHARED_LIB_EXT := .so
        SHARED_LIB_FLAG := -shared
    endif
    fixpath = $1
    MKDIR_P := mkdir -p $(1)
    RM := rm -rf
    PATH_SEP := /
    EXE_EXT :=
    PWD := $(shell pwd)
endif

# --------------------------------------------------------------------
# Build directories
# --------------------------------------------------------------------
BUILD_DIR := $(call fixpath,$(PWD)/build)
BIN_DIR := $(call fixpath,$(BUILD_DIR)/bin)
OBJ_DIR := $(call fixpath,$(BUILD_DIR)/obj)
LIB_DIR := $(call fixpath,$(BUILD_DIR)/lib)

# --------------------------------------------------------------------
# Compiler and flags
# --------------------------------------------------------------------
CC := clang  # Use clang compiler
CFLAGS := -std=c17 -Wall -Wextra -Werror -Wpedantic
LFLAGS :=

# Debug and optimization flags
ifeq ($(DEBUG),1)
    CFLAGS += -DDEBUG -g -O0
else
    CFLAGS += -DNDEBUG -O2
endif

# Dependency flags
DEPFLAGS := -MMD -MP

# --------------------------------------------------------------------
# Utilities
# --------------------------------------------------------------------
ECHO ?= @echo
