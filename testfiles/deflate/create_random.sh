#!/bin/sh

dd if=/dev/urandom of=random1024 bs=1024 count=1
gzip -c random1024 -n > random.gz
tail -c +11 random.gz > foo # Strip the first 10 bytes
head -c -8 foo > random1024.raw # Strip the last 8 bytes

rm foo
rm random.gz
