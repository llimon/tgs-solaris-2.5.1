#!/bin/sh
gcc -x c -std=gnu99 -D _TEST_STRTOIMAX_CUSTOM -lposix4 strtoimax_compat.h -o strtoimax_compat
