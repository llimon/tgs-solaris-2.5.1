#ifndef _STDINT_H
#define _STDINT_H

/* Fallback for environments without stdint.h */

#ifdef _MSC_VER
    // Correctly define uintptr_t for MSVC based on architecture
    #if defined(_M_AMD64)
      typedef unsigned __int64 uintptr_t; // 64-bit systems
    #else
      typedef unsigned long uintptr_t;    // 32-bit systems
    #endif

    /* Microsoft Visual C++ definitions */
    typedef __int8            int8_t;
    typedef __int16           int16_t;
    typedef __int32           int32_t;
    typedef __int64           int64_t;
    typedef unsigned __int8   uint8_t;
    typedef unsigned __int16  uint16_t;
    typedef unsigned __int32  uint32_t;
    typedef unsigned __int64  uint64_t;
#elif defined(__GNUC__)

    #include <sys/synch.h>
    #if defined(__UINTPTR_TYPE__)
        typedef __UINTPTR_TYPE__ uintptr_t;
    #else
        // Fallback for older GCC versions (less portable, but safest guess for 32-bit)
        typedef unsigned int uintptr_t;
    #endif

    #if defined(__INTPTR_TYPE__)
        typedef __INTPTR_TYPE__ intptr_t;
    #else
        typedef signed int intptr_t;
    #endif

    /* Other definitions for GCC */

    typedef signed char        int8_t;
    typedef short              int16_t;
    typedef int                int32_t;
    typedef long long          int64_t;
    #ifndef _UINT8_T
    typedef unsigned char      uint8_t;
    #endif
    typedef unsigned short     uint16_t;
    #ifndef _UINT32_T
    typedef unsigned long       uint32_t;
    #endif
    #ifndef _UINT64_T
    //typedef unsigned long long uint64_t;
    typedef u_longlong_t uint64_t;
    #endif

    #ifndef _INTMAX_T
    typedef long long          intmax_t;
    #define _INTMAX_T
    #endif

    #ifndef _UINTMAX_T
    typedef unsigned long long uintmax_t;
    #define _UINTMAX_T
    #endif

    #ifndef LONG_MIN
    #define LONG_MIN 	(-2147483647L - 1)
    #endif


/* Define int_fast types.  short is often slow */
/* some come from gcc: ./sparc-sun-solaris2.5.1/libgfortran/gstdint.h */


    // Fastest integer types
    #ifndef _INT_FAST8_T
    typedef int8_t       int_fast8_t;
    #define _INT_FAST8_T
    #endif

    #ifndef _UINT_FAST8_T
    typedef uint8_t      uint_fast8_t;
    #define _UINT_FAST8_T
    #endif

    #ifndef _INT_FAST16_T
    typedef int          int_fast16_t;
    #define _INT_FAST16_T
    #endif

    #ifndef _UINT_FAST16_T
    typedef unsigned int uint_fast16_t;
    #define _UINT_FAST16_T
    #endif
    #ifndef _INT_FAST32_T
    typedef int32_t      int_fast32_t;
    #endif


    #ifdef _INT64_T
    typedef int64_t      int_fast64_t;
    #endif
    #ifdef _UINT64_T
    typedef uint64_t     uint_fast64_t;
    #endif

    /* Limits for fixed-width integer types for 32-bit */
    #ifndef UINT16_MAX
    #define UINT16_MAX         65535U      /* 2**16 - 1 */
    #endif
    #ifndef UINT32_MAX
    #define UINT32_MAX         4294967295U /* 2**32 - 1 */
    #endif
    #ifndef UINT64_MAX
    #define UINT64_MAX         18446744073709551615ULL /* 2**64 - 1 */
    #endif

    #ifndef INT32_MAX
    #define INT32_MAX          2147483647L /* 2**31 - 1 */
    #endif

    #ifndef INT32_MIN
    #define INT32_MIN          (-2147483647L - 1L) /* Or -2147483648L */
    #endif

    #ifndef SIZE_MAX
    #define SIZE_MAX           UINT32_MAX /* `size_t` is 32-bit on a 32-bit system */
    #endif

    /* constants formulated to build openssl3 
     * include/internal/numbers.h:61:1: warning: "UINT64_MAX" redefined
     */

    // 8-bit integer limits
    #ifndef INT8_MAX
    # define INT8_MAX 127
    #endif
    #ifndef INT8_MIN
    # define INT8_MIN (-127-1)
    #endif
    #ifndef UINT8_MAX
    # define UINT8_MAX 255
    #endif

    #ifndef INT16_MAX
    # define INT16_MAX 32767
    #endif
    #ifndef INT16_MIN
    # define INT16_MIN (-32767-1)
    #endif

    #ifndef INT64_MIN
    # define INT64_MIN (-9223372036854775807LL - 1LL)
    #endif
    
    #ifndef INT64_MAX
    # define INT64_MAX 9223372036854775807LL
    #endif

    /* constants needed by git
    */
    #ifndef INTMAX_MAX
    #define INTMAX_MAX  INT64_MAX
    #endif
    #ifndef INTMAX_MIN
    #define INTMAX_MIN  INT64_MIN
    #endif
    #ifndef UINTMAX_MAX
    #define UINTMAX_MAX UINT64_MAX
    #endif 

#endif 

#endif /* _STDINT_H */

