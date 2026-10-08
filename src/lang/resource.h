#ifndef SESH_LANG_RESOURCE_H
#define SESH_LANG_RESOURCE_H
#include "lang/workspace.h"
/* Revision is local serialized metadata, NOT a remote transaction/CAS token.
 * Zero revision is a valid baseline. Host restores through setRevision only
 * while externally excluding borrowers; submit advances it with overflow guard.
 */
typedef struct Resource { uint64_t id; uint64_t workspaceId; uint64_t revision; } Resource;
Resource Resource_0(void);
Resource Resource_3(uint64_t id, uint64_t workspaceId, uint64_t revision);
#define Resource(...) SESH_CONSTRUCT(Resource, __VA_ARGS__)
#define Resource_zero() Resource_0()
bool Resource_setId(Resource *self, uint64_t id);
bool Resource_setWorkspaceId(Resource *self, uint64_t workspaceId);
bool Resource_setRevision(Resource *self, uint64_t revision);
uint64_t Resource_getId(const Resource *self);
uint64_t Resource_getWorkspaceId(const Resource *self);
uint64_t Resource_getRevision(const Resource *self);
bool Resource_toString(const Resource *self, char *dest, size_t cap, bool *outTruncated);
bool Resource_toStringStruct(const Resource *self, char *dest, size_t cap, bool *outTruncated);
#endif
