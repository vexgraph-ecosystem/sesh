#include "lang/client.h"
#include "session/text.h"
#include "annotation/definition.h"
#include "annotation/overview.h"
;;DEFINITION
/* A SeshClient is one host-assigned installation identity, independent of its
 * network endpoint, account and platform. Value object; no owned resources.
 * The host must persist/deduplicate IDs before sharing them with other clients.
 */
;;OVERVIEW
/* CLASS: SeshClient. STRUCT FIELDS: uint64_t id: installation identity (zero=empty).
 * PUBLIC: _0/_1/chooser/zero, setId (reject zero preserving state), getId (null=0),
 * toString/toStringStruct. No private helpers. Externally serialized.
 */
SeshClient SeshClient_0(void) { return (SeshClient) {0}; }
SeshClient SeshClient_1(uint64_t id) {
    SeshClient result = SeshClient_0();
    SeshClient_setId(&result, id);
    return result;
}
bool SeshClient_setId(SeshClient *self, uint64_t id) {
    if (self == nullptr || id == 0) {
        THROW("client identity: null or zero ID");
        return false;
    }
    (*self).id = id;
    return true;
}
SESH_GETTER(SeshClient, uint64_t, Id, id, 0)
SESH_VALUE_STRING(SeshClient, SeshText_format(dest, cap, outTruncated, "client %llu", (unsigned long long) (*self).id))
bool SeshClient_toStringStruct(const SeshClient *self, char *dest, size_t cap, bool *outTruncated) {
    return self == nullptr ? SeshText_format(dest, cap, outTruncated, "nullptr") :
        SeshText_format(dest, cap, outTruncated, "SeshClient{id=%llu}", (unsigned long long) (*self).id);
}
