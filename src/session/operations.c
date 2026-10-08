#include "lang/operations.h"
#include "session/text.h"
#include "annotation/definition.h"
#include "annotation/overview.h"
#include "annotation/intention.h"
#include <string.h>
;;DEFINITION
/* Operations is a flat copied-intent ledger with explicit host-driven growth.
 * Capacity is current borrowed storage, never a permanent entity ceiling. The
 * host reserves larger rows cold; failure preserves history and resource state.
 * Replays are idempotent only while history is retained. clear discards dedup
 * memory and requires exclusion/new identity epoch before sessions resume.
 */
;;OVERVIEW
/* CLASS: Operations. STRUCT FIELDS: OperationsSlot *items: borrowed contiguous rows;
 * size_t count: initialized rows; size_t capacity: current admitted row capacity.
 * PUBLIC: _0/_2/chooser/zero; reserve (reject null, zero, undersized, overflow);
 * add (copied admission, identical replay, changed-identity rejection, full storage);
 * find (copies output, absence unchanged/quiet, invalid input rejects);
 * isApplied (null/absent=false), apply (local revision+receipt, replay/conflict/reject);
 * getCount/getCapacity (null=0), clear (quiet, external exclusion required), projections.
 * SLOT RECORD: OperationsSlot: Operation operation: copied intent; bool applied:
 * receipt for one local revision advance (false until apply succeeds).
 * No private helpers. Count/items controlled through reserve/add/clear, not raw setters.
 */
;;INTENTION("per the Conflict Triage Law + Single Class Per File Law (Java Law): reserve/add/clear own backing/count updates; no scalar setter may forge deduplication history")
Operations Operations_0(void) { return (Operations) {0}; }
Operations Operations_2(OperationsSlot *items, size_t capacity) {
    Operations result = Operations_0();
    Operations_reserve(&result, items, capacity);
    return result;
}
bool Operations_reserve(Operations *self, OperationsSlot *items, size_t capacity) {
    if (self == nullptr || items == nullptr || capacity == 0 || capacity > SIZE_MAX / sizeof(OperationsSlot)) {
        THROW("operations reserve: invalid storage");
        return false;
    }
    if (capacity < (*self).capacity) {
        THROW("operations reserve: cannot shrink storage");
        return false;
    }
    if ((*self).count != 0)
        memmove(items, (*self).items, (*self).count * sizeof(OperationsSlot));
    (*self).items = items;
    (*self).capacity = capacity;
    return true;
}
bool Operations_find(const Operations *self, uint64_t clientId, uint64_t id, Operation *dest) {
    if (self == nullptr || dest == nullptr || clientId == 0 || id == 0) {
        THROW("operations find: invalid input");
        return false;
    }
    for (size_t i = 0; i < (*self).count; i++) {
        const OperationsSlot *slot = &(*self).items[i];
        const Operation *row = &(*slot).operation;
        if (Operation_getClientId(row) == clientId && Operation_getId(row) == id) {
            *dest = *row;
            return true;
        }
    }
    return false;
}
int Operations_add(Operations *self, const Operation *operation) {
    if (self == nullptr || !Operation_isValid(operation)) {
        THROW("operations add: invalid operation");
        return OPERATIONS_REJECTED;
    }
    Operation previous;
    if (Operations_find(self, Operation_getClientId(operation), Operation_getId(operation), &previous)) {
        if (Operation_equals(&previous, operation))
            return OPERATIONS_REPLAY;
        THROW("operations add: identity reused with changed intent");
        return OPERATIONS_REJECTED;
    }
    if ((*self).count == (*self).capacity) {
        THROW("operations add: reserve larger storage");
        return OPERATIONS_REJECTED;
    }
    OperationsSlot *slot = &(*self).items[(*self).count];
    (*slot).operation = *operation;
    (*slot).applied = false;
    (*self).count++;
    return OPERATIONS_ADDED;
}
bool Operations_isApplied(const Operations *self, uint64_t clientId, uint64_t id) {
    if (self == nullptr)
        return false;
    for (size_t i = 0; i < (*self).count; i++) {
        const OperationsSlot *slot = &(*self).items[i];
        const Operation *row = &(*slot).operation;
        if (Operation_getClientId(row) == clientId && Operation_getId(row) == id)
            return (*slot).applied;
    }
    return false;
}
int Operations_apply(Operations *self, const Operation *operation, Resource *resource) {
    if (self == nullptr || !Operation_isValid(operation) || resource == nullptr ||
        Operation_getResourceId(operation) != Resource_getId(resource) ||
        Operation_getWorkspaceId(operation) != Resource_getWorkspaceId(resource)) {
        THROW("operations apply: invalid scope");
        return OPERATIONS_REJECTED;
    }
    for (size_t i = 0; i < (*self).count; i++) {
        OperationsSlot *slot = &(*self).items[i];
        Operation *row = &(*slot).operation;
        if (Operation_getClientId(row) != Operation_getClientId(operation) || Operation_getId(row) != Operation_getId(operation))
            continue;
        if (!Operation_equals(row, operation)) {
            THROW("operations apply: changed intent");
            return OPERATIONS_REJECTED;
        }
        if ((*slot).applied)
            return OPERATIONS_REPLAY;
        uint64_t revision = Resource_getRevision(resource);
        if (revision != Operation_getExpectedRevision(operation)) {
            THROW("operations apply: revision conflict");
            return OPERATIONS_CONFLICT;
        }
        if (revision == UINT64_MAX) {
            THROW("operations apply: revision overflow");
            return OPERATIONS_REJECTED;
        }
        Resource_setRevision(resource, revision + UINT64_C(1));
        (*slot).applied = true;
        return OPERATIONS_ADDED;
    }
    THROW("operations apply: intent not admitted");
    return OPERATIONS_REJECTED;
}
SESH_GETTER(Operations, size_t, Count, count, 0)
SESH_GETTER(Operations, size_t, Capacity, capacity, 0)
void Operations_clear(Operations *self) {
    if (self != nullptr)
        (*self).count = 0;
}
SESH_VALUE_STRING(Operations, SeshText_format(dest, cap, outTruncated, "%zu operations", (*self).count))
bool Operations_toStringStruct(const Operations *self, char *dest, size_t cap, bool *outTruncated) {
    return self == nullptr ? SeshText_format(dest, cap, outTruncated, "nullptr") :
        SeshText_format(dest, cap, outTruncated, "Operations{items=%p,count=%zu,capacity=%zu}",
                        (void*) (*self).items, (*self).count, (*self).capacity);
}
