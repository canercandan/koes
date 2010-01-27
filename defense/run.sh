#!/usr/bin/env sh

FILES="
set1.txt
set2i.txt
set2ii.txt
set3i.txt
set3ii.txt
set4i.txt
set4ii.txt
set4iii.txt
set5i.txt
set5ii.txt
set6.txt
"

for file in $FILES; do
    ../bin/es -f $file
done
