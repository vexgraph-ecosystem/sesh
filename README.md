# sesh, by Vex.

## CLion: CMake is IDE metadata only

Open this repository root as a CMake project. `CMakeLists.txt` is an IDE-only
blueprint entry: there are no production sources or C23 source targets yet,
so there is nothing to provide semantic diagnostics or inlay hints for.
No fake declarations, dependency downloads, linking or application runner are
wired into it. IDE appearance is user-verified.

Future builds belong to [b](https://github.com/vex-graph/b). The workspace's empty
registry entry is not implemented session behavior or a standalone runtime build.

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
- **Compile-Time Dependencies**: The allowlist remains Vexspoke + api-haven. R2 is split between Vexspoke CPU computation/behavior and Relational Engine memory/storage/native C search; that ownership does not grant Sesh a direct engine dependency. Migration is staged: existing Vexspoke memory/container ABI and default allocator remain. R1 owns lifetimes/residency; no C/Rust atomic-layout compatibility or automatic schema migration is assumed. GPU shaders/dispatch remain Graphvex R3. This source-free blueprint has no implemented session or engine integration.
- **Consuming Applications**: Powering `darling-editor`, `semicolon` remote pairing, and `anti` bug reporting.
