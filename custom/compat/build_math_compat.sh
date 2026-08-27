#!/bin/sh
gcc -x c -std=gnu99 -D_TEST_MATH_COMPAT_CUSTOM -lposix4 math_compat.h -o math_compat
