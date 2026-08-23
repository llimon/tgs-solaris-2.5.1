#!/bin/sh
gcc -x c -std=gnu99 -D_TEST_MBSTOWCS_COMPAT -lw mbstowcs_custom.h -o mbstowcs_custom
