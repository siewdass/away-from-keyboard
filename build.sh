#!/usr/bin/env bash
mkdir -p build
cd build
cmake ..
make -j4

if [ -d /opt/afk ]; then
	sudo cp afk /opt/afk/afk
	echo "afk copiado a /opt/afk/"
else
	echo "/opt/afk no existe, no se copio el binario"
fi
