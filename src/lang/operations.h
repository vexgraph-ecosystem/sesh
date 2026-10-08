#ifndef SESH_LANG_OPERATIONS_H
#define SESH_LANG_OPERATIONS_H
#include "lang/operation.h"
#define OPERATIONS_REJECTED 0
#define OPERATIONS_ADDED 1
#define OPERATIONS_REPLAY 2
#define OPERATIONS_CONFLICT 3
/* Growable flat ledger in host-owned storage. reserve copies existing records
 * into larger caller storage; caller releases old storage afterwards. No indices
 * or borrowed row pointers escape: find copies out by (clientId,id).
 * add snapshots the entire intent, rejects identity reuse with changed content.
 * Storage and this object require external serialization; no concurrent CAS or
 * persistence. Caller excludes API use before releasing borrowed backing memory.
 * Consumers never mutate backing rows; find dest must be disjoint from them.
 */
/* SLOT RECORD: receipt marker is ledger-owned, not caller Operation intent. */
typedef struct OperationsSlot { Operation operation; bool applied; } OperationsSlot;
typedef struct Operations { OperationsSlot *items; size_t count; size_t capacity; } Operations;
Operations Operations_0(void);
Operations Operations_2(OperationsSlot *items, size_t capacity);
#define Operations(...) SESH_CONSTRUCT(Operations, __VA_ARGS__)
#define Operations_zero() Operations_0()
bool Operations_reserve(Operations *self, OperationsSlot *items, size_t capacity);
int Operations_add(Operations *self, const Operation *operation);
bool Operations_find(const Operations *self, uint64_t clientId, uint64_t id, Operation *dest);
bool Operations_isApplied(const Operations *self, uint64_t clientId, uint64_t id);
/* Applies an already-admitted intent to local revision and records its receipt.
 * No data bytes or remote state are changed. Conflict returns 3 without mutation.
 */
int Operations_apply(Operations *self, const Operation *operation, Resource *resource);
size_t Operations_getCount(const Operations *self);
size_t Operations_getCapacity(const Operations *self);
void Operations_clear(Operations *self);
bool Operations_toString(const Operations *self, char *dest, size_t cap, bool *outTruncated);
bool Operations_toStringStruct(const Operations *self, char *dest, size_t cap, bool *outTruncated);
#endif
