# Jagged Alliance 2 Straciatella Tactical (TFTD) for OpenXcom (OXCE)

This mod brings tactical ballistics, effective weapon ranges, stance dynamics, pre-shot obstacle awareness, burst recoil climb, and symmetrical night vision to **Terror from the Deep (xcom2)**.

---

## 1. Features Summary

1. **Pre-Shot Line-of-Fire Indicator (`visualLOFIndicator: true`)**: Raycasts trajectory and displays `NO LOF` when obstacles obstruct line of fire.
2. **Aim High (`allowAimHigh: true`)**: Hold <kbd>Shift</kbd> to elevate target voxel by +6 voxels to clear waist-high cover at a -15% accuracy cost.
3. **Beyond-Visual-Range (BVR) Penalty (`noLOSAccuracyPenaltyGlobal: 50`)**: -50% accuracy penalty on targets outside personal sight.
4. **Weapon Effective Ranges**:
   - Dart / Gauss / Sonic Pistols: 15–18 tiles (-3% to -4% dropoff).
   - Jet Harpoon / Gauss Rifle / Sonic Blasta Rifle: 32–36 tiles (-2% dropoff).
   - Heavy Support Weapons (Gas Cannon, Hydro-Jet, Heavy Gauss, Sonic Cannon): 40–48 tiles (-1% dropoff).
5. **Bipod Simulation via Enhanced Kneeling (`kneelBonus: 140–145`)**: Kneeling with heavy support weapons grants up to +45% accuracy.
6. **Burst Recoil (`burstRecoil: true`)**: Cumulative accuracy loss (-15% per shot) and muzzle climb on automatic bursts.
7. **Symmetrical Night Vision (`symmetricalNightVision: true`)**: 12 tiles base human night vision; 16 tiles when holding a Chemical Flare (`STR_CHEMICAL_FLARE`).

---

## 2. Related Documentation

* [ja2_stracciatella_tactical_guide.md](../../../docs_ja2s_mod/ja2_stracciatella_tactical_guide.md) — Tactical guide covering ballistics, stances, and formulas.
* [ja2_stracciatella_mods_overview.md](../../../docs_ja2s_mod/ja2_stracciatella_mods_overview.md) — Combined overview of all Jagged Alliance 2 Stracciatella mods.
