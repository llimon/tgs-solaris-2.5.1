# custom stdint.h and inttypes.h

## Description
One of the mayor problems building software in Solaris 2.5.1 is missing stdint.h. It is supposed to be part of C99 standard, GCC assumes they exist. But because they are provided by libc with is very old in Solaris 2.5.1 and does not have stdint.h included in the distribution, compilation fails.  

Because we target ONLY  Sparc 32bit. We can assume specific sizes for the different types defined by stdint.h.

With some trial and error and the reluctant help of Google AI. I was able to formulate a functional stdint.h.

- Luis



