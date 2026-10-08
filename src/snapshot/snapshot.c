#include "snapshot/snapshot.h"
#include "annotation/definition.h"
#include "annotation/overview.h"
#include "annotation/intention.h"
#include "exception/throw.h"
#include <stdio.h>
#include <string.h>

;;DEFINITION
/* SeshSnapshot turns explicitly supplied bytes into one immutable upload job.
 * It copies input into borrowed caller storage before admission, then retains
 * that exact key/content across pending steps and retries. API Haven supplies
 * provider vocabulary; R1 supplies execution, transport and resource lifetimes.
 * No engine allocation or filesystem dependency is needed to prove this core.
 * This is backup scheduling, not a durable local journal or collaborative sync.
 * The provider must deduplicate keys; Drive mapping/OAuth are not implemented.
 */
;;OVERVIEW
/* CLASS: SeshSnapshot
 * STRUCT FIELDS (header order):
 * uint8_t *buffer: borrowed staging bytes; size_t capacity: admitted storage size;
 * size_t length: immutable queued byte count; HavenSnapshotPutFn put: provider;
 * void *context: borrowed provider context; uint64_t key: unique snapshot identity;
 * uint64_t nextAttemptMs: earliest retry; uint64_t retryDelayMs: configurable delay;
 * uint32_t attempts: RETRY outcomes; uint32_t maxAttempts: retry exhaustion bound;
 * int state: idle/queued/saved/failed/cancelled.
 * PUBLIC: _0/zero; setBuffer/setProvider/setRetryPolicy (reject while queued);
 * queue (reject busy, zero key, null nonempty bytes, oversized input);
 * step (pending/retry/durable done/terminal reject); cancel (quiet idempotent);
 * getBuffer/Capacity/Length/Provider/Context/Key/NextAttemptMs/RetryDelayMs/
 * Attempts/MaxAttempts/State; toString/toStringStruct (bounded cold projections).
 * PRIVATE: configurable validates cold mutations; stringify formats projections.
 * State-derived fields are
 * changed through queue/step/cancel only, never unconstrained scalar setters.
 * No private helper classes. No owned resources. No concurrency claim.
 */
;;INTENTION("per the Single Class Per File Law (Java Law) + Conflict Triage Law: configuration has validated setters; job-derived fields mutate only through queue/step/cancel so callers cannot forge acknowledgement or break provider borrows")

static bool configurable(SeshSnapshot *self) {
    if (self == nullptr || (*self).state == SESH_SNAPSHOT_QUEUED) {
        THROW("snapshot configuration: null or active job");
        return false;
    }
    return true;
}

SeshSnapshot SeshSnapshot_0(void) {
    return (SeshSnapshot) { .retryDelayMs = SESH_SNAPSHOT_RETRY_DELAY_DEFAULT,
                           .maxAttempts = SESH_SNAPSHOT_MAX_ATTEMPTS_DEFAULT };
}

bool SeshSnapshot_setBuffer(SeshSnapshot *self, uint8_t *buffer, size_t capacity) {
    if (!configurable(self))
        return false;
    if (buffer == nullptr || capacity == 0) {
        THROW("snapshot buffer: missing storage");
        return false;
    }
    (*self).buffer = buffer;
    (*self).capacity = capacity;
    (*self).length = 0;
    return true;
}

bool SeshSnapshot_setProvider(SeshSnapshot *self, HavenSnapshotPutFn put, void *context) {
    if (!configurable(self))
        return false;
    if (put == nullptr) {
        THROW("snapshot provider: missing callback");
        return false;
    }
    (*self).put = put;
    (*self).context = context;
    return true;
}

bool SeshSnapshot_setRetryPolicy(SeshSnapshot *self, uint64_t delayMs, uint32_t maxAttempts) {
    if (!configurable(self))
        return false;
    if (delayMs == 0 || maxAttempts == 0) {
        THROW("snapshot retry policy: zero delay or attempts");
        return false;
    }
    (*self).retryDelayMs = delayMs;
    (*self).maxAttempts = maxAttempts;
    return true;
}

bool SeshSnapshot_queue(SeshSnapshot *self, uint64_t key, const uint8_t *bytes, size_t length) {
    if (self == nullptr || key == 0 || (bytes == nullptr && length != 0)) {
        THROW("snapshot queue: invalid input");
        return false;
    }
    if ((*self).state == SESH_SNAPSHOT_QUEUED || (*self).buffer == nullptr ||
        (*self).put == nullptr || length > (*self).capacity) {
        THROW("snapshot queue: busy, unconfigured or oversized");
        return false;
    }
    if (length != 0)
        memmove((*self).buffer, bytes, length);
    (*self).length = length;
    (*self).key = key;
    (*self).attempts = 0;
    (*self).nextAttemptMs = 0;
    (*self).state = SESH_SNAPSHOT_QUEUED;
    return true;
}

