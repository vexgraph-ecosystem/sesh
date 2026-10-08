#ifndef SESH_SNAPSHOT_H
#define SESH_SNAPSHOT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "storage/snapshot_io.h"
#include "annotation/what.h"

#define SESH_SNAPSHOT_RETRY_DELAY_DEFAULT UINT64_C(1000)
#define SESH_SNAPSHOT_MAX_ATTEMPTS_DEFAULT UINT32_C(3)
#define SESH_SNAPSHOT_IDLE 0
#define SESH_SNAPSHOT_QUEUED 1
#define SESH_SNAPSHOT_SAVED 2
#define SESH_SNAPSHOT_FAILED 3
#define SESH_SNAPSHOT_CANCELLED 4

typedef struct SeshSnapshot {
    uint8_t *buffer;
    size_t capacity;
    size_t length;
    HavenSnapshotPutFn put;
    ;;WHAT("provider-owned opaque snapshot transport context")
    void *context;
    uint64_t key;
    uint64_t nextAttemptMs;
    uint64_t retryDelayMs;
    uint32_t attempts;
    uint32_t maxAttempts;
    int state;
} SeshSnapshot;

/* Cold, externally serialized API. No allocation, clocks, files or sockets.
 * Struct is caller-owned; fields are read-only to consumers. Buffer/context
 * remain live until cancel/completion. Copying a queued object is forbidden.
 * Step clocks are caller-supplied monotonic milliseconds; no sleeping.
 * Setters configure idle/completed objects; queue owns the remaining fields.
 * Invalid admission preserves state. Empty snapshots are valid. There is one
 * active snapshot, not a queue ceiling: busy admission explicitly rejects.
 */
SeshSnapshot SeshSnapshot_0(void);
#define SeshSnapshot(...) SeshSnapshot_0(__VA_ARGS__)
#define SeshSnapshot_zero() SeshSnapshot_0()
bool SeshSnapshot_setBuffer(SeshSnapshot *self, uint8_t *buffer, size_t capacity);
bool SeshSnapshot_setProvider(SeshSnapshot *self, HavenSnapshotPutFn put, void *context);
bool SeshSnapshot_setRetryPolicy(SeshSnapshot *self, uint64_t delayMs, uint32_t maxAttempts);
bool SeshSnapshot_queue(SeshSnapshot *self, uint64_t key, const uint8_t *bytes, size_t length);
int SeshSnapshot_step(SeshSnapshot *self, uint64_t nowMs);
/* Quiet bounded teardown: cancel is idempotent and releases provider borrows. */
void SeshSnapshot_cancel(SeshSnapshot *self);
const uint8_t *SeshSnapshot_getBuffer(const SeshSnapshot *self);
size_t SeshSnapshot_getCapacity(const SeshSnapshot *self);
size_t SeshSnapshot_getLength(const SeshSnapshot *self);
HavenSnapshotPutFn SeshSnapshot_getProvider(const SeshSnapshot *self);
void *SeshSnapshot_getContext(const SeshSnapshot *self);
uint64_t SeshSnapshot_getKey(const SeshSnapshot *self);
uint64_t SeshSnapshot_getNextAttemptMs(const SeshSnapshot *self);
uint64_t SeshSnapshot_getRetryDelayMs(const SeshSnapshot *self);
uint32_t SeshSnapshot_getAttempts(const SeshSnapshot *self);
uint32_t SeshSnapshot_getMaxAttempts(const SeshSnapshot *self);
int SeshSnapshot_getState(const SeshSnapshot *self);
bool SeshSnapshot_toString(const SeshSnapshot *self, char *dest, size_t cap, bool *outTruncated);
bool SeshSnapshot_toStringStruct(const SeshSnapshot *self, char *dest, size_t cap, bool *outTruncated);

#endif
