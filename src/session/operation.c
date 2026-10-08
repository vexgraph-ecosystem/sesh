#include "lang/operation.h"
#include "session/text.h"
#include "annotation/definition.h"
#include "annotation/overview.h"
;;DEFINITION
/* Operation carries a client-scoped idempotency identity and workspace/resource
 * scope plus expected local revision. It owns no payload or identity storage.
 * A ledger copies it, so later edits to this caller value cannot alter history.
 */
;;OVERVIEW
/* CLASS: Operation. STRUCT FIELDS: uint64_t id: client-local unique operation;
 * uint64_t clientId: origin; uint64_t workspaceId: namespace; uint64_t resourceId:
 * target; uint64_t expectedRevision: optimistic local baseline.
 * PUBLIC: _0/_5/chooser/zero; setId/ClientId/WorkspaceId/ResourceId (null/zero reject);
 * setExpectedRevision (null rejects, zero/max representable); corresponding getters
 * (null=0); isValid (nonzero identities); equals (null=false); bounded projections.
 * No private helpers. Setters preserve rejection state; external serialization.
 */
Operation Operation_0(void) { return (Operation) {0}; }
Operation Operation_5(uint64_t id, uint64_t clientId, uint64_t workspaceId, uint64_t resourceId, uint64_t expectedRevision) {
    Operation result = {id, clientId, workspaceId, resourceId, expectedRevision};
    if (!Operation_isValid(&result)) {
        THROW("operation identity: zero ID");
        return Operation_0();
    }
    return result;
}
#define OPERATION_SETTER(Name, field) \
    bool Operation_set##Name(Operation *self, uint64_t value) { \
        if (self == nullptr || value == 0) { \
            THROW("operation identity: null or zero ID"); \
            return false; \
        } \
        (*self).field = value; \
        return true; \
    }
OPERATION_SETTER(Id, id)
OPERATION_SETTER(ClientId, clientId)
OPERATION_SETTER(WorkspaceId, workspaceId)
OPERATION_SETTER(ResourceId, resourceId)
#undef OPERATION_SETTER
bool Operation_setExpectedRevision(Operation *self, uint64_t expectedRevision) {
    if (self == nullptr) {
        THROW("operation revision: null operation");
        return false;
    }
    (*self).expectedRevision = expectedRevision;
    return true;
}
SESH_GETTER(Operation, uint64_t, Id, id, 0)
SESH_GETTER(Operation, uint64_t, ClientId, clientId, 0)
SESH_GETTER(Operation, uint64_t, WorkspaceId, workspaceId, 0)
SESH_GETTER(Operation, uint64_t, ResourceId, resourceId, 0)
SESH_GETTER(Operation, uint64_t, ExpectedRevision, expectedRevision, 0)
bool Operation_isValid(const Operation *self) {
    return self != nullptr && (*self).id != 0 && (*self).clientId != 0 &&
        (*self).workspaceId != 0 && (*self).resourceId != 0;
}
bool Operation_equals(const Operation *left, const Operation *right) {
    return left != nullptr && right != nullptr && (*left).id == (*right).id &&
        (*left).clientId == (*right).clientId && (*left).workspaceId == (*right).workspaceId &&
        (*left).resourceId == (*right).resourceId && (*left).expectedRevision == (*right).expectedRevision;
}
SESH_VALUE_STRING(Operation, SeshText_format(dest, cap, outTruncated, "operation %llu/%llu", (unsigned long long) (*self).clientId, (unsigned long long) (*self).id))
bool Operation_toStringStruct(const Operation *self, char *dest, size_t cap, bool *outTruncated) {
    return self == nullptr ? SeshText_format(dest, cap, outTruncated, "nullptr") :
        SeshText_format(dest, cap, outTruncated, "Operation{id=%llu,clientId=%llu,workspaceId=%llu,resourceId=%llu,expectedRevision=%llu}",
                        (unsigned long long) (*self).id, (unsigned long long) (*self).clientId,
                        (unsigned long long) (*self).workspaceId, (unsigned long long) (*self).resourceId,
                        (unsigned long long) (*self).expectedRevision);
}
