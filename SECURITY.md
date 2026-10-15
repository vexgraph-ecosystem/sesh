# SECURITY.md — sesh

> **Internal invariant set, not a public disclosure channel.** See the
> [vexgraph security baseline](../../../SECURITY.md) for everything inherited
> here. This file states only this repository's own surface.

**Role:** R4 session relay. It owns local session composition and explicit
snapshot backup. It owns **no credentials, no sockets and no cloud identity**:
those are the API-haven connector's and the host's.

## Surface

- **Outbound uploads** — caller-owned snapshot bytes are handed to a
  provider-neutral callback (`storage/snapshot_io.h`) that API-haven implements.
  Sesh never speaks HTTPS or OAuth itself.
- **Local session state** — identities, revisions and a copied
  intent/receipt ledger in host-owned storage.

## Secrets

- Sesh borrows an **API-haven credential descriptor** plus a trusted host/API
  identity verifier. Credentials alone never establish a principal; the verifier
  owns authentication and issuer/subject-to-principal mapping.
- Secrets and context are redacted from every projection, and configuration
  failure revokes admitted identity.

## Authorization is local, and it is stated as local

- A principal id is derived through authenticate/revoke only — there is no raw
  setter to forge one.
- Expected-revision checks, replay protection and operation identity are **local**
  guarantees. They are not remote authorization, not a distributed CAS, and not
  a data/SQL execution path; a real provider must supply its own authoritative
  authorization and revision guarantees before multi-client synchronisation is
  claimed anywhere.
- All parts sharing a session require **one caller-owned serialization domain**.
  There is no internal thread safety and no cross-process atomicity.

## Data leaving the machine

- An upload sends caller-owned bytes to a third-party provider. Nothing is
  uploaded implicitly: admission is explicit, identity and storage are supplied
  by the caller, and only a disposable ignored test file is used for an
  explicitly authorised cloud smoke.
- Tracked test sources are never ignored, and canonical preferences are never
  uploaded.

## Limits

- The **offline fake put/get is not provider, TLS or OAuth proof**; a durable
  journal, validated restore and collaborative merging remain unimplemented.
- There is no core-level overall deadline: the caller must cancel on its own
  deadline.
- Google Drive is the planned first provider; iCloud needs its own
  entitlement/identity integration.
