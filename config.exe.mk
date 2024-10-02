# config.exe.mk - Configuration specific to executables

# Platform-specific settings
ifeq ($(BUILD_PLATFORM),windows)
    EXE_EXT := .exe
else
    EXE_EXT :=
endif
