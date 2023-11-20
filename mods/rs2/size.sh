#!/bin/bash

# Define the file paths
files=("output/rs2.bin" "output/input.bin" "output/draw.bin")

# Initialize the total size variable
totalSize=0

# Loop through each file in the files array
for file in "${files[@]}"
do
    # Check if the file exists
    if [ -f "$file" ]; then
        # Get the size of the file in bytes
        size=$(stat -c%s "$file")
        echo "Size of $file: $size bytes"
        
        # Add the size to the total
        totalSize=$((totalSize + size))
    else
        echo "File $file not found."
    fi
done

# Print the total size
echo "Total size of all files: $totalSize bytes"
