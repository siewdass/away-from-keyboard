#!/usr/bin/env bash
if [ ! -f "build" ]; then
	mkdir build
fi
cd build
cmake ..
make -j4
