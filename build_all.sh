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
    PLATFORM_DIR="windows"
    LDFLAGS="-luser32 -lOpenGL32"
elif [ "$PLATFORM" == "linux" ]; then
    PLATFORM_DIR="linux"
    LDFLAGS=""
elif [ "$PLATFORM" == "macos" ]; then
    PLATFORM_DIR="macos"
    LDFLAGS="-framework OpenGL"
else
    echo "Invalid platform: $PLATFORM"
    exit 1
fi

echo "$ACTION_STR everything on $PLATFORM ($TARGET)..."

# --------------------------------------------------------------------------
# Copy over libraries
# --------------------------------------------------------------------------
mkdir -p build/$PLATFORM_DIR/bin
cp -r lib/* build/$PLATFORM_DIR/bin

# --------------------------------------------------------------------------
# Libraries
# --------------------------------------------------------------------------

# Engine core library
make -j -f Makefile.lib.mak $ACTION TARGET=$TARGET ASSEMBLY=engine-lib OUTPUT=engine PLATFORM=$PLATFORM_DIR LDFLAGS="$LDFLAGS"
if [ $? -ne 0 ]; then
    echo "Error: $?"
    exit $?
fi

# Game logic library
make -j -f Makefile.lib.mak $ACTION TARGET=$TARGET ASSEMBLY=game-lib OUTPUT=game PLATFORM=$PLATFORM_DIR LDFLAGS="-Lbuild/$PLATFORM_DIR/bin -lengine"
if [ $? -ne 0 ]; then
    echo "Error: $?"
    exit $?
fi

# Editor logic library
make -j -f Makefile.lib.mak $ACTION TARGET=$TARGET ASSEMBLY=editor-lib OUTPUT=editor PLATFORM=$PLATFORM_DIR LDFLAGS="-Lbuild/$PLATFORM_DIR/bin -lengine"
if [ $? -ne 0 ]; then
    echo "Error: $?"
    exit $?
fi

# --------------------------------------------------------------------------
# Executables
# --------------------------------------------------------------------------

# Editor Executable (No linking against dynamic libraries)
make -j -f Makefile.exe.mak $ACTION TARGET=$TARGET ASSEMBLY=editor PLATFORM=$PLATFORM_DIR LDFLAGS="-Lbuild/$PLATFORM_DIR/bin -lSDL3"
if [ $? -ne 0 ]; then
    echo "Error: $?"
    exit $?
fi

# Game Executable (No linking against dynamic libraries)
make -j -f Makefile.exe.mak $ACTION TARGET=$TARGET ASSEMBLY=game PLATFORM=$PLATFORM_DIR LDFLAGS="-Lbuild/$PLATFORM_DIR/bin -lSDL3"
if [ $? -ne 0 ]; then
    echo "Error: $?"
    exit $?
fi

# Tests Executable
make -j -f Makefile.exe.mak $ACTION TARGET=$TARGET ASSEMBLY=tests PLATFORM=$PLATFORM_DIR LDFLAGS="-Lbuild/$PLATFORM_DIR/bin -lengine -lgame -leditor"
if [ $? -ne 0 ]; then
    echo "Error: $?"
    exit $?
fi

echo "All assemblies $ACTION_STR_PAST successfully."
