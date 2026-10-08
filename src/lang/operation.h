#ifndef SESH_LANG_OPERATION_H
#define SESH_LANG_OPERATION_H
#include "lang/resource.h"
/* Immutable intent once copied into Operations: identity is (clientId,id).
 * The first operation form is a local revision advance, not file/SQL payload.
 * UINT64_MAX expectedRevision cannot advance and rejects in Sesh_submit.
 */
typedef struct Operation {
    uint64_t id;
    uint64_t clientId;
    uint64_t workspaceId;
    uint64_t resourceId;
    uint64_t expectedRevision;
} Operation;
Operation Operation_0(void);
Operation Operation_5(uint64_t id, uint64_t clientId, uint64_t workspaceId, uint64_t resourceId, uint64_t expectedRevision);
#define Operation(...) SESH_CONSTRUCT(Operation, __VA_ARGS__)
#define Operation_zero() Operation_0()
bool Operation_setId(Operation *self, uint64_t id);
bool Operation_setClientId(Operation *self, uint64_t clientId);
bool Operation_setWorkspaceId(Operation *self, uint64_t workspaceId);
bool Operation_setResourceId(Operation *self, uint64_t resourceId);
bool Operation_setExpectedRevision(Operation *self, uint64_t expectedRevision);
uint64_t Operation_getId(const Operation *self);
uint64_t Operation_getClientId(const Operation *self);
uint64_t Operation_getWorkspaceId(const Operation *self);
uint64_t Operation_getResourceId(const Operation *self);
uint64_t Operation_getExpectedRevision(const Operation *self);
bool Operation_isValid(const Operation *self);
bool Operation_equals(const Operation *left, const Operation *right);
bool Operation_toString(const Operation *self, char *dest, size_t cap, bool *outTruncated);
bool Operation_toStringStruct(const Operation *self, char *dest, size_t cap, bool *outTruncated);
#endif
