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

# OUTPUT
ifeq ($(OUTPUT),)
    OUTPUT := $(ASSEMBLY)
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

# Defines
_DEFINES += -DENGINE_EXPORT

# Directories
SRCDIR = src/$(ASSEMBLY)
BUILDDIR = build/$(PLATFORM)
LIBDIR = $(BUILDDIR)/lib
OBJDIR = $(BUILDDIR)/obj/$(ASSEMBLY)

# Find all C source files and generate corresponding object files and dependencies
SRCFILES = $(wildcard $(SRCDIR)/**/*.c) $(wildcard $(SRCDIR)/*.c)
OBJFILES = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCFILES))
LOGFILE = $(BUILDDIR)/build_log.txt

# Detect platform
ifeq ($(OS),Windows_NT)
    # Windows
    PLATFORM := windows
    EXTENSION := dll
    PREFIX := 
    SHELL := cmd.exe
        
    _DYNAMIC_LIB := $(LIBDIR)/$(PREFIX)$(OUTPUT).$(EXTENSION)

    _CFLAGS +=
    _LDFLAGS += -L$(OBJDIR) -shared

    MKDIR = if not exist "$(subst /,\,$(1))" mkdir "$(subst /,\,$(1))"
    RM = if exist "$(subst /,\,$(1))" rmdir /S /Q "$(subst /,\,$(1))"
    DEL = if exist "$(subst /,\,$(1))" del "$(subst /,\,$(1))"
    ECHO = echo
else
    OS := $(shell uname -s)
    ifeq ($(OS),Linux)
        # Linux
        PLATFORM := linux
        EXTENSION := so
        PREFIX := lib

        _DYNAMIC_LIB := $(LIBDIR)/$(PREFIX)$(OUTPUT).$(EXTENSION)

        _CFLAGS += -fPIC
        _LDFLAGS += -L./$(OBJDIR) -shared

        MKDIR = mkdir -p "$(1)"
        RM = rm -rf "$(1)"
        DEL = rm -f "$(1)"
        ECHO = echo
    endif
    ifeq ($(OS),Darwin)
        # MacOS
        PLATFORM := macos
        EXTENSION := dylib
        PREFIX := lib

        _DYNAMIC_LIB := $(LIBDIR)/$(PREFIX)$(OUTPUT).$(EXTENSION)

        _CFLAGS += -fPIC
        _LDFLAGS += -L./$(OBJDIR) -shared -dynamiclib

        MKDIR = mkdir -p "$(1)"
        RM = rm -rf "$(1)"
        DEL = rm -rf "$(1)"
        ECHO = echo
    endif
endif

all: scaffold compile link

.NOTPARALLEL: scaffold

.PHONY: scaffold
scaffold:
	@$(call MKDIR,$(LIBDIR))
	@$(call MKDIR,$(OBJDIR))

.PHONY: link
link: scaffold $(OBJFILES)
	@$(ECHO) Linking '$(_DYNAMIC_LIB)'...
	@$(CC) $(OBJFILES) $(_LDFLAGS) -o $(_DYNAMIC_LIB)
	@$(ECHO) Compiled '$(_DYNAMIC_LIB)' as a '$(PLATFORM)' library with flags: $(_CFLAGS) $(_LDFLAGS) > $(LOGFILE)

.PHONY: compile
compile:
	@$(ECHO) --- Performing '$(ASSEMBLY)' '$(_DYNAMIC_LIB)' build ---
-include $(OBJFILES:.o=.d)

.PHONY: clean
clean:
	@$(ECHO) --- Cleaning '$(ASSEMBLY)' ---
	@$(call DEL,$(_DYNAMIC_LIB))
ifeq ($(PLATFORM),windows)
	@$(call DEL,$(LIBDIR)/$(PREFIX)$(OUTPUT).*)
endif
	@$(call RM,$(OBJDIR))

# Create object files from source files and generate dependencies
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@$(ECHO) Compiling '$<'...
	@$(call MKDIR,$(dir $@))
	@$(CC) $< $(_CFLAGS) -c -o $@ $(_DEFINES) $(_INCLUDES)

# Compile .m to .o objects only for macos
# ifeq ($(PLATFORM),macos)
# $(OBJDIR)/%.o: $(SRCDIR)/%.m
# 	@$(ECHO) Compiling '$<'...
# 	@$(call MKDIR,$(dir $@))
# 	@$(CC) $< $(_CFLAGS) -c -o $@ $(_DEFINES) $(_INCLUDES)
# endif

# Generate dependencies
-include $(OBJFILES:.o=.d)
