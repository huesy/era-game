#!/bin/bash

# Build engine first
echo "--- Cleaning Engine ---"
cd engine || exit
make clean
if [ $? -ne 0 ]; then
    echo "Failed to clean engine."
    exit 1
fi
cd ..

# Build game next
echo "--- Cleaning Game ---"
cd game || exit
make clean
if [ $? -ne 0 ]; then
    echo "Failed to clean game."
    exit 1
fi
cd ..

echo "--- Cleaned all successfully ---"
