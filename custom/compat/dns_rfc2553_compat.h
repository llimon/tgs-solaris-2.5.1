#ifndef COMPAT_DNS_RFC2553_COMPAT_H
#define COMPAT_DNS_RFC2553_COMPAT_H

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

/* Standard RFC 2553 / RFC 3493 Error Codes */
#ifndef EAI_ADDRFAMILY
#define EAI_ADDRFAMILY  1 /* Address family for hostname not supported */
#define EAI_AGAIN       2 /* Temporary failure in name resolution */
#define EAI_BADFLAGS    3 /* Invalid value for ai_flags */
#define EAI_FAIL        4 /* Non-recoverable failure in name resolution */
#define EAI_FAMILY      5 /* ai_family not supported */
#define EAI_MEMORY      6 /* Memory allocation failure */
#define EAI_NODATA      7 /* No address associated with hostname */
#define EAI_NONAME      8 /* Name or service not known */
#define EAI_SERVICE     9 /* Servname not supported for ai_socktype */
#define EAI_SOCKTYPE   10 /* ai_socktype not supported */
#define EAI_SYSTEM     11 /* System error returned in errno */
#define EAI_BADHINTS   12 /* Invalid hints */
#define EAI_PROTOCOL   13 /* Resolved protocol is unknown */
#define EAI_MAX        14
#endif

/* AI Flags */
#ifndef AI_PASSIVE
#define AI_PASSIVE     0x0001
#define AI_CANONNAME   0x0002
#define AI_NUMERICHOST 0x0004
#define AI_NUMERICSERV 0x0008
#endif

/* 1. Standard struct addrinfo definition */
#ifndef COMPAT_ADDRINFO_DEFINED
#define COMPAT_ADDRINFO_DEFINED
struct addrinfo {
    int              ai_flags;
    int              ai_family;
    int              ai_socktype;
    int              ai_protocol;
    size_t           ai_addrlen;
    char            *ai_canonname;
    struct sockaddr *ai_addr;
    struct addrinfo *ai_next;  /* Correct self-referential linked list pointer */
};
#endif /* COMPAT_ADDRINFO_DEFINED */

/* 2. Free linked list */
static inline void freeaddrinfo(struct addrinfo *res) {
    struct addrinfo *p, *next;
    for (p = res; p != NULL; p = next) {
        next = p->ai_next;
        if (p->ai_addr) free(p->ai_addr);
        if (p->ai_canonname) free(p->ai_canonname);
        free(p);
    }
}

/* 3. Error string lookup using clean switch statement */
static inline char *gai_strerror(int ecode) {
    switch (ecode) {
        case 0:              return "Success";
        case EAI_ADDRFAMILY: return "Address family for hostname not supported";
        case EAI_AGAIN:      return "Temporary failure in name resolution";
        case EAI_BADFLAGS:   return "Invalid value for ai_flags";
        case EAI_FAIL:       return "Non-recoverable failure in name resolution";
        case EAI_FAMILY:     return "ai_family not supported";
        case EAI_MEMORY:     return "Memory allocation failure";
        case EAI_NODATA:     return "No address associated with hostname";
        case EAI_NONAME:     return "Name or service not known";
        case EAI_SERVICE:    return "Servname not supported for ai_socktype";
        case EAI_SOCKTYPE:   return "ai_socktype not supported";
        case EAI_SYSTEM:     return "System error";
        case EAI_BADHINTS:   return "Invalid hints";
        case EAI_PROTOCOL:   return "Unknown protocol";
        default:             return "Unknown error";
    }
}

/* 4. Multi-IP DNS Pool-Capable getaddrinfo */
static inline int getaddrinfo(const char *nodename, const char *servname,
                              const struct addrinfo *hints,
                              struct addrinfo **res) {
    struct hostent *he;
    struct servent *sp;
    struct addrinfo *head = NULL, *prev = NULL, *curr = NULL;
    struct sockaddr_in *sa_in;
    int port = 0;
    int family = AF_UNSPEC, socktype = 0, protocol = 0, flags = 0;
    const char *proto = NULL;
    int i = 0;

    if (nodename == NULL && servname == NULL)
        return (EAI_NONAME);

    if (hints != NULL) {
        family = hints->ai_family;
        socktype = hints->ai_socktype;
        protocol = hints->ai_protocol;
        flags = hints->ai_flags;

        if (family != AF_UNSPEC && family != AF_INET)
            return (EAI_FAMILY);

        switch (socktype) {
            case 0:
                break;
            case SOCK_STREAM:
                proto = "tcp";
                break;
            case SOCK_DGRAM:
                proto = "udp";
                break;
            default:
                return (EAI_SOCKTYPE);
        }
    }

    if (servname != NULL) {
        char *e;
        long tmp_port = strtol(servname, &e, 10);
        if (*e == '\0') {
            if (tmp_port < 0 || tmp_port > 65535)
                return (EAI_SERVICE);
            port = htons((unsigned short)tmp_port);
        } else {
            sp = getservbyname(servname, proto);
            if (sp == NULL)
                return (EAI_SERVICE);
            port = sp->s_port;
            if (socktype == 0) {
                if (strcmp(sp->s_proto, "tcp") == 0) socktype = SOCK_STREAM;
                else if (strcmp(sp->s_proto, "udp") == 0) socktype = SOCK_DGRAM;
            }
        }
    }

    if (nodename != NULL) {
        he = gethostbyname(nodename);
        if (he == NULL || he->h_addrtype != AF_INET)
            return (EAI_NONAME);

        /* Build full linked list for all returned DNS pool records */
        while (he->h_addr_list[i] != NULL) {
            curr = (struct addrinfo *)calloc(1, sizeof(struct addrinfo));
            sa_in = (struct sockaddr_in *)calloc(1, sizeof(struct sockaddr_in));

            if (!curr || !sa_in) {
                if (curr) free(curr);
                if (sa_in) free(sa_in);
                freeaddrinfo(head);
                return (EAI_MEMORY);
            }

            sa_in->sin_family = AF_INET;
            sa_in->sin_port = (unsigned short)port;
            memcpy(&sa_in->sin_addr, he->h_addr_list[i], he->h_length);

            curr->ai_flags = flags;
            curr->ai_family = AF_INET;
            curr->ai_socktype = (socktype != 0) ? socktype : SOCK_DGRAM;
            curr->ai_protocol = protocol;
            curr->ai_addrlen = sizeof(struct sockaddr_in);
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
    }

    if (head == NULL)
        return (EAI_NONAME);

    *res = head;
    return 0;
}

/* 5. Standard Reverse Lookup getnameinfo */
static inline int getnameinfo(const struct sockaddr *sa, socklen_t salen,
                              char *host, size_t hostlen,
                              char *serv, size_t servlen, int flags) {
    struct hostent *hp;
    size_t namelen;

    if (!sa || sa->sa_family != AF_INET)
        return EAI_FAMILY;

    hp = gethostbyaddr((const char *)&((const struct sockaddr_in *)sa)->sin_addr,
                       4, AF_INET);
    if (!hp)
        return (h_errno == TRY_AGAIN) ? EAI_AGAIN : EAI_FAIL;

    if (host && hostlen > 0) {
        namelen = strlen(hp->h_name);
        if (namelen >= hostlen) namelen = hostlen - 1;
        strncpy(host, hp->h_name, namelen);
        host[namelen] = '\0';
    }
    return 0;
}

#endif /* COMPAT_DNS_RFC2553_COMPAT_H */
