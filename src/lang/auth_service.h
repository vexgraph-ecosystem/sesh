#ifndef SESH_LANG_AUTH_SERVICE_H
#define SESH_LANG_AUTH_SERVICE_H
#include "api/auth.h"
#include "lang/arity.h"
#include "annotation/what.h"
/* Trusted host/API adapter verifies credentials and maps issuer+subject to a
 * canonical nonzero principal ID. Callback must return within bounded slices;
 * false/nonzero mismatch revokes local identity. No OAuth implementation here.
 * Credential pointer and callback context are borrowed; no secrets are printed.
 */
typedef bool (*AuthServiceVerifyFn)(const ApiAuth *auth, void *context, uint64_t *outPrincipalId);
typedef struct AuthService {
    const ApiAuth *auth;
    AuthServiceVerifyFn verifier;
    ;;WHAT("host-owned authentication verifier context")
    void *context;
    uint64_t principalId;
} AuthService;
AuthService AuthService_0(void);
AuthService AuthService_3(const ApiAuth *auth, AuthServiceVerifyFn verifier, void *context);
#define AuthService(...) SESH_CONSTRUCT(AuthService, __VA_ARGS__)
#define AuthService_zero() AuthService_0()
bool AuthService_setAuth(AuthService *self, const ApiAuth *auth);
bool AuthService_setVerifier(AuthService *self, AuthServiceVerifyFn verifier, void *context);
bool AuthService_setContext(AuthService *self, void *context);
const ApiAuth *AuthService_getAuth(const AuthService *self);
AuthServiceVerifyFn AuthService_getVerifier(const AuthService *self);
void *AuthService_getContext(const AuthService *self);
uint64_t AuthService_getPrincipalId(const AuthService *self);
bool AuthService_isAuthenticated(const AuthService *self);
bool AuthService_authenticate(AuthService *self);
void AuthService_revoke(AuthService *self);
bool AuthService_toString(const AuthService *self, char *dest, size_t cap, bool *outTruncated);
bool AuthService_toStringStruct(const AuthService *self, char *dest, size_t cap, bool *outTruncated);
#endif
