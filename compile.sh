#!/bin/bash
for file in `ls src/*.c`; do
    echo `gcc $file -o ${file/.c/} -O2 -Wall`
done
echo "\n"
echo "Done"
