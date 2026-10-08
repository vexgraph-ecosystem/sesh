#include "lang/sesh.h"
#include "session/text.h"
#include "annotation/definition.h"
#include "annotation/overview.h"
;;DEFINITION
/* Sesh composes independently owned authentication, installation, workspace,
 * resource and operation-history objects. Cold submit validates that verified
 * principal, client and namespace agree, deduplicates immutable intent, rejects
 * stale revisions, and advances one local revision under external serialization.
 * It neither executes a network transfer nor owns database transactions. The
 * same model can later bind real providers without making local counters a
 * distributed atomicity claim. Closing merely detaches borrowed objects.
 */
;;OVERVIEW
/* CLASS: Sesh. STRUCT FIELDS: AuthService *authService: borrowed verified identity;
 * SeshClient *client: borrowed installation; Workspace *workspace: borrowed namespace;
 * Resource *resource: borrowed target/revision; Operations *operations: borrowed ledger.
 * PUBLIC: _0/_5/chooser/zero; setAuthService/Client/Workspace/Resource/Operations
 * (null rejects unchanged; binding alone admits no operation); matching getters
 * (null safe); submit (applied/replay/conflict/rejected); close (quiet/idempotent);
 * toString/toStringStruct (child VALUE projections, never credential dumps).
 * No private helpers. Every shared component requires external serialization.
 */
Sesh Sesh_0(void) { return (Sesh) {0}; }
Sesh Sesh_5(AuthService *authService, SeshClient *client, Workspace *workspace, Resource *resource, Operations *operations) {
    if (authService == nullptr || client == nullptr || workspace == nullptr || resource == nullptr || operations == nullptr) {
        THROW("sesh binding: missing component");
        return Sesh_0();
    }
    return (Sesh) {authService, client, workspace, resource, operations};
}
#define SESH_PART(Type, Name, field) \
    bool Sesh_set##Name(Sesh *self, Type *value) { \
        if (self == nullptr || value == nullptr) { \
            THROW("sesh binding: null component"); \
            return false; \
        } \
        (*self).field = value; \
        return true; \
    } \
    Type *Sesh_get##Name(const Sesh *self) { \
        if (self == nullptr) \
            return nullptr; \
        return (*self).field; \
    }
SESH_PART(AuthService, AuthService, authService)
SESH_PART(SeshClient, Client, client)
SESH_PART(Workspace, Workspace, workspace)
SESH_PART(Resource, Resource, resource)
SESH_PART(Operations, Operations, operations)
#undef SESH_PART
int Sesh_submit(Sesh *self, const Operation *operation) {
    if (self == nullptr || !Operation_isValid(operation)) {
        THROW("sesh submit: invalid operation");
        return SESH_REJECTED;
    }
    AuthService *auth = (*self).authService;
    SeshClient *client = (*self).client;
    Workspace *workspace = (*self).workspace;
    Resource *resource = (*self).resource;
    Operations *operations = (*self).operations;
    if (auth == nullptr || client == nullptr || workspace == nullptr || resource == nullptr || operations == nullptr ||
        !AuthService_isAuthenticated(auth) || AuthService_getPrincipalId(auth) != Workspace_getPrincipalId(workspace) ||
        Operation_getClientId(operation) != SeshClient_getId(client) ||
        Operation_getWorkspaceId(operation) != Workspace_getId(workspace) ||
        Resource_getWorkspaceId(resource) != Workspace_getId(workspace) ||
        Operation_getResourceId(operation) != Resource_getId(resource)) {
        THROW("sesh submit: unauthenticated or wrong identity scope");
        return SESH_REJECTED;
    }
    Operation previous;
    if (Operations_find(operations, Operation_getClientId(operation), Operation_getId(operation), &previous)) {
        if (!Operation_equals(operation, &previous)) {
            THROW("sesh submit: identity reused with changed intent");
            return SESH_REJECTED;
        }
        if (Operations_isApplied(operations, Operation_getClientId(operation), Operation_getId(operation)))
            return SESH_REPLAY;
    }
    uint64_t revision = Resource_getRevision(resource);
    if (Operation_getExpectedRevision(operation) != revision) {
        THROW("sesh submit: revision conflict");
        return SESH_CONFLICT;
    }
    if (revision == UINT64_MAX) {
        THROW("sesh submit: revision overflow");
        return SESH_REJECTED;
    }
    if (Operations_add(operations, operation) == OPERATIONS_REJECTED)
        return SESH_REJECTED;
    return Operations_apply(operations, operation, resource) == OPERATIONS_ADDED ? SESH_APPLIED : SESH_REJECTED;
}
void Sesh_close(Sesh *self) {
    if (self != nullptr)
        *self = Sesh_0();
}
SESH_VALUE_STRING(Sesh, SeshText_format(dest, cap, outTruncated, "sesh workspace %llu resource %llu", (unsigned long long) Workspace_getId((*self).workspace), (unsigned long long) Resource_getId((*self).resource)))
bool Sesh_toStringStruct(const Sesh *self, char *dest, size_t cap, bool *outTruncated) {
    if (self == nullptr)
        return SeshText_format(dest, cap, outTruncated, "nullptr");
    /* Safety bound: these child VALUE formats contain only bounded numeric IDs. */
    char auth[96], client[96], workspace[96], resource[96], operations[96];
    if (!AuthService_toString((*self).authService, auth, sizeof(auth), nullptr) ||
        !SeshClient_toString((*self).client, client, sizeof(client), nullptr) ||
        !Workspace_toString((*self).workspace, workspace, sizeof(workspace), nullptr) ||
        !Resource_toString((*self).resource, resource, sizeof(resource), nullptr) ||
        !Operations_toString((*self).operations, operations, sizeof(operations), nullptr))
        return false;
    return SeshText_format(dest, cap, outTruncated, "Sesh{authService=%s,client=%s,workspace=%s,resource=%s,operations=%s}",
                           auth, client, workspace, resource, operations);
}
