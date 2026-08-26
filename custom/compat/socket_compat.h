#ifndef COMPAT_SOCKET_COMPAT_H
#define COMPAT_SOCKET_COMPAT_H

#include <sys/types.h>
#include <sys/socket.h>

/* Shutdown constants for shutdown(2) */
#ifndef SHUT_RD
#define SHUT_RD   0
#endif

#ifndef SHUT_WR
#define SHUT_WR   1
#endif

#ifndef SHUT_RDWR
#define SHUT_RDWR 2
#endif

/* Solaris 2.5.1 socklen_t fallback */
#ifndef SOCKLEN_T
#ifndef _SOCKLEN_T
typedef int socklen_t;
#define SOCKLEN_T
#define _SOCKLEN_T
#endif
#endif

#endif /* COMPAT_SOCKET_COMPAT_H */
