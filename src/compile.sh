#!/bin/bash
for sourcefile in `ls ./*.c`; do
    echo `gcc $sourcefile -o ${sourcefile/.c/} -Wall -Wpedantic`
done
echo ""
echo "Finished"
echo ""
