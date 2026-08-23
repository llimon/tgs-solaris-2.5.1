#!/bin/sh
gcc -x c -std=gnu99 -D_TEST_USLEEP_COMPAT -lposix4 usleep_compat.h -o usleep_compat
