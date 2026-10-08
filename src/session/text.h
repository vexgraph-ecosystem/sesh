#ifndef SESH_SESSION_TEXT_H
#define SESH_SESSION_TEXT_H
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include "exception/throw.h"
/* Internal cold formatter: callers supply only metadata, never credentials.
 * Error leaves a terminated prefix when capacity is nonzero. No heap ownership.
 */
static inline bool SeshText_format(char *dest, size_t cap, bool *outTruncated, const char *format, ...) {
    if (outTruncated != nullptr)
        *outTruncated = false;
    if (dest == nullptr || cap == 0) {
        if (outTruncated != nullptr)
            *outTruncated = true;
        THROW("session text: missing destination");
        return false;
    }
    if (format == nullptr) {
        THROW("session text: missing format");
        return false;
    }
    va_list args;
    va_start(args, format);
    int count = vsnprintf(dest, cap, format, args);
    va_end(args);
    if (count < 0 || (size_t) count >= cap) {
        if (outTruncated != nullptr)
            *outTruncated = true;
        THROW("session text: truncated");
        return false;
    }
    return true;
}
#define SESH_VALUE_STRING(Class, Expression) \
    bool Class##_toString(const Class *self, char *dest, size_t cap, bool *outTruncated) { \
        if (self == nullptr) \
            return SeshText_format(dest, cap, outTruncated, "nullptr"); \
        return Expression; \
    }
#define SESH_GETTER(Class, Type, Name, field, fallback) \
    Type Class##_get##Name(const Class *self) { \
        if (self == nullptr) \
            return fallback; \
        return (*self).field; \
    }
#endif
