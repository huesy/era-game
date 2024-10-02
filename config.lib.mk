# config.lib.mk - Configuration specific to libraries

# Shared compiler and linker flags for libraries
CFLAGS += -fPIC
LFLAGS += $(SHARED_LIB_FLAG)

# Platform-specific settings
ifeq ($(BUILD_PLATFORM),windows)
    LIB_EXT := $(SHARED_LIB_EXT)
else
    LIB_EXT := $(SHARED_LIB_EXT)
endif
