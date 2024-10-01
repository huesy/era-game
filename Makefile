# Root Makefile

# Platform detection
ifeq ($(OS),Windows_NT)
    RM = del /Q
    MKDIR_P = if not exist $(subst /,\,$(1)) mkdir $(subst /,\,$(1))
    BUILD_DIR = $(shell cd)\build
else
    RM = rm -f
    MKDIR_P = mkdir -p $(1)
    BUILD_DIR = $(shell pwd)/build
endif

.PHONY: all scaffold engine game clean

all: scaffold engine game

scaffold:
	@$(call MKDIR_P,$(BUILD_DIR))

engine: scaffold
	$(MAKE) -C engine BUILD_DIR=$(BUILD_DIR)

game: scaffold engine
	$(MAKE) -C game BUILD_DIR=$(BUILD_DIR)

clean:
	$(MAKE) -C engine clean BUILD_DIR=$(BUILD_DIR)
	$(MAKE) -C game clean BUILD_DIR=$(BUILD_DIR)
	$(RM) $(BUILD_DIR)  # Ensures full clean in root