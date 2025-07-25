#pragma once

using namespace std;

// === Logging Macros ===
// Logging enabled by default
#ifndef SFE_LOGGING
#define SFE_LOGGING 1
#endif

#if SFE_LOGGING
#include <iostream>
<<<<<<< HEAD
#define SFE_LOG_ERROR(message) cerr << "[SFE ERROR] " << (message) << endl
#define SFE_LOG_WARN(message) cerr << "[SFE WARN] " << (message) << endl
#define SFE_LOG_INFO(message) cout << "[SFE INFO] " << (message) << endl
    == == ==
    =
#define SFE_LOG_ERROR(message) cerr << "[SFE][ERROR] " << message << endl
#define SFE_LOG_WARN(message) cerr << "[SFE][WARN] " << message << endl
#define SFE_LOG_INFO(message) cout << "[SFE][INFO] " << message << endl
        >>>>>>> 8a1a6043ba7584453955ae0b8074130be688ce1c

#define SFE_LOG_WARN_IF(condition, message)   \
    do {                                      \
        if (condition) SFE_LOG_WARN(message); \
    } while (0)
#else
#define SFE_LOG_ERROR(message)
#define SFE_LOG_WARN(message)
#define SFE_LOG_INFO(message)
#define SFE_LOG_WARN_IF(condition, message)
#endif

// === Exceptions Macros ===
// Exceptions enabled by default
#ifndef SFE_EXCEPTIONS
#define SFE_EXCEPTIONS 1
#endif

#if SFE_EXCEPTIONS
#include <stdexcept>
#define SFE_THROW(ExceptionType, message) \
    do {                                  \
        SFE_LOG_ERROR(message);           \
        throw ExceptionType(message);     \
                                          \
    } while (0)

#define SFE_THROW_IF(condition, ExceptionType, message) \
    do {                                                \
        if (condition) {                                \
            SFE_LOG_ERROR(message);                     \
            throw ExceptionType(message);               \
        }                                               \
    } while (0)

#define SFE_THROW_OR_RETURN_IF(condition, ExceptionType, message, retval) \
    SFE_THROW_IF(condition, ExceptionType, message)
#else
#define SFE_THROW(ExceptionType, message) SFE_LOG_WARN(message)
#define SFE_THROW_IF(condition, ExceptionType, message) SFE_LOG_WARN_IF(condition, message)
#define SFE_THROW_OR_RETURN_IF(condition, ExceptionType, message, retval) \
    do {                                                                  \
        if (condition) {                                                  \
            SFE_LOG_WARN(message)                                         \
            return retval;                                                \
        }                                                                 \
    } while (0)
#endif
