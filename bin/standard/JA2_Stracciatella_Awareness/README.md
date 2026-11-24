# Jagged Alliance 2 Straciatella Awareness for OpenXcom (OXCE)

This mod brings **tactical acoustics, auditory awareness, alert-based patrol investigation, and ~180° peripheral vision** directly into OpenXcom.

---

## 1. Features Summary

1. **Physical Sound Propagation (`acousticSimulation: true`)**:
   - Physical actions emit sound waves that travel through 3D space with distance attenuation ($-1$ volume per tile in open air).
   - Structural barriers muffle sound: **$-6$ volume** through closed doors (`acousticDoorAttenuation: 6`) and **$-10$ volume** through solid walls (`acousticWallAttenuation: 10`).
   - Action noise profiles: Walking (6 tiles), Running (14 tiles), Door Creak (4 tiles), Smashed Door (15 tiles), Rifle Fire (35 tiles), Heavy Explosions (55 tiles).
2. **Dynamic Auditory AI Noise Tracking**:
   - When an enemy or player makes noise that reaches an operative or guard (`effVolume > 0`), the hearing unit records the 3D map coordinates (`_noiseTarget`).
   - Guards in `STATUS_YELLOW` or `STATUS_RED` suspend regular patrol routes to actively pathfind toward and investigate the disturbance.
3. **Realistic ~180° Peripheral Vision (`peripheralVision: true`)**:
   - Replaces the unnatural 90° tunnel vision with a realistic forward half-plane (~180° arc).
   - Detects movement, shadows, and weapon flashes on flanks at **$50\%$ of normal sight range** (`peripheralVisionRatio: 0.5`).
4. **3-Tier Alert State Machine**:
   - `STATUS_GREEN` (Patrol / Unaware): Standard unalerted patrol routines; guards walk looking straight ahead.
   - `STATUS_YELLOW` (Suspicious / Investigating): Triggered by hearing non-combat noise or peripheral movement. Guards log sound coordinates, break patrol, investigate, and scan the area. Decays after 2 quiet turns.
   - `STATUS_RED` (Combat Alert / Engaged): Triggered by direct visual contact, gunfire, or explosions. Guards seek hard cover, prioritize ambushes, and hold reaction TUs. Decays after 4 quiet turns.

---

## 2. How It Affects Gameplay & Enemies

* **Player Gameplay**: Enables genuine tactical stealth infiltration (walking quietly to stack up outside doors), creating sound distractions to lure guards into kill-zones, and awareness of threats on flanks. Operatives can no longer be blindly ambushed from 91° flank angles.
* **Enemy Aliens**: Aliens break mindless patrol paths to actively investigate noises, seek defensive ambush cover when gunfire is heard, and react to operatives attempting to sprint past their flanks.

---

## 3. Related Documentation

* [ja2_stracciatella_awareness_guide.md](../../../docs_ja2s_mod/ja2_stracciatella_awareness_guide.md) — Comprehensive guide on acoustics, decibel attenuation formulas, and the 3-tier alert state machine.
* [ja2_stracciatella_mods_overview.md](../../../docs_ja2s_mod/ja2_stracciatella_mods_overview.md) — Combined overview of all Jagged Alliance 2 Stracciatella mods vs. vanilla OpenXcom.
