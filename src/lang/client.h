#ifndef SESH_LANG_CLIENT_H
#define SESH_LANG_CLIENT_H
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "lang/arity.h"
/* Graphvex exports Client; this session identity is deliberately SeshClient.
 * Host assigns/persists a nonzero installation ID. No hardware/IP identity and
 * no automatic global uniqueness or persistence is claimed. External serialization.
 */
typedef struct SeshClient { uint64_t id; } SeshClient;
SeshClient SeshClient_0(void);
SeshClient SeshClient_1(uint64_t id);
#define SeshClient(...) SESH_CONSTRUCT(SeshClient, __VA_ARGS__)
#define SeshClient_zero() SeshClient_0()
bool SeshClient_setId(SeshClient *self, uint64_t id);
uint64_t SeshClient_getId(const SeshClient *self);
bool SeshClient_toString(const SeshClient *self, char *dest, size_t cap, bool *outTruncated);
bool SeshClient_toStringStruct(const SeshClient *self, char *dest, size_t cap, bool *outTruncated);
#endif
