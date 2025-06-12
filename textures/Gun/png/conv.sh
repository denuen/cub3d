#!/bin/bash

# Check if ImageMagick's convert tool is available
if ! command -v convert &> /dev/null
then
    echo "ImageMagick's 'convert' tool could not be found. Please install ImageMagick."
    exit 1
fi

# --- Configuration ---
SCALE_FACTOR=1
ORIGINAL_WIDTH=32
ORIGINAL_HEIGHT=32
# --- End Configuration ---

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <input_png_file> <output_xpm_file>"
    exit 1
fi

INPUT_PNG="$1"
OUTPUT_XPM="$2"

# Calculate the new dimensions
NEW_WIDTH=$((ORIGINAL_WIDTH * SCALE_FACTOR))
NEW_HEIGHT=$((ORIGINAL_HEIGHT * SCALE_FACTOR))

echo "Converting $INPUT_PNG to $OUTPUT_XPM..."
echo "Original dimensions: ${ORIGINAL_WIDTH}x${ORIGINAL_HEIGHT}"
echo "Scale factor: $SCALE_FACTOR"
echo "New dimensions: ${NEW_WIDTH}x${NEW_HEIGHT}"

# Convert and resize the image
convert "$INPUT_PNG" -resize "${NEW_WIDTH}x${NEW_HEIGHT}" "$OUTPUT_XPM"

if [ $? -eq 0 ]; then
    echo "Conversion successful: $OUTPUT_XPM created."
else
    echo "Conversion failed."
    exit 1
fi

echo "Make sure to update your C code to use the new XPM file and set the scale factor in draw_gun to 1 if you use this pre-scaled XPM."
