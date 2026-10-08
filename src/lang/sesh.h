#ifndef SESH_LANG_SESH_H
#define SESH_LANG_SESH_H
#include "lang/auth_service.h"
#include "lang/client.h"
#include "lang/workspace.h"
#include "lang/resource.h"
#include "lang/operations.h"
#define SESH_REJECTED 0
#define SESH_APPLIED 1
#define SESH_REPLAY 2
#define SESH_CONFLICT 3
/* Non-owning session composition. Every component, credentials, context and
 * ledger backing remains live until close/rebind, with all callers excluded.
 * Binding does not authenticate. Host drives AuthService verification separately.
 * submit is a local revision-intent admission, not upload/SQL execution. All
 * components/ledgers require ONE shared external serialization domain, including
 * distinct Sesh objects sharing a Resource. No distributed lock/transaction.
 */
typedef struct Sesh {
    AuthService *authService;
    SeshClient *client;
    Workspace *workspace;
    Resource *resource;
    Operations *operations;
} Sesh;
Sesh Sesh_0(void);
Sesh Sesh_5(AuthService *authService, SeshClient *client, Workspace *workspace, Resource *resource, Operations *operations);
#define Sesh(...) SESH_CONSTRUCT(Sesh, __VA_ARGS__)
#define Sesh_zero() Sesh_0()
bool Sesh_setAuthService(Sesh *self, AuthService *authService);
bool Sesh_setClient(Sesh *self, SeshClient *client);
bool Sesh_setWorkspace(Sesh *self, Workspace *workspace);
bool Sesh_setResource(Sesh *self, Resource *resource);
bool Sesh_setOperations(Sesh *self, Operations *operations);
AuthService *Sesh_getAuthService(const Sesh *self);
SeshClient *Sesh_getClient(const Sesh *self);
Workspace *Sesh_getWorkspace(const Sesh *self);
Resource *Sesh_getResource(const Sesh *self);
Operations *Sesh_getOperations(const Sesh *self);
int Sesh_submit(Sesh *self, const Operation *operation);
void Sesh_close(Sesh *self);
bool Sesh_toString(const Sesh *self, char *dest, size_t cap, bool *outTruncated);
bool Sesh_toStringStruct(const Sesh *self, char *dest, size_t cap, bool *outTruncated);
#endif
