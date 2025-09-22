/****
** Copyright(c) 2025, Luis Enrique Limon
** Custom implementration of isinf.
** It is supposed to be provided by C99, Problem under Solaris 2.5.1 and GCC 4.x Our compiler is C99 but libc is NOT!.
** This isinf implementation was written to backport vim 8.2 to solaris.
****/

#ifndef MY_ISINF_H
#define MY_ISINF_H

#include <stdint.h>

// Use a macro to provide a generic isinf that selects the right function.
// The sizeof() operator is a compile-time constant.
// The ?: operator is standard C, and the compiler will only evaluate the
// branch that matches the type of `x`.
#define isinf(x) \
    (sizeof(x) == sizeof(float) ? isinf_float(x) : isinf_double(x))

/*
 * Custom isinf for single-precision float types.
 */
static inline int isinf_float(float x) {
    union {
        float f;
        uint32_t i;
    } u;
    
    u.f = x;
    
    // An infinite float has an exponent of all 1s (0xFF) and a mantissa of 0.
    // The pattern for an all-ones exponent and all-zeros mantissa is 0x7F800000.
    return (u.i & 0x7FFFFFFF) == 0x7F800000;
}

/*
 * Custom isinf for double-precision float types.
 */
static inline int isinf_double(double x) {
    union {
        double d;
        uint64_t i;
    } u;

    u.d = x;

    // An infinite double has an exponent of all 1s (0x7FF) and a mantissa of 0.
    // The pattern for an all-ones exponent and all-zeros mantissa is 0x7FF0000000000000.
    return (u.i & 0x7FFFFFFFFFFFFFFF) == 0x7FF0000000000000;
}

#endif /* MY_ISINF_H */

