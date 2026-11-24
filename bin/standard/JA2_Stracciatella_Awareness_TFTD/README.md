# Jagged Alliance 2 Straciatella Awareness (TFTD) for OpenXcom (OXCE)

This mod brings **tactical acoustics, auditory awareness, alert-based patrol investigation, and ~180° peripheral vision** directly into **Terror from the Deep (xcom2)**.

---

## 1. Features Summary

1. **Acoustic Simulation (`acousticSimulation: true`)**: Physical actions emit sound waves that propagate through 3D space with distance attenuation and structural muffling (doors $-6$, walls $-10$).
2. **Dynamic Auditory AI Noise Tracking**: Aliens hearing disturbances log coordinates and investigate.
3. **~180° Peripheral Vision (`peripheralVision: true`)**: Detects movement in a 180° forward arc at 50% max sight range.
4. **3-Tier Alert State Machine**: `STATUS_GREEN` (Patrol), `STATUS_YELLOW` (Investigating noise), `STATUS_RED` (Combat ready).

---

## 2. Related Documentation

* [ja2_stracciatella_awareness_guide.md](../../../docs_ja2s_mod/ja2_stracciatella_awareness_guide.md) — Comprehensive guide on acoustics, decibel attenuation formulas, and the 3-tier alert state machine.
* [ja2_stracciatella_mods_overview.md](../../../docs_ja2s_mod/ja2_stracciatella_mods_overview.md) — Combined overview of all Jagged Alliance 2 Stracciatella mods.
