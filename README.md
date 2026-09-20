# sesh, by Vex.

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

- **Runtime Supervised**: Supervised by R0 `hotcwap`.
- **Compile-Time Dependencies**: Borrows R1 `vexspoke` (memory, atomics, sockets) and R2 `api-haven` (connector shapes, fanout).
- **Consuming Applications**: Powering `darling-editor`, `semicolon` remote pairing, and `anti` bug reporting.
