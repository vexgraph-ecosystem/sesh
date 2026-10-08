#ifndef SESH_LANG_WORKSPACE_H
#define SESH_LANG_WORKSPACE_H
#include "lang/client.h"
/* One private principal-owned workspace. Sharing/roles are future contracts,
 * not implied by matching a caller-supplied ID. IDs are live metadata, not secrets.
 */
typedef struct Workspace { uint64_t id; uint64_t principalId; } Workspace;
Workspace Workspace_0(void);
Workspace Workspace_2(uint64_t id, uint64_t principalId);
#define Workspace(...) SESH_CONSTRUCT(Workspace, __VA_ARGS__)
#define Workspace_zero() Workspace_0()
bool Workspace_setId(Workspace *self, uint64_t id);
bool Workspace_setPrincipalId(Workspace *self, uint64_t principalId);
uint64_t Workspace_getId(const Workspace *self);
uint64_t Workspace_getPrincipalId(const Workspace *self);
bool Workspace_toString(const Workspace *self, char *dest, size_t cap, bool *outTruncated);
bool Workspace_toStringStruct(const Workspace *self, char *dest, size_t cap, bool *outTruncated);
#endif
