#!/usr/bin/env bash
if [ ! -f "build" ]; then
	rm -rf build
fi
mkdir build
cd build
cmake ..
make
cp afk ../
