#ifndef LOG_H
#define LOG_H

#ifdef ENABLE_LOGGING

#include <cstdio>

#define LOG_TRACE(format, ...)                              \
    do {                                                    \
        if (__log_level <= LOG_LEVEL_TRACE) {               \
            fprintf(stdout, "TRACE: " format, __VA_ARGS__); \
        }                                                   \
    } while (0)

#define LOG_DEBUG(format, ...)                              \
    do {                                                    \
        if (__log_level <= LOG_LEVEL_TRACE) {               \
            fprintf(stdout, "DEBUG: " format, __VA_ARGS__); \
        }                                                   \
    } while (0)


#else

#define LOG_TRACE(format, ...)
#define LOG_DEBUG(format, ...)

#endif // ENABLE_LOGGING

#endif // LOG_H
