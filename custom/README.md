Legacy SPARC Extensions
----------------------

# LSEcmph — C99 & POSIX.1-2001 Compatibility Header Suite

**LSEcmph** (*Legacy SPARC Extensions Compatibility Headers*) is a lightweight, zero-dependency header-only shim layer created to bring ISO C99 standard library features and POSIX.1-2001 system interfaces to **SunOS 5.5.1 (Solaris 2.5.1)** running on SPARC architectures.

When porting modern C software (such as Git, Vim, or core GNU utilities) to vintage UNIX systems, missing C99 types (`uint32_t`, `intmax_t`), missing string parsers (`strtoimax`), or missing socket constants (`SHUT_WR`) frequently break compilation. **LSEcmph** resolves these deficits cleanly at the header level using `static inline` functions and preprocessor guards, allowing modern source trees to compile cleanly with GCC without requiring heavy external runtime libraries or invasive source modifications.

---

## Features & Header Index

### Core Standards Shims (`/usr/tgcware/include/`)

* **`stdint.h`**
  Provides standard fixed-width integer types (`int8_t`, `uint32_t`, `uint64_t`, `uintmax_t`, `intmax_t`), along with exact limit macros (`INTMAX_MAX`, `UINTMAX_MAX`).
* **`inttypes.h`**
  Includes standard format specifiers (`PRIdMAX`, `PRIuMAX`, `PRIxMAX`, `SCNuMAX`) and delegates string-to-integer conversions to `compat/strtoimax_compat.h`.

### Subsystem Compatibility Headers (`/usr/tgcware/include/compat/`)

| Header | Description | Embedded Test Flag |
| :--- | :--- | :--- |
| **`strtoimax_compat.h`** | Inline C99 64-bit string-to-integer parsers (`strtoimax`, `strtoumax`) featuring base-36 support, auto-base detection (hex/octal), and `ERANGE` bounds checking. | `_TEST_STRTOIMAX_CUSTOM` |
| **`math_compat.h`** | Type-generic C99 floating-point classification macro (`isinf`) implemented via IEEE 754 bit-pattern inspection for float and double types on SPARC big-endian hardware. | `_TEST_MATH_COMPAT_CUSTOM` |
| **`dns_rfc2553_compat.h`** | Modern RFC 2553 name resolution structure shims (`struct addrinfo`, `getaddrinfo`, `freeaddrinfo`, `gai_strerror`, `getnameinfo`) with multi-IP pool support. | `_TEST_DNS_RFC2553_COMPAT` |
| **`ftello_compat.h`** | POSIX 64-bit file stream offset functions (`ftello`, `fseeko`) wrapping native Solaris `llseek(2)`. Verified for file offsets across the 2 GB boundary on UFS. | `_TEST_FTELLO_COMPAT` |
| **`socket_compat.h`** | POSIX socket definitions: `shutdown(2)` flags (`SHUT_RD`, `SHUT_WR`, `SHUT_RDWR`), socket type aliases (`socklen_t`, `in_addr_t`), and IP buffer constants (`INET_ADDRSTRLEN`, `INET6_ADDRSTRLEN`). | — |
| **`utimes_compat.h`** | Microsecond-resolution file timestamp modifier shims (`utimes`). | — |
| **`mbstowcs_compat.h`** | Multi-byte to wide-character string conversion utilities (`mbstowcs`). | — |
| **`wcsstr_compat.h`** | Wide-character substring search helper (`wcsstr`). | — |
| **`usleep_compat.h`** | Sub-second execution pause fallback using POSIX interval timers. | — |

---

## Directory Layout

When installed as part of the TGCware / LSE build hierarchy, files are arranged as follows:

```text
/usr/tgcware/include/
├── inttypes.h
├── stdint.h
└── compat/
    ├── dns_rfc2553_compat.h
    ├── ftello_compat.h
    ├── math_compat.h
    ├── mbstowcs_compat.h
    ├── socket_compat.h
    ├── strtoimax_compat.h
    ├── usleep_compat.h
    ├── utimes_compat.h
    └── wcsstr_compat.h
