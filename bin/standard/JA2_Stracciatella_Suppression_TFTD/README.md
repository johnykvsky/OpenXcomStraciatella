# Jagged Alliance 2 Straciatella Suppression (TFTD) for OpenXcom (OXCE)

This mod brings **projectile suppression, pinning fire, reaction disruption, forced defensive stances, and combat UI notifications** from Jagged Alliance 2 into **Terror from the Deep (xcom2)**.

---

## 1. Features Summary

1. **3D Voxel Trajectory Suppression (`nearMissSuppression: true`)**: Traces projectile flight paths in 3D voxel space; near-misses apply suppression points.
2. **Time Unit Penalties & Pinning**: Suppressed units lose up to **$25\%$ of their Time Units** on their next turn.
3. **Immediate Overwatch Disruption**: Deducts current TUs, breaking deadly alien reaction overwatch mid-stride.
4. **Forced Defensive Kneeling ("Hit the Dirt!") (`suppressionKneelThreshold: 3`)**: Standing units accumulating 3+ suppression points drop to their knees.
5. **Morale Shock (`suppressionMoraleLoss: 3`)**: Heavy incoming bursts accelerate panic on unsupported units.

---

## 2. Related Documentation

* [ja2_stracciatella_suppression_guide.md](../../../docs_ja2s_mod/ja2_stracciatella_suppression_guide.md) — Complete guide on near-miss tracking, TU penalties, and tactical suppression applications.
* [ja2_stracciatella_mods_overview.md](../../../docs_ja2s_mod/ja2_stracciatella_mods_overview.md) — Combined overview of all Jagged Alliance 2 Stracciatella mods.
