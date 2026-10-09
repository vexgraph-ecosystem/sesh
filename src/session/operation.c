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
// Returns an empty operation value with all identity fields unset.
Operation Operation_0(void) { return (Operation) {0}; }
// Constructs an operation when all required scope IDs are nonzero.
Operation Operation_5(uint64_t id, uint64_t clientId, uint64_t workspaceId, uint64_t resourceId, uint64_t expectedRevision) {
    Operation result = {id, clientId, workspaceId, resourceId, expectedRevision};
    if (!Operation_isValid(&result)) {
        THROW("operation identity: zero ID");
        return Operation_0();
    }
    return result;
}
// Generates a validated nonzero-identity setter for each required scope ID.
#define OPERATION_SETTER(Name, field) \
    bool Operation_set##Name(Operation *self, uint64_t value) { \
        if (self == nullptr || value == 0) { \
            THROW("operation identity: null or zero ID"); \
            return false; \
        } \
        (*self).field = value; \
        return true; \
    }
// Sets the operation's own nonzero id.
OPERATION_SETTER(Id, id)
// Sets the originating client's nonzero id.
OPERATION_SETTER(ClientId, clientId)
// Sets the workspace's nonzero scope id.
OPERATION_SETTER(WorkspaceId, workspaceId)
// Sets the target resource's nonzero id.
OPERATION_SETTER(ResourceId, resourceId)
#undef OPERATION_SETTER
// Sets the expected revision; zero is valid and null self is rejected.
bool Operation_setExpectedRevision(Operation *self, uint64_t expectedRevision) {
    if (self == nullptr) {
        THROW("operation revision: null operation");
        return false;
    }
    (*self).expectedRevision = expectedRevision;
    return true;
}
// Returns the operation ID, or zero for a null value.
SESH_GETTER(Operation, uint64_t, Id, id, 0)
// Returns the originating client ID, or zero for a null value.
SESH_GETTER(Operation, uint64_t, ClientId, clientId, 0)
// Returns the workspace scope ID, or zero for a null value.
SESH_GETTER(Operation, uint64_t, WorkspaceId, workspaceId, 0)
// Returns the target resource ID, or zero for a null value.
SESH_GETTER(Operation, uint64_t, ResourceId, resourceId, 0)
// Returns the expected local revision, or zero for a null value.
SESH_GETTER(Operation, uint64_t, ExpectedRevision, expectedRevision, 0)
// Checks that all required identity and scope IDs are nonzero.
bool Operation_isValid(const Operation *self) {
    return self != nullptr && (*self).id != 0 && (*self).clientId != 0 &&
        (*self).workspaceId != 0 && (*self).resourceId != 0;
}
// Compares all identity, scope, and expected-revision fields.
bool Operation_equals(const Operation *left, const Operation *right) {
    return left != nullptr && right != nullptr && (*left).id == (*right).id &&
        (*left).clientId == (*right).clientId && (*left).workspaceId == (*right).workspaceId &&
        (*left).resourceId == (*right).resourceId && (*left).expectedRevision == (*right).expectedRevision;
}
// Formats client/operation identity as a bounded value summary.
SESH_VALUE_STRING(Operation, SeshText_format(dest, cap, outTruncated, "operation %llu/%llu", (unsigned long long) (*self).clientId, (unsigned long long) (*self).id))
// Formats all operation fields into a bounded structure summary.
bool Operation_toStringStruct(const Operation *self, char *dest, size_t cap, bool *outTruncated) {
    return self == nullptr ? SeshText_format(dest, cap, outTruncated, "nullptr") :
        SeshText_format(dest, cap, outTruncated, "Operation{id=%llu,clientId=%llu,workspaceId=%llu,resourceId=%llu,expectedRevision=%llu}",
                        (unsigned long long) (*self).id, (unsigned long long) (*self).clientId,
                        (unsigned long long) (*self).workspaceId, (unsigned long long) (*self).resourceId,
                        (unsigned long long) (*self).expectedRevision);
}
