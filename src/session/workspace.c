#include "lang/workspace.h"
#include "session/text.h"
#include "annotation/definition.h"
#include "annotation/overview.h"
;;DEFINITION
/* Workspace scopes resources to one authenticated principal in this first
 * private-workspace slice. Host creates/persists identities; no remote ACL proof.
 */
;;OVERVIEW
/* CLASS: Workspace. STRUCT FIELDS: uint64_t id: project namespace;
 * uint64_t principalId: private owner. Both zero in empty identity.
 * PUBLIC: _0/_2/chooser/zero; setId/setPrincipalId (reject zero/null, unchanged);
 * getId/getPrincipalId (null=0); toString/toStringStruct. No private helpers.
 * Mutations require external serialization with all Sesh borrowers.
 */
// Returns an empty workspace with no identity or admitted principal.
Workspace Workspace_0(void) { return (Workspace) {0}; }
// Constructs a workspace for a nonzero identity and principal.
Workspace Workspace_2(uint64_t id, uint64_t principalId) {
    Workspace result = Workspace_0();
    if (id == 0 || principalId == 0) {
        THROW("workspace identity: zero ID");
        return result;
    }
    result.id = id;
    result.principalId = principalId;
    return result;
}
// Sets a nonzero workspace ID; invalid input preserves the previous value.
bool Workspace_setId(Workspace *self, uint64_t id) {
    if (self == nullptr || id == 0) {
        THROW("workspace identity: null or zero ID");
        return false;
    }
    (*self).id = id;
    return true;
}
// Sets a nonzero principal ID; invalid input preserves the previous value.
bool Workspace_setPrincipalId(Workspace *self, uint64_t principalId) {
    if (self == nullptr || principalId == 0) {
        THROW("workspace principal: null or zero ID");
        return false;
    }
    (*self).principalId = principalId;
    return true;
}
// Returns the workspace ID, or zero for null.
SESH_GETTER(Workspace, uint64_t, Id, id, 0)
// Returns the owning principal ID, or zero for null.
SESH_GETTER(Workspace, uint64_t, PrincipalId, principalId, 0)
// Formats the workspace ID as a bounded value summary.
SESH_VALUE_STRING(Workspace, SeshText_format(dest, cap, outTruncated, "workspace %llu", (unsigned long long) (*self).id))
// Formats both identity fields into a bounded structure summary.
bool Workspace_toStringStruct(const Workspace *self, char *dest, size_t cap, bool *outTruncated) {
    return self == nullptr ? SeshText_format(dest, cap, outTruncated, "nullptr") :
        SeshText_format(dest, cap, outTruncated, "Workspace{id=%llu,principalId=%llu}",
                        (unsigned long long) (*self).id, (unsigned long long) (*self).principalId);
}
