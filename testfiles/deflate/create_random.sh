#!/bin/bash

if [ ${1} -eq 0 ]; then
cp /dev/null random0
else
dd if=/dev/urandom of="random${1}" bs=$1 count=1
fi
# Parameters:
# -9 maximum compression
# -c compress
# -n do not save the original file  name
gzip -9 -c "random${1}" -n > random.gz
tail -c +11 random.gz > foo       # Strip the first 10 bytes
head -c -8 foo > "random${1}.raw" # Strip the last 8 bytes

rm foo
rm random.gz
