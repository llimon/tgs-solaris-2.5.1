#ifndef _SHUT_CONSTS
#define _SHUT_CONSTS

#define SHUT_RD   0
#define SHUT_WR   1
#define SHUT_RDWR 2

// On older systems that don't have socklen_t, we define our own.
// We use a guard to prevent redefinition issues.
#ifndef SOCKLEN_T
    // The type `int` works for older systems and doesn't cause issues on newer ones
    // because its size is sufficient.
    typedef int socklen_t;
#endif

#endif
