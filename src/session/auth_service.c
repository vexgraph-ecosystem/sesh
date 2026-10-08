#include "lang/auth_service.h"
#include "session/text.h"
#include "annotation/definition.h"
#include "annotation/overview.h"
#include "annotation/intention.h"
;;DEFINITION
/* AuthService binds borrowed API Haven credentials to a trusted verifier. A
 * principal is admitted only through successful verification, never merely by
 * possessing an API key string. Authentication failure revokes local identity.
 * Host/API owns actual verification, secure storage, expiry and issuer mapping.
 */
;;OVERVIEW
/* CLASS: AuthService. STRUCT FIELDS: const ApiAuth *auth: borrowed credential
 * descriptor; AuthServiceVerifyFn verifier: trusted bounded identity callback;
 * void *context: borrowed verifier context; uint64_t principalId: admitted identity.
 * PUBLIC: _0/_3/chooser/zero; setAuth/setVerifier/setContext (reject missing required inputs; successful
 * reconfiguration revokes principal); getAuth/Verifier/Context/PrincipalId;
 * isAuthenticated; authenticate (failure revokes); revoke (quiet/idempotent);
 * toString/toStringStruct (credentials and context redacted, no dereference).
 * No private helpers. External serialization; verification is not sandboxed.
 */
;;INTENTION("per the Conflict Triage Law + Single Class Per File Law (Java Law): principalId has no arbitrary setter; authenticate/revoke alone establish verification state")
AuthService AuthService_0(void) { return (AuthService) {0}; }
AuthService AuthService_3(const ApiAuth *auth, AuthServiceVerifyFn verifier, void *context) {
    AuthService result = AuthService_0();
    if (auth == nullptr || verifier == nullptr) {
        THROW("auth service: missing credential or verifier");
        return result;
    }
    result.auth = auth;
    result.verifier = verifier;
    result.context = context;
    return result;
}
bool AuthService_setAuth(AuthService *self, const ApiAuth *auth) {
    if (self == nullptr || auth == nullptr) {
        THROW("auth service credentials: null input");
        return false;
    }
    (*self).auth = auth;
    (*self).principalId = 0;
    return true;
}
bool AuthService_setVerifier(AuthService *self, AuthServiceVerifyFn verifier, void *context) {
    if (self == nullptr || verifier == nullptr) {
        THROW("auth service verifier: null input");
        return false;
    }
    (*self).verifier = verifier;
    (*self).context = context;
    (*self).principalId = 0;
    return true;
}
bool AuthService_setContext(AuthService *self, void *context) {
    if (self == nullptr) {
        THROW("auth service context: null service");
        return false;
    }
    (*self).context = context;
    (*self).principalId = 0;
    return true;
}
SESH_GETTER(AuthService, const ApiAuth *, Auth, auth, nullptr)
SESH_GETTER(AuthService, AuthServiceVerifyFn, Verifier, verifier, nullptr)
SESH_GETTER(AuthService, void *, Context, context, nullptr)
SESH_GETTER(AuthService, uint64_t, PrincipalId, principalId, 0)
bool AuthService_isAuthenticated(const AuthService *self) { return AuthService_getPrincipalId(self) != 0; }
void AuthService_revoke(AuthService *self) {
    if (self != nullptr)
        (*self).principalId = 0;
}
bool AuthService_authenticate(AuthService *self) {
    if (self == nullptr || (*self).auth == nullptr || (*self).verifier == nullptr) {
        AuthService_revoke(self);
        THROW("auth service authenticate: unconfigured");
        return false;
    }
    uint64_t principalId = 0;
    if (!(*self).verifier((*self).auth, (*self).context, &principalId) || principalId == 0) {
        AuthService_revoke(self);
        THROW("auth service authenticate: rejected identity");
        return false;
    }
    (*self).principalId = principalId;
    return true;
}
SESH_VALUE_STRING(AuthService, SeshText_format(dest, cap, outTruncated, "principal %llu", (unsigned long long) (*self).principalId))
bool AuthService_toStringStruct(const AuthService *self, char *dest, size_t cap, bool *outTruncated) {
    return self == nullptr ? SeshText_format(dest, cap, outTruncated, "nullptr") :
        SeshText_format(dest, cap, outTruncated, "AuthService{auth=%s,verifier=%s,context=%s,principalId=%llu}",
                        (*self).auth == nullptr ? "nullptr" : "redacted", (*self).verifier == nullptr ? "nullptr" : "bound",
                        (*self).context == nullptr ? "nullptr" : "redacted", (unsigned long long) (*self).principalId);
}
