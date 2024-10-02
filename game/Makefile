ASSEMBLY := game
# Default to static, can be changed to 'dynamic'
LIB_TYPE ?= static

CC = @clang
CFLAGS := -Wall -Wextra -Werror -Wpedantic
LDFLAGS :=

# Directories
SRCDIR = src
INCDIR = include
BUILDDIR = build
OBJDIR = $(BUILDDIR)/obj
DEPDIR = $(BUILDDIR)/dep

# Paths to the engine object files and include directory
ENGINE_OBJDIR = ../engine/build/obj
ENGINE_INCDIR = ../engine/include
ENGINE_OBJ = $(wildcard $(ENGINE_OBJDIR)/**/*.o) $(wildcard $(ENGINE_OBJDIR)/*.o)

# Paths to the engine dynamic library
ENGINE_LIBDIR = ../engine/build
# For Linux (example)
ENGINE_LIB := $(ENGINE_LIBDIR)/libengine.so
ifeq ($(OS), Windows_NT)
    ENGINE_LIB := $(ENGINE_LIBDIR)/engine.dll
else ifeq ($(OS), Darwin)
    ENGINE_LIB := $(ENGINE_LIBDIR)/libengine.dylib
endif

# Find all C source files and generate corresponding object files and dependencies
SRC = $(wildcard $(SRCDIR)/**/*.c) $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))
DEPS = $(patsubst $(SRCDIR)/%.c,$(DEPDIR)/%.d,$(SRC))

LOGFILE = $(BUILDDIR)/build_log.txt

# Detect platform
ifeq ($(OS), Windows_NT)
    PLATFORM := windows
    SHELL := cmd.exe
    MKDIR = if not exist "$(subst /,\,$(1))" mkdir "$(subst /,\,$(1))"
    RM := rmdir /S /Q
    EXTENSION := .exe
    TARGET := $(BUILDDIR)/$(ASSEMBLY)$(EXTENSION)
    ECHO := echo
else
    OS := $(shell uname -s)
    ifeq ($(OS), Darwin)
        PLATFORM := macos
        MKDIR := mkdir -p
        RM := rm -rf
        ECHO := echo
        EXTENSION :=
        TARGET := $(BUILDDIR)/$(ASSEMBLY)$(EXTENSION)
    else ifeq ($(OS), Linux)
        PLATFORM := linux
        MKDIR := mkdir -p
        RM := rm -rf
        ECHO := echo
        EXTENSION :=
        TARGET := $(BUILDDIR)/$(ASSEMBLY)$(EXTENSION)
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
ifeq ($(PLATFORM),windows)
	@$(call MKDIR,$(dir $@))
else
	@$(MKDIR) $(dir $@)
endif
	@$(CC) $(CFLAGS) -I$(INCDIR) -I$(ENGINE_INCDIR) -MMD -MP -c $< -o $@

# Link the game executable with engine object files (static) or engine library (dynamic)
$(TARGET): $(OBJ)
	@$(ECHO) "Linking $(LIB_TYPE) game executable $(TARGET)..."
ifeq ($(PLATFORM),windows)
	@$(call MKDIR,$(BUILDDIR))
else
	@$(MKDIR) $(BUILDDIR)
endif
ifeq ($(LIB_TYPE),static)
	@$(CC) $(OBJ) $(ENGINE_OBJ) -o $(TARGET)
else
	@$(CC) $(OBJ) -L$(ENGINE_LIBDIR) -lengine $(LDFLAGS) -o $(TARGET)
endif
	@$(ECHO) "Compiled $(TARGET) with flags: $(CFLAGS) $(LDFLAGS)" > $(LOGFILE)

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
