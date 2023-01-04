#!/usr/bin/env bash
if [ ! -f "build" ]; then
	rm -rf build > /dev/null 2>&1
fi
mkdir build
cd build
cmake .. > /dev/null 2>&1
make > /dev/null 2>&1
cp afk ../
