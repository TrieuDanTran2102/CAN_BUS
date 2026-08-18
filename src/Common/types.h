#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <stdint.h>
#include <stdbool.h>

/* Generic result/status code used across drivers & middleware */
typedef enum {
    STATUS_OK = 0,
    STATUS_ERROR = -1,
    STATUS_TIMEOUT = -2,
    STATUS_BUSY = -3,
    STATUS_INVALID_PARAM = -4,
    STATUS_NOT_READY = -5,
} status_t;

/* Millisecond timestamp type, used everywhere time-sync is involved */
typedef uint64_t timestamp_ms_t;

#endif /* COMMON_TYPES_H */