int SeshSnapshot_step(SeshSnapshot *self, uint64_t nowMs) {
    if (self == nullptr) {
        THROW("snapshot step: null job");
        return SESH_SNAPSHOT_FAILED;
    }
    if ((*self).state != SESH_SNAPSHOT_QUEUED || nowMs < (*self).nextAttemptMs)
        return (*self).state;
    int result = (*self).put((*self).context, (*self).key, (*self).buffer, (*self).length, 0);
    if (result == HAVEN_SNAPSHOT_DONE)
        (*self).state = SESH_SNAPSHOT_SAVED;
    else if (result == HAVEN_SNAPSHOT_RETRY) {
        (*self).attempts++;
        if ((*self).attempts >= (*self).maxAttempts || nowMs > UINT64_MAX - (*self).retryDelayMs) {
            SeshSnapshot_cancel(self);
            (*self).state = SESH_SNAPSHOT_FAILED;
            THROW("snapshot step: retry exhausted or clock overflow");
        } else
            (*self).nextAttemptMs = nowMs + (*self).retryDelayMs;
    } else if (result != HAVEN_SNAPSHOT_PENDING) {
        SeshSnapshot_cancel(self);
        (*self).state = SESH_SNAPSHOT_FAILED;
        THROW("snapshot step: provider rejected snapshot");
    }
    return (*self).state;
}

void SeshSnapshot_cancel(SeshSnapshot *self) {
    if (self == nullptr || (*self).state != SESH_SNAPSHOT_QUEUED)
        return;
    (*self).put((*self).context, (*self).key, (*self).buffer, (*self).length, 1);
    (*self).state = SESH_SNAPSHOT_CANCELLED;
}

#define SNAPSHOT_GETTER(type, Name, field, fallback) \
    type SeshSnapshot_get##Name(const SeshSnapshot *self) { \
        if (self == nullptr) \
            return fallback; \
        return (*self).field; \
    }
SNAPSHOT_GETTER(const uint8_t *, Buffer, buffer, nullptr)
SNAPSHOT_GETTER(size_t, Capacity, capacity, 0)
SNAPSHOT_GETTER(size_t, Length, length, 0)
SNAPSHOT_GETTER(HavenSnapshotPutFn, Provider, put, nullptr)
SNAPSHOT_GETTER(void *, Context, context, nullptr)
SNAPSHOT_GETTER(uint64_t, Key, key, 0)
SNAPSHOT_GETTER(uint64_t, NextAttemptMs, nextAttemptMs, 0)
SNAPSHOT_GETTER(uint64_t, RetryDelayMs, retryDelayMs, 0)
SNAPSHOT_GETTER(uint32_t, Attempts, attempts, 0)
SNAPSHOT_GETTER(uint32_t, MaxAttempts, maxAttempts, 0)
SNAPSHOT_GETTER(int, State, state, SESH_SNAPSHOT_IDLE)
#undef SNAPSHOT_GETTER

static bool stringify(const SeshSnapshot *self, bool structure, char *dest, size_t cap, bool *outTruncated) {
    if (outTruncated != nullptr)
        *outTruncated = false;
    if (dest == nullptr || cap == 0) {
        if (outTruncated != nullptr)
            *outTruncated = true;
        THROW("snapshot string: missing destination");
        return false;
    }
    int count;
    if (self == nullptr)
        count = snprintf(dest, cap, "nullptr");
    else if (structure)
        count = snprintf(dest, cap, "SeshSnapshot{buffer=%p,capacity=%zu,length=%zu,put=%s,context=%p,key=%llu,nextAttemptMs=%llu,retryDelayMs=%llu,attempts=%u,maxAttempts=%u,state=%d}",
                         (void*) (*self).buffer, (*self).capacity, (*self).length,
                         (*self).put == nullptr ? "nullptr" : "bound", (*self).context,
                         (unsigned long long) (*self).key, (unsigned long long) (*self).nextAttemptMs,
                         (unsigned long long) (*self).retryDelayMs, (*self).attempts, (*self).maxAttempts, (*self).state);
    else
        count = snprintf(dest, cap, "snapshot %llu: %zu bytes, state %d",
                         (unsigned long long) (*self).key, (*self).length, (*self).state);
    if (count < 0 || (size_t) count >= cap) {
        if (outTruncated != nullptr)
            *outTruncated = true;
        THROW("snapshot string: truncated");
        return false;
    }
    return true;
}

bool SeshSnapshot_toString(const SeshSnapshot *self, char *dest, size_t cap, bool *outTruncated) {
    return stringify(self, false, dest, cap, outTruncated);
}

bool SeshSnapshot_toStringStruct(const SeshSnapshot *self, char *dest, size_t cap, bool *outTruncated) {
    return stringify(self, true, dest, cap, outTruncated);
}
