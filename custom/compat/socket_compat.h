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
#ifndef socklen_t
#ifndef SOCKLEN_T
#ifndef _SOCKLEN_T
typedef int socklen_t;
#define SOCKLEN_T
#define _SOCKLEN_T
#endif
#endif
#endif

/* Solaris 2.5.1 in_addr_t fallback */
#ifndef IN_ADDR_T
#ifndef _IN_ADDR_T
typedef unsigned long in_addr_t;
#define IN_ADDR_T
#define _IN_ADDR_T
#endif
#endif

/* Solaris 2.5.1 INET_ADDRSTRLEN fallback */
#ifndef INET_ADDRSTRLEN
#define INET_ADDRSTRLEN 16
#endif

#ifndef INET6_ADDRSTRLEN
#define INET6_ADDRSTRLEN 46
#endif

#endif /* COMPAT_SOCKET_COMPAT_H */
