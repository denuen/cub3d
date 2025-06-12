#!/bin/bash

if ! command -v convert &> /dev/null; then
    echo "Errore: il comando 'convert' (ImageMagick) non è installato." >&2
    exit 1
fi

INPUT_DIR="."

for file in "$INPUT_DIR"/*.png; do
    [ -e "$file" ] || continue
    filename=$(basename -- "$file")
    name="${filename%.*}"
    output="${INPUT_DIR}/${name}.xpm"
    convert "$file" "$output"

    if [ $? -eq 0 ]; then
        echo "Convertito: $file -> $output"
    else
        echo "Errore nella conversione di: $file" >&2
    fi
done
