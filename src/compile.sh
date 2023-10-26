#!/bin/bash
for sourcefile in `ls ./*.c`; do
	gcc $sourcefile -o $sourcefile[:2] -Wall -Wpedantic
done
echo ""
echo "Finished"
echo ""
