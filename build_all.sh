#!/bin/bash

PLATFORM="$1"
ACTION="$2"
TARGET="$3"

set echo off

txtgrn=$(echo -e '\e[0;32m')
txtred=$(echo -e '\e[0;31m')
txtrst=$(echo -e '\e[0m')

if [ $ACTION == "all" ] || [ $ACTION == "build" ]
then
  ACTION="all"
  echo "Building all on $PLATFORM ($TARGET)..."
elif [ $ACTION == "clean" ]
then
  ACTION="clean"
  echo "Cleaning all on $PLATFORM ($TARGET)..."
else
  echo "${txtred}Invalid action: $ACTION${txtrst}"
  exit 1
fi

# Build engine first
echo "--- Building Engine ---"
cd engine || exit
make
if [ $? -ne 0 ]; then
    echo "Failed to build engine."
    exit 1
fi
cd ..

# Build game next
echo "--- Building Game ---"
cd game || exit
make
if [ $? -ne 0 ]; then
    echo "Failed to build game."
    exit 1
fi
cd ..

echo "--- Build All Completed Successfully ---"
