#ifndef MY_USLEEP_H
#define MY_USLEEP_H

#include <time.h>
#include <errno.h>

// Use a compiler-agnostic way to define an inline function
#if defined(__GNUC__) || defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
static inline int usleep_wrapper(long microseconds) {
#else
static int usleep_wrapper(long microseconds) {
#endif
    struct timespec requested_time;
    struct timespec remaining_time;
    int ret;

    if (microseconds < 0) {
        errno = EINVAL;
        return -1;
    }

    requested_time.tv_sec = microseconds / 1000000;
    requested_time.tv_nsec = (microseconds % 1000000) * 1000;

    // Use a loop to handle interruptions by signals
    while ((ret = nanosleep(&requested_time, &remaining_time)) == -1 && errno == EINTR) {
        requested_time = remaining_time;
    }

    return ret;
}

// Use a macro to map usleep to your wrapper.
// This is useful if you can't rename the function in your codebase.
#define usleep usleep_wrapper

#endif /* MY_USLEEP_H */
