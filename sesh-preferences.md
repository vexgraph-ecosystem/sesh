# sesh — Repo-Local Living Preferences
> Universal constitution: `gist.md` is the compressed law list; `PREFERENCES.md` is its rationale.

## 0. Constitution Link (supreme)
- [gist.md](https://gist.github.com/vex-graph/4132a6c45cb6d3797c3e8eff2e94035a#file-gist-md) — the compressed law list, mirrored in this workspace at `../../../metadata/gists/preferences/gist.md`. It is the only normative law text; `../../../PREFERENCES.md` carries the rationale and is never ingested whole. Numbers are slice addresses inside one file, never law identity: cite by Title.
- All universal laws bind this repository. Read one on demand from the workspace root: `./tools/prefs law "<title>"`, `./tools/prefs section "<section>"`, `./tools/prefs repo <name>`.
- This file defines exclusive `sesh` (R4 Session Relay) rules: R2 splits Vexspoke computation/behavior from Relational Engine memory/storage/native C search; Sesh may borrow RE IO/NIO, Vexspoke CPU contracts, api-haven and hotcwap's one-seam window, and implements no graphics of its own. Native IO/NIO ownership proves neither session host/cloud nor Rust/C integration; current session-composition proof is local metadata only.

## 1. Repo-Local Law Index (Binding Matrix)

Universal laws inherit from the compressed constitution (`../../../metadata/gists/preferences/gist.md`); this table lists this repository's additions.

| Law Title | Scope | Enforcement |
| :--- | :--- | :--- |
| **Single-Transaction Live Coordination Law** | R4 Session Relay | Mandatory for `sesh` |
| **No-Transaction-Across-Event-Dispatch Law** | R4 Session Relay | Mandatory for `sesh` |
| **Snapshot Backup Boundary Law** | R4 explicit backup scheduling | Caller-buffer owner proof; cloud/durability gaps remain explicit |
| **Session Composition and Identity Law** | R4 session objects and local operation admission | Per-class owner/identity/replay tests; no implicit distributed transaction |

## 2. Exclusive Repo-Local Laws (FULL PROSE RESTATEMENT)

### Single-Transaction Live Coordination Law

On live resize, every seam-canvas and DIRECT pane-layer change lands in ONE explicit `CATransaction`: frame in logical points, `drawableSize` in native px (Native Pixel Law), implicit layer actions disabled. This keeps every edge tracking the same event. `presentsWithTransaction YES` makes drawable swaps atomic with their motion; as sublayers of one window subtree, blur + seam + panes commit together on one vsync.

---

### No-Transaction-Across-Event-Dispatch Law

Never span `[NSApp sendEvent:]` with a transaction. A live-resize drag enters AppKit's modal tracking loop INSIDE `sendEvent`, blocking thread 0 until mouse-up; a transaction left open would collect blur-view, content-view and pane frames into one mouse-up commit and freeze the window. The event pump commits and re-begins around each dispatch: no outer transaction during tracking; AppKit commits each drag step on its runloop turn; the pump's batch commits before/after for idle-cadence `presentsWithTransaction` drawables.

---

## 3. Repo-Local Extensions (managed, per the Conflict Triage Law)

### Snapshot Backup Boundary Law

First implemented slice: explicit snapshot backup, not realtime collaboration. `SeshSnapshot` owns job policy, borrows caller-owned staging storage and an API Haven provider callback/context; admission copies bytes once, so input may then be released. Only one active job: overlapping admission rejects observably and preserves prior bytes/state. Caller may supply larger storage between jobs; initial capacity is no project-size ceiling.

Caller supplies monotonic time, unique nonzero snapshot identities, resource lifetimes and external serialization. Sesh owns no allocator, worker, filesystem, credential or network. `storage/snapshot_io.h` is API Haven's provider-neutral vocabulary; Google-specific request/OAuth belongs to its future connector, not Sesh. Engine native IO/NIO may be borrowed; offline caller-buffer proof is not engine or host integration.

Provider callbacks return within bounded slices and retain key/content across pending/retry. DONE means durable acknowledgement; retries are idempotent. Exhaustion, rejection and cancellation release outstanding borrows before staging reuse. No core overall deadline exists for pending progress: caller must cancel by its deadline. Teardown cancellation never logs.

Configuration has validated symmetric APIs. Getters expose job-derived length, identity, retry count, next-attempt time and state; only queue/step/cancel change them. Scoped managed exception to unrestricted scalar setters under the Conflict Triage Law and Single Class Per File Law (Java Law), marked `;;INTENTION`, preserves acknowledgement truth and borrow safety. No setter may forge SAVED or drop an active borrow.

Proof: `python3 tests/sesh/run.py` runs strict/assertion-enabled and ASan/UBSan owners covering offline fake put/get, rejection diagnostics, immutable admission, retry clock arithmetic and cancellation. Only disposable ignored `../../../tests/sesh/test.txt` is for an explicitly authorized cloud smoke test; tracked test source is never ignored. No canonical preferences or secrets are implicitly uploaded. Google Drive is the planned first provider; iCloud needs identity/entitlement integration. HTTPS/OAuth, actual cloud idempotency, durable local journal, validated restore and collaborative merging remain future obligations, not proved by the fake.

### Session Composition and Identity Law

`lang/sesh.h` owns composition via borrowed AuthService, SeshClient, Workspace, Resource and Operations references. Public contracts: `src/lang`; implementations: `src/session`, one class per pair. SeshClient is prefixed to avoid collision with Graphvex's public `Client`. FileSession/DirectorySession will compose Sesh, not duplicate identity/admission/history machinery.

AuthService borrows API Haven's credential descriptor and a trusted host/API identity verifier. Credentials alone establish no principal: the verifier owns authentication and issuer/subject-to-principal mapping. A fake callback is not OAuth proof. Configuration/failure revokes admitted identity; projections redact secrets/context. Principal ID derives only through authenticate/revoke; no raw setter is a scoped safety exception under the Conflict Triage Law and Single Class Per File Law (Java Law), marked in implementation.

Host assigns and persists nonzero identities: Client = installation, Workspace = private principal-owned namespace, Resource = target, `(clientId, operationId)` = retry identity. None are pointers or ecosystem type IDs. Provider account linking, shared ACLs, durable identities and automatic globally unique generation are unimplemented.

Operations is a flat copied-intent/receipt ledger in host-owned storage. Cold reserve grows/moves records; no escaped row pointer or permanent total ceiling. Count/backing/receipt flags change only through reserve/add/apply/clear, not setters—a scoped Conflict Triage Law exception preserving initialized history and acknowledgement truth. Queued intent is not an applied receipt. Clearing requires exclusion and coordinated baseline/identity epoch changes; replay protection lasts only while receipts remain retained.

`Sesh_submit` validates admitted principal/client/workspace/resource scope and expected local revision, copies intent and advances local revision exactly once. Replays do not advance; conflicts, overflow and exhausted current storage preserve revision/history. All shared parts/sessions require one caller-owned external serialization domain. No internal thread safety, cross-process atomicity, remote ACL, data/SQL execution or distributed CAS is inferred; database transactions belong to the database owner. Real providers must supply authoritative authorization/revision guarantees before claiming multi-client sync. Close detaches, never destroys borrows.

Proof: `python3 tests/sesh/session_run.py` (also main Sesh runner): strict and ASan/UBSan owners, TSan caller-serialized four-client admission, supported/unsupported arities, Graphvex/Sesh header composition. Real files/directories, cloud providers, persistence and distributed operation application require separate workflow/integration proof.

;;INTENTION("R4 Session Relay: lockless single-transaction live coordination; bounded wait networking; zero transaction across event dispatch.")

---

## 4. Readiness Cross-Reference (Living Documentation Law)

- Feature readiness matrix: [sesh](https://gist.github.com/vex-graph/6943f92acb931b25dad1073c46da6ce7#file-sesh-md).
- Open blockers and deferred decisions: [ecosystem blockers Gist](https://gist.github.com/vex-graph/e921fa188eebbd0c68c4e59646109887).
