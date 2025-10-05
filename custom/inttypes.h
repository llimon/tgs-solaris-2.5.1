#ifndef _SPARC32_INTTYPE_H
#define _SPARC32_INTTYPE_H


#if defined(__sun__) && defined(__sparc__)
// For 32-bit SPARC, like Sun-4m/Solaris 2.5.1
#include <stdint.h>
		       //

#define PRIdPTR "ld"
#define PRIuPTR "lu"

#elif defined(__x86_64__) || defined(__aarch64__)
// For 64-bit systems
#define PRIdPTR "ld"
#define PRIuPTR "lu"

#else
// Default for older 32-bit systems
#define PRIdPTR "d"
#define PRIuPTR "u"

#endif

#endif /* _SPARC32_INTTYPE_H */
