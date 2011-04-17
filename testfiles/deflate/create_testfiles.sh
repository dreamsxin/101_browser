#!/bin/bash

./create_random.sh 0
mv random0 null
mv random0.raw null.raw
./create_random.sh 1024
./create_random.sh 65536

# The gzip is not able to make a file of 65536 random bytes using only 
# one block. So we have to hack a manual solution...
dd if=/dev/urandom of="random65535" bs=65535 count=1
echo -n -e "\x01\xff\xff\x00\x00" > random65535.raw
cat random65535 >> random65535.raw


