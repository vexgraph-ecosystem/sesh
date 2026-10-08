# sesh — Repo-Local Living Preferences
> Repo-local preferences governed by the Living Documentation Law.
> Universal Supreme Constitution: workspace-root preferences.md, published on Gist.

## 0. Constitution Link (supreme)
- [preferences.md](https://gist.github.com/vex-graph/4132a6c45cb6d3797c3e8eff2e94035a) — real, Git-ignored workspace-root file at ../../../preferences.md, not a tracked Vexspoke file or symlink.
- All universal laws in `../../../preferences.md` are mandatory and binding across the ecosystem.
- This document codifies **exclusive** preferences for `sesh` (R4 Session Relay). R2 is split between Vexspoke computation/behavior and Relational Engine memory/storage/native C search. Sesh may borrow RE IO/NIO, Vexspoke CPU contracts and api-haven; graphics remain forbidden. Migrated native IO/NIO ownership does not implement this session blueprint or prove Rust/C integration.

## 1. Repo-Local Law Index (Binding Matrix)

Universal laws are inherited from the canonical `../../../preferences.md` Index; this table indexes the additional laws specific to this repository.

| Law Title | Scope | Enforcement |
| :--- | :--- | :--- |
| **Single-Transaction Live Coordination Law** | R4 Session Relay | Mandatory for `sesh` |
| **No-Transaction-Across-Event-Dispatch Law** | R4 Session Relay | Mandatory for `sesh` |
| **Snapshot Backup Boundary Law** | R4 explicit backup scheduling | Caller-buffer owner proof; cloud/durability gaps remain explicit |

## 2. Exclusive Repo-Local Laws (FULL PROSE RESTATEMENT)

### Single-Transaction Live Coordination Law

During a live resize every layer change across the seam canvas and any DIRECT
pane layers lands in ONE explicit `CATransaction` (frame in logical points +
`drawableSize` in native px per the Native Pixel Law) with implicit layer
actions disabled for the drag — so every edge tracks the cursor on the same
event and no layer lags the other. `presentsWithTransaction YES` then makes
each drawable swap atomic with its own motion, and because the layers are
sublayers of the same window subtree committed in that one transaction, the
whole composite (blur + seam + panes) lands on one vsync.

---

### No-Transaction-Across-Event-Dispatch Law

No transaction spans `[NSApp sendEvent:]`. A live-resize drag enters AppKit's
modal tracking loop INSIDE `sendEvent` and blocks thread 0 until mouse-up.
Any `CATransaction` open across that call collects every native layer frame
(blur view, content view, pane motion) into ONE commit at mouse-up — the
window freezes mid-drag. The event pump therefore commits+rebegins around
each dispatch: the tracking loop always runs with no outer transaction, AppKit
commits each native drag step on its own runloop turn, and the pump's own
batch commits before/after (idle cadence for `presentsWithTransaction`
drawables).

---

## 3. Repo-Local Extensions (managed, per the Conflict Triage Law)

### Snapshot Backup Boundary Law

The first implemented slice is explicit snapshot backup, not realtime collaboration.
`SeshSnapshot` owns job policy but borrows caller-owned staging storage and an API
Haven provider callback/context. Admission copies bytes once; input can then be
released. One active job rejects overlapping admission observably and preserves
the prior bytes/state. Larger caller storage can be supplied between jobs; the
initial capacity is not a permanent project-size ceiling.

The caller supplies monotonic time, unique nonzero snapshot identities, resource
lifetimes and external serialization. No allocator, worker, filesystem, credential
or network ownership moves into Sesh. `storage/snapshot_io.h` is API Haven's
provider-neutral vocabulary; Google-specific request/OAuth behavior belongs to
its future connector, not Sesh. Engine native IO/NIO may be borrowed when needed;
offline caller-buffer proof does not claim engine or host integration.

Provider callbacks return within bounded slices and retain key/content across
pending/retry. DONE means durable acknowledgement. Retry must be idempotent;
exhaustion, rejection and cancellation release outstanding borrows before staging
storage is reused. Pending progress has no core-level overall deadline yet; the
caller must cancel on its own deadline. Teardown cancellation never logs.

Configuration has validated symmetric APIs. Job-derived length, identity, retry
count, next-attempt time and state are read through getters and changed only by
queue/step/cancel. This scoped managed exception to unrestricted scalar setters
preserves acknowledgement truth and borrow safety under the Conflict Triage Law
and Single Class Per File Law (Java Law); the implementation carries `;;INTENTION`.
Never expose a setter that lets a consumer forge SAVED or drop an active borrow.

Registered proof is `python3 tests/sesh/run.py`: strict/assertion-enabled and
ASan/UBSan owners, offline fake put/get roundtrip, rejection diagnostics, immutable
admission, retry clock arithmetic and cancellation. Only disposable ignored
`tests/sesh/test.txt` is intended for an explicitly authorized cloud smoke test.
Tracked test source is never ignored. No canonical preferences or secrets are
implicitly uploaded. Google Drive is the planned first storage provider; iCloud
requires its own identity/entitlement integration. HTTPS/OAuth, actual cloud
idempotency, durable local journal, validated restore and collaborative merging
remain future integration obligations, not claims established by the fake.

;;INTENTION("R4 Session Relay: lockless single-transaction live coordination; bounded wait networking; zero transaction across event dispatch.")

---

## 4. Readiness Cross-Reference (Living Documentation Law)

- Feature readiness matrix: [sesh](../../ecosystem/sesh.md), rendered as `[[sesh]]`.
