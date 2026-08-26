#!/bin/sh
gcc -x c -std=gnu99 -D_TEST_GETADDRINFO_COMPAT -lsocket -lnsl getaddrinfo_compat.h -o getaddrinfo_compat
