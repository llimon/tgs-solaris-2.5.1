#!/bin/sh
gcc -x c -std=gnu99 -D_TEST_UTIMES_COMPAT utimes_compat.h -o utimes_compat
