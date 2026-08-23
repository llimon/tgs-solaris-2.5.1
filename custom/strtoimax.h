#ifndef MY_STRTOIMAX_H
#define MY_STRTOIMAX_H

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>

// Define intmax_t and UINTMAX_MAX if they are not present
// The custom stdint.h from your previous example can be used here.
// #ifndef INTMAX_MAX
// typedef long long intmax_t;
// #define INTMAX_MAX LLONG_MAX
// #define INTMAX_MIN LLONG_MIN
// #endif
// 
// #ifndef UINTMAX_MAX
// typedef unsigned long long uintmax_t;
// #define UINTMAX_MAX ULLONG_MAX
// #endif

// Mark functions as static inline for header-only implementation
// and to avoid linker errors due to multiple definitions.

// Check if a system-level strtoimax already exists
#if !defined(HAVE_STRTOIMAX) && !defined(strtoimax)

// Use a macro to provide a transparent fallback
#define strtoimax my_strtoimax
#define strtoumax my_strtoumax

static inline intmax_t my_strtoimax(const char *nptr, char **endptr, int base) {
    const char *s = nptr;
    intmax_t acc = 0;
    int sign = 1;
    int any = 0;
    int overflow_occurred = 0;

    if ((base < 0 || base == 1 || base > 36) && base != 0) {
        if (endptr) *endptr = (char *)nptr;
        errno = EINVAL;
        return 0;
    }

    while (isspace((unsigned char)*s)) {
        s++;
    }

    if (*s == '-') {
        sign = -1;
        s++;
    } else if (*s == '+') {
        s++;
    }

    if (base == 0) {
        if (*s == '0') {
            if (*(s + 1) == 'x' || *(s + 1) == 'X') {
                base = 16;
                s += 2;
            } else {
                base = 8;
                s++;
            }
        } else {
            base = 10;
        }
    } else if (base == 16) {
        if (*s == '0' && (*(s + 1) == 'x' || *(s + 1) == 'X')) {
            s += 2;
        }
    }

    intmax_t cutoff = (sign == 1) ? (INTMAX_MAX / base) : -(INTMAX_MIN / base);
    intmax_t cutlim = (sign == 1) ? (INTMAX_MAX % base) : -(INTMAX_MIN % base);
    if (cutlim < 0) cutlim = -cutlim;

    while (*s) {
        int digit;
        if (isdigit((unsigned char)*s)) {
            digit = *s - '0';
        } else if (isalpha((unsigned char)*s)) {
            if (islower((unsigned char)*s)) {
                digit = *s - 'a' + 10;
            } else {
                digit = *s - 'A' + 10;
            }
        } else {
            break;
        }

        if (digit >= base) {
            break;
        }

        if (acc > cutoff || (acc == cutoff && digit > cutlim)) {
            overflow_occurred = 1;
        } else {
            acc = acc * base + digit;
            any = 1;
        }
        s++;
    }

    if (overflow_occurred) {
        errno = ERANGE;
        if (sign == 1) {
            acc = INTMAX_MAX;
        } else {
            acc = INTMAX_MIN;
        }
    } else {
        acc *= sign;
    }

    if (endptr != NULL) {
        if (any) {
            *endptr = (char *)s;
        } else {
            *endptr = (char *)nptr;
        }
    }

    return acc;
}


static inline uintmax_t my_strtoumax(const char *nptr, char **endptr, int base) {
    const char *s = nptr;
    uintmax_t acc = 0;
    int any = 0;
    int overflow_occurred = 0;

    if ((base < 0 || base == 1 || base > 36) && base != 0) {
        if (endptr) *endptr = (char *)nptr;
        errno = EINVAL;
        return 0;
    }

    while (isspace((unsigned char)*s)) {
        s++;
    }

    if (*s == '+') {
        s++;
    } else if (*s == '-') {
        s++;
        if (endptr) *endptr = (char *)nptr;
        errno = ERANGE;
        return UINTMAX_MAX;
    }

    if (base == 0) {
        if (*s == '0') {
            if (*(s + 1) == 'x' || *(s + 1) == 'X') {
                base = 16;
                s += 2;
            } else {
                base = 8;
                s++;
            }
        } else {
            base = 10;
        }
    } else if (base == 16) {
        if (*s == '0' && (*(s + 1) == 'x' || *(s + 1) == 'X')) {
            s += 2;
        }
    }

    uintmax_t cutoff = UINTMAX_MAX / base;
    uintmax_t cutlim = UINTMAX_MAX % base;

    while (*s) {
        int digit;
        if (isdigit((unsigned char)*s)) {
            digit = *s - '0';
        } else if (isalpha((unsigned char)*s)) {
            if (islower((unsigned char)*s)) {
                digit = *s - 'a' + 10;
            } else {
                digit = *s - 'A' + 10;
            }
        } else {
            break;
        }

        if (digit >= base) {
            break;
        }

        if (acc > cutoff || (acc == cutoff && digit > cutlim)) {
            overflow_occurred = 1;
        } else {
            acc = acc * base + digit;
            any = 1;
        }
        s++;
    }

    if (overflow_occurred) {
        errno = ERANGE;
        acc = UINTMAX_MAX;
    }

    if (endptr != NULL) {
        if (any) {
            *endptr = (char *)s;
        } else {
            *endptr = (char *)nptr;
        }
    }

    return acc;
}

#endif // !HAVE_STRTOIMAX

#endif // MY_STRTOIMAX_H

