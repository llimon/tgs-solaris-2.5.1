#ifndef MY_UTIMES_H
#define MY_UTIMES_H

#include <utime.h>
#include <sys/time.h> // For struct timeval
#include <errno.h>

// This function provides a custom implementation of utimes() for compatibility
// with older systems that only provide the second-precision utime().
// It sacrifices microsecond precision.
static inline int utimes(const char *path, const struct timeval times[2]) {
    struct utimbuf new_times;

    // Case 1: If `times` is NULL, set the file's times to the current time.
    if (times == NULL) {
        return utime(path, NULL);
    }

    // Case 2: If `times` is not NULL, convert and apply the specified times.
    // NOTE: Microsecond precision is lost here as utime() only supports seconds.
    new_times.actime = times[0].tv_sec; // Access time (atime)
    new_times.modtime = times[1].tv_sec; // Modification time (mtime)

    return utime(path, &new_times);
}

#endif // MY_UTIMES_H
