#ifndef COMPAT_GETADDRINFO_COMPAT_H
#define COMPAT_GETADDRINFO_COMPAT_H

/* 1. Include standard network headers first so native types parse */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifndef AF_INET
#define AF_INET 2
#endif

/* 2. Clear potential system macros to avoid header mangling */
#undef getaddrinfo
#undef freeaddrinfo
#undef addrinfo

/* 3. Unique struct definition to avoid header redefinition errors */
struct compat_addrinfo {
    int                     ai_flags;
    int                     ai_family;
    int                     ai_socktype;
    int                     ai_protocol;
    size_t                  ai_addrlen;
    char                   *ai_canonname;
    struct sockaddr        *ai_addr;
    struct compat_addrinfo *ai_next;
};

static inline void freeaddrinfo_compat(struct compat_addrinfo *res) {
    struct compat_addrinfo *p, *next;
    for (p = res; p != NULL; p = next) {
        next = p->ai_next;
        if (p->ai_addr) free(p->ai_addr);
        if (p->ai_canonname) free(p->ai_canonname);
        free(p);
    }
}

static inline int getaddrinfo_compat(const char *nodename, const char *servname,
                                    const struct compat_addrinfo *hints, 
                                    struct compat_addrinfo **res) {
    struct hostent *he;
    struct compat_addrinfo *head = NULL, *prev = NULL, *curr = NULL;
    struct sockaddr_in *sa_in;
    int port = 0;
    int i = 0;

    if (!nodename && !servname) return -1;
    if (servname) port = htons((unsigned short)atoi(servname));

    /* Legacy IPv4 lookup via gethostbyname */
    he = gethostbyname(nodename);
    if (!he || he->h_addrtype != AF_INET) return -1;

    /* Iterate through all returned IPv4 addresses */
    while (he->h_addr_list[i] != NULL) {
        curr = (struct compat_addrinfo *)calloc(1, sizeof(struct compat_addrinfo));
        sa_in = (struct sockaddr_in *)calloc(1, sizeof(struct sockaddr_in));

        if (!curr || !sa_in) {
            if (curr) free(curr);
            if (sa_in) free(sa_in);
            freeaddrinfo_compat(head);
            return -1;
        }

        sa_in->sin_family = AF_INET;
        sa_in->sin_port = port;
        memcpy(&sa_in->sin_addr, he->h_addr_list[i], he->h_length);

        curr->ai_family = AF_INET;
        curr->ai_socktype = (hints) ? hints->ai_socktype : SOCK_DGRAM;
        curr->ai_protocol = (hints) ? hints->ai_protocol : 0;
        curr->ai_addrlen = sizeof(struct sockaddr_in); /* Strictly 16 bytes for ISC assertion */
        curr->ai_addr = (struct sockaddr *)sa_in;
        curr->ai_next = NULL;

        if (he->h_name) {
            curr->ai_canonname = strdup(he->h_name);
        }

        if (!head) head = curr;
        else prev->ai_next = curr;

        prev = curr;
        i++;
    }

    *res = head;
    return 0;
}

/* Macro overrides for legacy source patches */
#define addrinfo compat_addrinfo
#define getaddrinfo getaddrinfo_compat
#define freeaddrinfo freeaddrinfo_compat

#endif /* COMPAT_GETADDRINFO_COMPAT_H */


/* =========================================================================
 * Embedded Unit Test Harness
 * Compile with: gcc -x c -std=gnu99 -D_TEST_GETADDRINFO_COMPAT=1 compat/getaddrinfo_compat.h -o test_getaddrinfo -lsocket -lnsl
 * ========================================================================= */
#ifdef _TEST_GETADDRINFO_COMPAT
int main(int argc, char **argv) {
    const char *target = (argc > 1) ? argv[1] : "0.pool.ntp.org";
    struct compat_addrinfo hints, *res = NULL, *p = NULL;
    int count = 0;

    printf("=== Running Solaris 2.5.1 getaddrinfo_compat Unit Test ===\n");
    printf("Testing resolution for host: %s\n", target);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo_compat(target, "123", &hints, &res) != 0) {
        fprintf(stderr, "FAIL: Unable to resolve %s\n", target);
        return 1;
    }

    for (p = res; p != NULL; p = p->ai_next) {
        struct sockaddr_in *sin = (struct sockaddr_in *)p->ai_addr;
        count++;

        printf("  [%d] Resolved IP: %s (addrlen: %lu bytes, port: %d)\n",
               count, inet_ntoa(sin->sin_addr), (unsigned long)p->ai_addrlen, ntohs(sin->sin_port));

        /* Verify ISC assertion condition from ntpdate.c */
        if (p->ai_addrlen > sizeof(struct sockaddr_in)) {
            fprintf(stderr, "FAIL: ai_addrlen exceeds sizeof(sockaddr_in)!\n");
            freeaddrinfo_compat(res);
            return 1;
        }
    }

    freeaddrinfo_compat(res);

    if (count > 0) {
        printf("RESULT: PASSED (%d address(es) returned cleanly)\n", count);
        return 0;
    }

    printf("RESULT: FAILED (No addresses returned)\n");
    return 1;
}
#endif /* _TEST_GETADDRINFO_COMPAT */

