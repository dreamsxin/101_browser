#!/bin/bash

./create_random.sh 0
# Why this renaming? We don't want to ignore this file in
# the repository (random* is ignored (see .gitignore)
mv random0 null
mv random0.raw null.raw
./create_random.sh 1024
./create_random.sh 65536

# The gzip is not able to make a file of 65536 random bytes using only 
# one block. So we have to hack a manual solution...
dd if=/dev/urandom of="random65535" bs=65535 count=1
# -n: no newline at the end
echo -n -e "\x01\xff\xff\x00\x00" > random65535.raw
cat random65535 >> random65535.raw

# At the moment, we don't want this test file, that's why it is commented
# cat null.raw null.raw > error_data_after_final_block.raw

# 02 00 - the same as null.raw, but single block is not marked
# as final (03 00 as in null.raw - here the block is marked as final
echo -n -e "\x02\x00" > error_no_final_block.raw
