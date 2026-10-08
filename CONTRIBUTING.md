# Contributing to sesh

This project is a strictly solo development process conducted in tight pair-programming partnership with an AI coding assistant.

It serves as an architectural manifesto for **Level 4 Session Synchronization and VPS Relay**: lockless live coordination, single-transaction atomic synchronization, bounded network timeouts, and zero transactions across event dispatches in pure C23.

---

## 1. The AI-First Architecture Manifesto & Boilerplate Defense

This codebase strictly enforces the verbose, explicit boilerplate required across the `vexgraph` ecosystem:
- Strict prohibition of arrow syntax (`p->field` is banned; only explicit `(*p).field` is permitted).
- Single Class Per File (the Java Law: one public `typedef struct` per `.h`/`.c` pair).
- Arity-overloaded explicit constructor dispatch macros (`Class_0()`, `Class_1()`).
- Complete, symmetric getters and setters for all struct fields.
- Strict dest-last parameter ordering `(a, b, dest)`.
- Two-layer member access cap (`(*layer1).layer2` maximum).
- Exhaustive `;;OVERVIEW` blueprints mirrored at the top of every implementation file.

---

## 2. Supreme Living Document: `../../../preferences.md` & Repo-Local Preferences

All architectural rules and style invariants are governed by the central constitution:

- **[preferences.md](https://gist.github.com/vex-graph/4132a6c45cb6d3797c3e8eff2e94035a)** (one real, Git-ignored workspace-root `../../../preferences.md`, not a tracked Vexspoke file or symlink)
- **[sesh-preferences.md](sesh-preferences.md)** (repo-local mirror binding sesh)

Under the Living Documentation Law, update affected contracts in the same cycle.
Universal changes are published to the existing Gist and byte-verified; repo-local
documentation is committed locally under the Git Workflow Law. Never auto-push.
R2 comprises Vexspoke CPU computation/behavior and Relational Engine memory/storage
and native C search. Sesh may borrow both R2 public contracts and api-haven;
native IO/NIO and the compatible default production Memory C implementation are
engine-owned, not a Rust allocator rewrite. The caller-buffer snapshot core
does not establish live engine integration. Run `python3 tests/sesh/run.py` from
the workspace root; read the README's explicit provider and durability gaps.
The public composition is `src/lang/sesh.h` with seven owning classes under
`src/session/`. Run `python3 tests/sesh/session_run.py` for their strict,
ASan/UBSan and caller-serialized TSan proof. `SeshClient` avoids the existing
Graphvex `Client` symbol; do not introduce a global `Client` alias. Local revision
admission is not remote authorization, data/SQL execution or distributed CAS.

---

## 3. `sesh` Architectural Invariants

| Invariant | Specification |
| :--- | :--- |
| **Single-Transaction Live Coordination** | Atomic sync transactions; never hold transaction locks across network boundaries (per the Single-Transaction Live Coordination Law). |
| **No-Transaction-Across-Event-Dispatch** | Complete all transactional mutations before emitting events; never dispatch events with open transactions (per the No-Transaction-Across-Event-Dispatch Law). |
| **Bounded Waits on Network Slices** | 100ms maximum wait times on network slices with graceful drop-degrade paths (per the Bounded Wait Law). |
| **Teardown Reverse Order** | Session channels, relays and sockets retire before memory arenas (per the Vertical Integration Law (Teardown)). |
