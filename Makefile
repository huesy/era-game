BUILD_DIR := bin
OBJ_DIR := obj

# Needs to be set by the caller.
# ASSEMBLY := engine

DEFINES := -DERA_EXPORT
BASE_COMPILER_FLAGS := -Wall -Wextra -Werror -Wstrict-prototypes -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-unused-value

# Detect OS and architecture.
ifeq ($(OS),Windows_NT)
	# WIN32
	BUILD_PLATFORM := windows
	EXTENSION := .dll
	COMPILER_FLAGS := $(BASE_COMPILER_FLAGS) -fdeclspec
	INCLUDE_FLAGS := -I$(ASSEMBLY)\src $(ADDL_INC_FLAGS)
	LINKER_FLAGS := -shared -L$(OBJ_DIR)\$(ASSEMBLY) -L.\$(BUILD_DIR) $(ADDL_LINK_FLAGS)
	DEFINES += -D_CRT_SECURE_NO_WARNINGS -DUNICODE

	# Make does not offer a recursive wildcard function, so we need to define it.
	# This is a Windows-specific implementation.
	rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
	DIR := $(subst /,\,${CURDIR})

	# .c files
	SRC_FILES := $(call rwildcard,$(ASSEMBLY)/,*.c)
	# Directories with .h files
	DIRECTORIES := \$(ASSEMBLY)\src $(subst $(DIR),,$(shell dir $(ASSEMBLY)\src /S /AD /B | findstr /i src))
	OBJ_FILES := $(SRC_FILES:%=$(OBJ_DIR)/%.o)
	ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
		# AMD64
	else
		ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
			# AMD64
		endif
		ifeq ($(PROCESSOR_ARCHITECTURE),x86)
			# IA32
		endif
	endif
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		# LINUX
		BUILD_PLATFORM := linux
		EXTENSION := .so
		# Note: -fPIC is required for shared libraries.
		#       -fvisibility=hidden is used to hide symbols by default and only export the ones we want.
		COMPILER_FLAGS := $(BASE_COMPILER_FLAGS) -fPIC -fvisibility=hidden
		INCLUDE_FLAGS := -I$(ASSEMBLY)/src - $(ADDL_INC_FLAGS)
		# Note: --no-undefined and --no-allow-shlib-undefined ensure that symbols linking against are resolved.
		# These are linux-specific, as the default behaviour is the opposite of this, allowing code to compile
		# here that would not on other platforms from not being exported (i.e. Windows)
		# Discovered the solution here for this: https://github.com/ziglang/zig/issues/8180
		LINKER_FLAGS := -Wl,--no-undefined,--no-allow-shlib-undefined -shared -lxcb -lX11 -lX11-xcb
		LINKER_FLAGS := -shared -L$(OBJ_DIR)/$(ASSEMBLY) -L./$(BUILD_DIR) $(ADDL_LINK_FLAGS)
		DEFINES += -D_GNU_SOURCE
	else
		ifeq ($(UNAME_S),Darwin)
			# OSX
			BUILD_PLATFORM := osx
			EXTENSION := .dylib
			COMPILER_FLAGS := -Wall -Wextra -Werror -fPIC -Wstrict-prototypes -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-unused-value
			INCLUDE_FLAGS := -I$(ASSEMBLY)/src $(ADDL_INC_FLAGS)
			LINKER_FLAGS := -shared -L$(OBJ_DIR)/$(ASSEMBLY) -L./$(BUILD_DIR) $(ADDL_LINK_FLAGS)
			DEFINES += -D_GNU_SOURCE
		endif
	endif
