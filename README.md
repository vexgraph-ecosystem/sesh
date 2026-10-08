# sesh, by Vex.

## CLion: CMake is IDE metadata only

Open this repository root as a CMake project. `CMakeLists.txt` supplies an excluded
C23 object target for diagnostics and inlay hints. Supply local Vexspoke and
API Haven source roots; no fake declarations, downloads or application runner.
IDE appearance is user-verified. Integrated builds belong to
[b](https://github.com/vex-graph/b); the isolated owner runner below proves the
snapshot slice without building the engine.

## Sesh: the session object

The public vocabulary is implemented under `src/lang/`, one class per header
and `src/session/` implementation pair:

| Class | Responsibility |
| :--- | :--- |
| `AuthService` | Borrow API Haven credentials and obtain a principal through an injected trusted verifier; reconfiguration/failure revokes identity. |
| `SeshClient` (`lang/client.h`) | Host-assigned installation identity. Graphvex already owns the unrelated C type `Client`; both APIs can coexist. |
| `Workspace` | Private principal-owned project identity. Remote ACLs/sharing are not implemented. |
| `Resource` | Workspace-scoped target identity and local revision metadata. |
| `Operation` | Copied intent: operation ID, originating client, workspace, resource and expected revision. |
| `Operations` | Flat host-backed growable intent/receipt ledger. `reserve` copies records into larger caller storage; no `Operation **` graph. |
| `Sesh` | Borrow these five parts; authenticate/scope-check admission, reject stale revisions, deduplicate intent and coordinate one local revision advance. |

`lang/sesh.h` is the composed entry point. All classes provide arity constructors,
safe getters, validated configuration and bounded string projections. Secret
values/context are never dumped. Object/backing lifetimes and identity persistence
are supplied by the host; no secret, allocator, socket or thread ownership moves
into the session model. Numeric identities are application metadata, not ecosystem
type IDs, memory addresses, hardware serials or automatic globally unique IDs.

Typical admission, using caller-supplied live objects and ledger storage:

```c
Sesh session = Sesh(authService, client, workspace, resource, operations);
Operation intent = Operation(operationId, SeshClient_getId(client),
                             Workspace_getId(workspace), Resource_getId(resource),
                             Resource_getRevision(resource));
int result = Sesh_submit(&session, &intent);
```

Authenticate `authService` through its trusted adapter first. `SESH_APPLIED`
means **local revision intent applied**, not bytes uploaded or a SQL transaction
committed. Identical retained receipts return `SESH_REPLAY`; changed intent with
the same `(clientId, operationId)` rejects. A merely queued intent is not an
applied receipt. Stale revisions return `SESH_CONFLICT` without modifying resource
or history. Capacity exhaustion preserves revision; caller can reserve larger
storage and retry. Revision overflow rejects. `Sesh_close` quietly detaches parts,
leaving borrowed identities/history usable.

**Concurrency contract:** one shared external serialization domain is required
for all operations on sessions, resources, authentication and ledgers—including
different sessions sharing the same resource. The classes do not supply an
internal mutex or distributed CAS. Clearing history/resetting revisions requires
exclusion and a coordinated new identity epoch/baseline; dedup lasts only while
receipts are retained. Borrowed backing rows must not be modified by consumers;
`Operations_find` outputs go to disjoint caller storage.

`python3 tests/sesh/run.py` now includes ten independently executed session owner
targets per strict and ASan/UBSan configuration, plus the synchronized four-client
Sesh owner under TSan. They prove scope rejection, auth revocation/secret redaction,
growth, copied intent, replay versus pending receipt, stale conflicts, overflow,
failure preservation and caller-serialized concurrency. Interop includes Graphvex's
header without naming collisions. Neither fake verification nor local revision
checks prove OAuth, remote authorization, persistence, distributed commits or sync.

## First implemented slice: explicit snapshot backup

`src/snapshot/snapshot.{h,c}` owns one upload job in caller-owned staging storage.
It copies admitted bytes, retains one nonzero identity across pending/retry steps,
enforces configurable retry delay/exhaustion, and cancels before releasing provider
borrows. Empty/exact-capacity input is supported; oversize/busy admission preserves
the existing job. Callers may supply larger storage between jobs; capacity is not
a hardcoded project-size ceiling. All operations require external serialization.

API Haven's `storage/snapshot_io.h` defines injected put/get callbacks. This is
provider vocabulary, **not an implemented Google Drive connector**. No filesystem,
allocator, socket, worker, secret, or OS-clock ownership lives in this class.
R1 must supply provider lifetimes and bounded execution. Pending operations can
continue until the caller cancels; retry exhaustion is not an overall deadline.
Caller clocks must be monotonic. Snapshot keys must be globally unique within the
provider namespace; the core does not generate identities or detect collisions.

From the workspace root:

```sh
python3 tests/sesh/run.py
```

The tracked runner and `tests/sesh/snapshot/snapshot_test.c` compile the real core
with strict C23/assertions, then ASan/UBSan, under bounded subprocess watchdogs.
They copy ignored `tests/sesh/test.txt` through an in-memory fake and restore
identical bytes. The runner creates disposable input if absent and verifies its
Git ignore rule. No canonical preferences file is uploaded or changed.

### Next integration: Google Drive first, iCloud later

1. API Haven: verified HTTPS, explicit status/error taxonomy, Drive uploads and
   downloads, OAuth desktop authorization/token refresh, idempotent identity mapping.
2. Sesh: durable local journal and validated restore policy; no blind overwrite or
   silent drop of user work. Credentials and unrelated files are excluded.
3. Explicitly authorized smoke test: upload only `test.txt` to an app-created Drive
   folder, download to separate storage, compare bytes, and clean up only test-owned
   remote objects. No real account access is performed by the offline suite.

Use Google's `drive.file` scope for visible test backups, not full-Drive access.
The user creates a Google Cloud project, enables Drive API, configures OAuth test
users, and registers a Desktop app client. Refresh tokens belong in secure OS
storage, never source/Git/logs. Hidden `appDataFolder` cannot share files.
References: [Drive scopes](https://developers.google.com/workspace/drive/api/guides/api-specific-auth)
and [app data](https://developers.google.com/workspace/drive/api/guides/appdata).
Google Drive is storage, not a live collaboration relay. iCloud/CloudKit needs
its own platform identity/entitlement integration; it is not interchangeable OAuth.

**Proof gaps:** Drive/iCloud, HTTPS/OAuth, durable journal, provider idempotency,
verified restoration, overall transfer deadlines, real host integration, concurrent
edits and non-macOS execution remain unproved or unimplemented.

## Accepted next public surface: file and directory sessions

The snapshot core is a building block, not the final user-facing workflow.
`FileSession` and `DirectorySession` are **proposed, not implemented classes**:

- `FileSession_sync(session, localFile)` saves one file through the selected provider.
- `DirectorySession_sync(session, localDirectory)` saves a versioned directory:
  upload file content first, then publish a manifest only after every file succeeds.
  Explicit sync is not a filesystem watcher. Ignore/exclusion rules must be explicit;
  a Git ignore rule alone is not authorization to upload private content.
- `DirectorySession_clone(session, localDirectory)` validates paths and file hashes,
  then restores into a new or empty destination. It must not overwrite unrelated work.
- `DirectorySession_rebase(session, localDirectory)` compares local/remote changes
  against the last synced baseline, reports conflicts and preserves both versions.
  Automatic text merging, multi-writer atomic publication and deletion propagation
  are separate contracts, not implied by the verb.

FileSession/DirectorySession will compose the implemented `Sesh` rather than
duplicate identity/admission/history. Local directories and files are supplied
explicitly at operation admission; no second generic `Session` base is needed.
Sesh owns workflow and conflict policy, API Haven owns Google protocol/OAuth, and
R2 owns local file/directory operations. The current engine `File` API has no
directory-enumeration operation; that seam must be implemented/proved before a
directory workflow can be delivered. Existing HTTP transport rejects HTTPS;
the Drive adapter needs verified TLS integration as well as credentials.

The next acceptance test is a real nested local directory with create/change/remove
cases, partial-upload failure preserving the last published manifest, clone into
separate storage, byte comparisons and hostile path/symlink rejection. A fake
provider alone is not Google proof, and the current snapshot test is not proof of
any of these unimplemented classes.

Session Management, VPS Relay, In-Engine Bug Ingestion & Cloudflare Edge Sync.

`sesh` is the networked collaboration, telemetry, and session state bridge for the `vexgraph` ecosystem. It powers real-time multi-user canvas pairing (Miro/Figma sync for `darling-editor`), remote VPS relaying, crash snapshot ingestion, and Cloudflare-edge traffic routing.

---

## What `sesh` Solves

1. **Multiplayer Canvas Sessions (`Miro + Figma`)**: Real-time state synchronization for notes, whiteboards, UI wireframes, and live remote cursors over bounded WebSocket fanout (`HavenWsFanout`).
2. **In-Engine Bug Catcher & Telemetry Ingestion**: Ingests crash dumps, memory arena health logs, and serialized scene snapshots from `anti`, `semicolon`, and `darling-editor`, forwarding them reliably to self-hosted VPS instances or Cloudflare endpoints.
3. **Cloudflare Edge Bypassing & Reverse Proxy**: Integration contracts for Cloudflare Workers, Tunnels, Turnstile validation, and KV/R2 asset storage, ensuring secure, low-latency traffic without third-party bloat.
4. **Zero-Allocation Network Sessions**: Session packets use fixed-stride `{session_id, user_id, delta_kind, payload}` records on top of `vexspoke` ring buffers and memory arenas.

---

## Architectural Position

- **Runtime Supervised**: Supervised by the R1 host `hotcwap`.
- **Compile-Time Dependencies**: R4 may borrow Vexspoke R2 CPU computation/behavior contracts, Relational Engine IO/NIO/storage contracts and API Haven. Native IO/NIO and the compatible default production Memory C implementation are engine-owned, not rewritten into Rust. The session and snapshot cores use API vocabulary and CPU annotations only; live engine/host integration is not proved. R1 owns lifetimes/residency; no C/Rust atomic-layout compatibility or automatic schema migration is assumed. Graphics remain forbidden.
- **Consuming Applications**: Powering `darling-editor`, `semicolon` remote pairing, and `anti` bug reporting.
