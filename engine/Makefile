ASSEMBLY := engine
LIB_TYPE ?= static
CC = clang
CFLAGS := -Wall -Wextra -Werror -Wpedantic
LDFLAGS :=

# Directories
SRCDIR = src
INCDIR = include
BUILDDIR = build
OBJDIR = $(BUILDDIR)/obj
DEPDIR = $(BUILDDIR)/dep

# Find all C source files and generate corresponding object files and dependencies
SRC = $(wildcard $(SRCDIR)/**/*.c) $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))
DEPS = $(patsubst $(SRCDIR)/%.c,$(DEPDIR)/%.d,$(SRC))
LOGFILE = $(BUILDDIR)/build_log.txt

# Detect platform
ifeq ($(OS),Windows_NT)
    PLATFORM := windows
    SHELL := cmd.exe
    MKDIR = if not exist "$(subst /,\,$(1))" mkdir "$(subst /,\,$(1))"
    RM := rmdir /S /Q
    ECHO := echo
    ifeq ($(LIB_TYPE),static)
        EXTENSION := .o
        TARGET := $(OBJ)
    else
        EXTENSION := .dll
        SHARED_FLAG := -shared
        TARGET := $(BUILDDIR)/$(ASSEMBLY)$(EXTENSION)
    endif
else
    OS := $(shell uname -s)
    ifeq ($(OS),Darwin)
        PLATFORM := macos
        MKDIR := mkdir -p
        RM := rm -rf
        ECHO := echo
        CFLAGS += -fPIC
        ifeq ($(LIB_TYPE),static)
            EXTENSION := .o
            TARGET := $(OBJ)
        else
            EXTENSION := .dylib
            SHARED_FLAG := -dynamiclib
            TARGET := $(BUILDDIR)/lib$(ASSEMBLY)$(EXTENSION)
        endif
    else ifeq ($(OS),Linux)
        PLATFORM := linux
        MKDIR := mkdir -p
        RM := rm -rf
        ECHO := echo
        CFLAGS += -fPIC
        ifeq ($(LIB_TYPE),static)
            EXTENSION := .o
            TARGET := $(OBJ)
        else
            EXTENSION := .so
            SHARED_FLAG := -shared
            TARGET := $(BUILDDIR)/lib$(ASSEMBLY)$(EXTENSION)
        endif
    else
        $(error Unsupported OS)
    endif
endif

# Ensure subdirectories are created
DIRS = $(sort $(dir $(OBJ)) $(dir $(DEPS)))

all: $(TARGET)

# Create object files from source files and generate dependencies
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@$(ECHO) "Compiling $<..."
	@$(call MKDIR,$(dir $@))
	@$(CC) $(CFLAGS) -I$(INCDIR) -MMD -MP -c $< -o $@

# Build the dynamic library
ifeq ($(LIB_TYPE),dynamic)
$(TARGET): $(OBJ)
	@$(ECHO) "Linking dynamic library $(TARGET)..."
	@$(call MKDIR,$(BUILDDIR))
	@$(CC) $(OBJ) $(SHARED_FLAG) -o $(TARGET)
	@$(ECHO) "Compiled $(TARGET) as a $(LIB_TYPE) library with flags: $(CFLAGS) $(SHARED_FLAG)" > $(LOGFILE)
endif

# Clean method
clean:
	@$(ECHO) "Cleaning $(BUILDDIR)..."
ifeq ($(PLATFORM),windows)
	@if exist $(subst /,\,$(BUILDDIR)) $(RM) $(subst /,\,$(BUILDDIR))
else
	@$(RM) $(BUILDDIR)
endif

# Include the dependency files to ensure proper recompilation on header changes
-include $(DEPS)