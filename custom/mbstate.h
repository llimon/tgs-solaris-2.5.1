/*#include <wchar.h>  // Should be the correct solution*/

/*
*  If your /usr/include/wchar.h is missing mbstate_t
* 
*
* You can apeend include this file and save the file in your default include folder.
* include <mbstate.h>
*
* This way you will not need to patch source code using mbstate_t
*
* Copyright(c) 2025, Luis Limon
*/

#ifndef HAVE_MBSTATE_T
/* A temporary workaround if wchar.h is broken or missing. */
typedef struct {
    int __opaque[4]; /* Sufficiently large to hold implementation-defined state */
} mbstate_t;
#endif

/* ... inside the _rl_mb_strcaseeqn function ... */
mbstate_t ps1;      // or perhaps mbstate_t *ps1; if dynamically allocated
mbstate_t ps2;      // or perhaps mbstate_t *ps2; if dynamically allocated
const char *s2;     // The second string to compare
int flags;          // Integer for internal function flags

