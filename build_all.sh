#!/bin/bash

# Build script for cleaning and/or building everything

PLATFORM="$1"
ACTION="$2"
TARGET="$3"

if [ "$ACTION" == "build" ]; then
    ACTION="all"
    ACTION_STR="Building"
    ACTION_STR_PAST="built"
elif [ "$ACTION" == "clean" ]; then
    ACTION="clean"
    ACTION_STR="Cleaning"
    ACTION_STR_PAST="cleaned"
else
    echo "Invalid action: $ACTION"
    exit 1
fi

if [ "$PLATFORM" == "windows" ]; then
    ENGINE_LINK="-luser32"
elif [ "$PLATFORM" == "linux" ] || [ "$PLATFORM" == "macos" ]; then
    ENGINE_LINK=""
else
    echo "Invalid platform: $PLATFORM"
    exit 1
fi

echo "$ACTION_STR everything on $PLATFORM ($TARGET)..."

# --------------------------------------------------------------------------
# Libraries
# --------------------------------------------------------------------------

# Engine core library
make -j -f Makefile.lib.mak $ACTION TARGET=$TARGET ASSEMBLY=engine LDFLAGS="$ENGINE_LINK"
if [ $? -ne 0 ]; then
    echo "Error: $?"
    exit $?
fi

# --------------------------------------------------------------------------
# Executables
# --------------------------------------------------------------------------

# Game
make -j -f Makefile.exe.mak $ACTION TARGET=$TARGET ASSEMBLY=game
if [ $? -ne 0 ]; then
    echo "Error: $?"
    exit $?
fi

echo "All assemblies $ACTION_STR_PAST successfully."