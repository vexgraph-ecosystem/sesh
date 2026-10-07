# sesh — Repo-Local Living Preferences
> Repo-local preferences governed by the Living Documentation Law.
> Universal Supreme Constitution: workspace-root preferences.md, published on Gist.

## 0. Constitution Link (supreme)
- [preferences.md](https://gist.github.com/vex-graph/4132a6c45cb6d3797c3e8eff2e94035a) — real, Git-ignored workspace-root file at ../../../preferences.md, not a tracked Vexspoke file or symlink.
- All universal laws in `../../../preferences.md` are mandatory and binding across the ecosystem.
- This document codifies **exclusive** preferences for `sesh` (R4 Session Relay). R2 is split between Vexspoke computation/behavior and Relational Engine memory/storage/native C search. Sesh's include allowlist remains Vexspoke + api-haven; storage ownership does not grant a new direct engine dependency or imply migration. This repository remains a session blueprint.

## 1. Repo-Local Law Index (Binding Matrix)

Universal laws are inherited from the canonical `../../../preferences.md` Index; this table indexes the additional laws specific to this repository.

| Law Title | Scope | Enforcement |
| :--- | :--- | :--- |
| **Single-Transaction Live Coordination Law** | R4 Session Relay | Mandatory for `sesh` |
| **No-Transaction-Across-Event-Dispatch Law** | R4 Session Relay | Mandatory for `sesh` |

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

;;INTENTION("R4 Session Relay: lockless single-transaction live coordination; bounded wait networking; zero transaction across event dispatch.")

---

## 4. Readiness Cross-Reference (Living Documentation Law)

- Feature readiness matrix: [sesh](../../ecosystem/sesh.md), rendered as `[[sesh]]`.
