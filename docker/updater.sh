#!/bin/bash
URL="https://static.tibia.com/download/tibia.x64.tar.gz"
FILE="tibia.x64.tar.gz"
DIR="/tibia/Tibia"

# -N solo descarga si el archivo en el servidor es más nuevo que el local
wget -q -N $URL

# Si el archivo se modificó hace menos de 1 minuto o no existe la carpeta del juego
if [ -n "$(find "$FILE" -mmin -1 2>/dev/null)" ] || [ ! -d "$DIR" ]; then
    echo "Actualizando archivos del juego..."
    rm -rf "$DIR" && tar -xf "$FILE" -C /tibia/
    echo "Sincronización terminada."
else
    echo "El juego ya está al día."
fi