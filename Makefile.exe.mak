# Notes:
# 'ASSEMBLY' must be set when calling this Makefile
# 'TARGET'   must be set when calling this Makefile
# 'STATIC'   may be set to '1' when calling this Makefile to build a static library.
# 'DYNAMIC'  may be set to '1' when calling this Makefile to build a dynamic library.
# 'CFLAGS'   may be set when calling this Makefile to append additional compiler flags.
# 'LDFLAGS'  may be set when calling this Makefile to append additional linker flags.
# 'INCLUDES' may be set when calling this Makefile to append additional include directories.
# 'DEFINES'  may be set when calling this Makefile to append additional preprocessor definitions.

CC = clang

# User defined CFLAGS
ifeq ($(CFLAGS),)
    _CFLAGS :=
else
    _CFLAGS := $(CFLAGS)
endif

# User defined LDFLAGS
ifeq ($(LDFLAGS),)
    _LDFLAGS :=
else
    _LDFLAGS := $(LDFLAGS)
endif

# User defined INCLUDES
ifeq ($(INCLUDES),)
    _INCLUDES :=
else
    _INCLUDES := $(INCLUDES)
endif

# User defined DEFINES
ifeq ($(DEFINES),)
    _DEFINES :=
else
    _DEFINES := $(DEFINES)
endif

# Compiler Flags
_CFLAGS += -Wall -Wextra -Werror -fdeclspec
ifeq ($(TARGET),release)
    # release
    _CFLAGS += -MD -O2
    _DEFINES += -D_RELEASE
else
    # debug
    _CFLAGS += -g -MD -O0
    _LDFLAGS += -g
    _DEFINES += -D_DEBUG
endif

# Include directories
_INCLUDES += -Iinclude

# Directories
SRCDIR = src/$(ASSEMBLY)
BUILDDIR = build/$(PLATFORM)
BINDIR = $(BUILDDIR)/bin
OBJDIR = $(BUILDDIR)/obj/$(ASSEMBLY)
DEPDIR = $(BUILDDIR)/dep

# Logging
LOGFILE = $(BUILDDIR)/build_log.txt

# Detect platform
ifeq ($(OS),Windows_NT)
    # Windows
    PLATFORM := windows
    EXTENSION := .exe
    PREFIX := 
    SHELL := cmd.exe

    _CFLAGS += -Wstrict-prototypes
    _LDFLAGS += -L$(OBJDIR) -L$(BINDIR)

    MKDIR = if not exist "$(subst /,\,$(1))" mkdir "$(subst /,\,$(1))"
    RM = if exist "$(subst /,\,$(1))" rmdir /S /Q "$(subst /,\,$(1))"
    DEL = if exist "$(subst /,\,$(1))" del "$(subst /,\,$(1))"
    ECHO = echo

    # Find all C source files and generate corresponding object files and dependencies
    SRCFILES = $(wildcard $(SRCDIR)/**/*.c) $(wildcard $(SRCDIR)/*.c)
    OBJFILES = $(patsubst $(SRCDIR)/%,$(OBJDIR)/%.o,$(SRCFILES))
else
    OS := $(shell uname -s)
    ifeq ($(OS),Linux)
        # Linux
        PLATFORM := linux
        EXTENSION :=
        PREFIX :=

        _CFLAGS += -fPIC
        _LDFLAGS += -L$(OBJDIR) -L$(BINDIR) -Wl,-rpath,.

        MKDIR = mkdir -p "$(1)"
        RM = rm -rf "$(1)"
        DEL = rm -f "$(1)"
        ECHO = echo

        # Find all C source files and generate corresponding object files and dependencies
        SRCFILES = $(shell find $(SRCDIR) -type f -name '*.c')
        OBJFILES = $(patsubst $(SRCDIR)/%,$(OBJDIR)/%.o,$(SRCFILES))
    endif
    ifeq ($(OS),Darwin)
        # MacOS
        PLATFORM := macos
        EXTENSION :=
        PREFIX :=

        _CFLAGS += -fPIC
        _LDFLAGS += -L$(OBJDIR) -L$(BUILDDIR) -rpath @executable_path/../bin -framework Cocoa

        MKDIR = mkdir -p "$(1)"
        RM = rm -rf "$(1)"
        DEL = rm -f "$(1)"
        ECHO = echo

        # Find all C source files and generate corresponding object files and dependencies
        SRCFILES = $(shell find $(SRCDIR) -type f \( -name '*.c' -or -name '*.m' \))
        OBJFILES = $(patsubst $(SRCDIR)/%,$(OBJDIR)/%.o,$(SRCFILES))
    endif
endif

# Define OUTPUT
OUTPUT := $(BINDIR)/$(PREFIX)$(ASSEMBLY)$(EXTENSION)

all: scaffold compile link

.NOTPARALLEL: scaffold

.PHONY: scaffold
scaffold:
	@$(call MKDIR,$(BINDIR))
	@$(call MKDIR,$(OBJDIR))

.PHONY: link
link: scaffold $(OBJFILES)
	@$(ECHO) Linking '$(OUTPUT)'...
	@$(CC) $(OBJFILES) $(_LDFLAGS) -o $(OUTPUT)
	@$(ECHO) Compiled '$(OUTPUT)' as a '$(PLATFORM)' library with flags: $(_CFLAGS) $(_LDFLAGS) > $(LOGFILE)

.PHONY: compile
compile:
	@$(ECHO) --- Performing '$(ASSEMBLY)' '$(OUTPUT)' build ---
-include $(OBJFILES:.o=.d)

.PHONY: clean
clean:
	@$(ECHO) --- Cleaning '$(ASSEMBLY)' ---
	@$(call DEL,$(OUTPUT))
	@$(call DEL,$(BINDIR)/$(ASSEMBLY).*)
	@$(call RM,$(OBJDIR))

# Create object files from source files and generate dependencies
$(OBJDIR)/%.c.o: $(SRCDIR)/%.c
	@$(ECHO) Compiling '$<'...
	@$(call MKDIR,$(dir $@))
	@$(CC) $< $(_CFLAGS) -c -o $@ $(_DEFINES) $(_INCLUDES)

# Compile .m to .m.o objects only for macos
ifeq ($(PLATFORM),macos)
$(OBJDIR)/%.m.o: $(SRCDIR)/%.m
	@$(ECHO) Compiling '$<'...
	@$(call MKDIR,$(dir $@))
	@$(CC) $< $(_CFLAGS) -c -o $@ $(_DEFINES) $(_INCLUDES)
endif

# Generate dependencies
-include $(OBJFILES:.o=.d)
