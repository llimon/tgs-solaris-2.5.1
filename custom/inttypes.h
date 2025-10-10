#ifndef _SPARC32_INTTYPE_H
#define _SPARC32_INTTYPE_H

#include <stdint.h>
#include <sys/types.h>
#if defined(__sun__) && defined(__sparc__)
// For 32-bit SPARC, like Sun-4m/Solaris 2.5.1
#define PRIdPTR "ld"
#define PRIuPTR "lu"
#define PRIuMAX "llu"
#define PRIoMAX "llo"

#define PRId32 "d"
#define PRIx32 "x"
#define PRId64 "lld"
#define PRIx64 "llx"

#elif defined(__x86_64__) || defined(__aarch64__)
// For 64-bit systems
#define PRIdPTR "ld"
#define PRIuPTR "lu"
#define PRIuMAX "llu"
#define PRIoMAX "llo"

#define PRId32 "d"
#define PRIx32 "x"
#define PRId64 "ld"
#define PRIx64 "lx"

#else
// Default for older 32-bit systems
#define PRIdPTR "d"
#define PRIuPTR "u"
#define PRIuMAX "llu"
#define PRIoMAX "llo"

#define PRId32 "d"
#define PRIx32 "x"
#define PRId64 "lld"
#define PRIx64 "llx"

#endif

#endif /* _SPARC32_INTTYPE_H */
