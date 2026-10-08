# sesh, by Vex.

## CLion: CMake is IDE metadata only

Open this repository root as a CMake project. `CMakeLists.txt` supplies an excluded
C23 object target for diagnostics and inlay hints. Supply local Vexspoke and
API Haven source roots; no fake declarations, downloads or application runner.
IDE appearance is user-verified. Integrated builds belong to
[b](https://github.com/vex-graph/b); the isolated owner runner below proves the
snapshot slice without building the engine.

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
- **Compile-Time Dependencies**: R4 may borrow Vexspoke R2 CPU computation/behavior contracts, Relational Engine IO/NIO/storage contracts and API Haven. Native IO/NIO and the compatible default production Memory C implementation are engine-owned, not rewritten into Rust. The snapshot slice uses API vocabulary and CPU annotations only; engine/host integration is not proved. R1 owns lifetimes/residency; no C/Rust atomic-layout compatibility or automatic schema migration is assumed. Graphics remain forbidden.
- **Consuming Applications**: Powering `darling-editor`, `semicolon` remote pairing, and `anti` bug reporting.
