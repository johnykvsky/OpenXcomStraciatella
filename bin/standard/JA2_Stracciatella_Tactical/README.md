# Jagged Alliance 2 Straciatella Tactical for OpenXcom (OXCE)

This mod brings the tactical ballistics, weapon ranges, stance dynamics, pre-shot obstacle awareness, burst recoil climb, and night electro-optics from **Jagged Alliance 2 (Stracciatella)** into **OpenXcom Extended (OXCE)**.

---

## 1. Features Summary

1. **Pre-Shot Line-of-Fire (LOF) Obstacle Indicator (`visualLOFIndicator: true`)**:
   - Raycasts the 3D projectile trajectory in real-time before firing.
   - Displays a red `NO LOF` crosshair indicator when cover, walls, or friendly units block the bullet's flight path.
2. **Target Elevation Toggle: Aim High (`allowAimHigh: true`)**:
   - Holding <kbd>Shift</kbd> elevates the target voxel by $+6$ toward the head/upper silhouette.
   - Clears waist-high obstacles (sandbags, fences, window sills) with a balanced $-15\%$ precision trade-off (`aimHighAccuracyPenalty: 15`).
3. **Beyond-Visual-Range (BVR) / Out-of-Sight Penalty (`noLOSAccuracyPenaltyGlobal: 50`)**:
   - Slashes firing accuracy by **$50\%$** if the shooter does not have personal visual line-of-sight on the target.
   - Eliminates Skyranger "ramp-sniping" where distant scouts reveal aliens for backline shooters.
   - Changes the targeting reticle to **Amber** and tags it with `[BVR]`. Dedicated precision/scoped rifles soften this penalty.
4. **Weapon Effective Ranges & Distance Dropoffs**:
   - **Pistols & Sidearms**: Max effective range 15–18 tiles; $-3\%$ to $-4\%$ accuracy penalty per tile beyond max range.
   - **Infantry Rifles**: Max effective range 32–36 tiles; $-2\%$ dropoff per tile beyond max range.
   - **Heavy Support Weapons**: Max effective range 40–50 tiles; $-1\%$ dropoff per tile beyond max range.
5. **Bipod Simulation via Enhanced Kneeling (`kneelBonus: 140–145`)**:
   - Kneeling with heavy support weapons (Heavy Cannon, Auto-Cannon, Heavy Laser) provides up to **$+45\%$ accuracy**, simulating deployed bipod/tripod stability.
6. **Recoil & Cumulative Burst Climb (`burstRecoil: true`)**:
   - Successive shots in an automatic burst suffer cumulative accuracy penalties ($-15\%$ for 2nd shot, $-30\%$ for 3rd shot) and climb $+1$ voxel upward.
   - Fully mitigated by kneeling (deploying bipod) or high operative strength ($> 40$).
7. **Symmetrical Night Vision & Tactical Electro-Optics (`symmetricalNightVision: true`)**:
   - Increases baseline human night vision to **12 tiles** (`playerBaseViewDistanceAtDark: 12`).
   - Equipping tactical electro-optics items (such as `STR_ELECTRO_FLARE`) grants **$+4$ tiles** of bonus vision, allowing a prepared squad to reach 16 tiles in darkness.
8. **Real-Time Cursor Sight & Range Indicators (`rangeVisualCursorTags: true`)**:
   - Crosshair displays real-time status tags (`[BVR]`, `[RANGE]`, `[BVR+RANGE]`) and distance readout `[dist: X/max: Y]`.
   - Real-time `NO LOF` indicator flashes in red when obstacles block the line of fire.
   - Clean HUD integration avoids visual clutter and terrain ground rings.

---

## 2. In-Game GUI Options: Options $\rightarrow$ Advanced $\rightarrow$ JA2S

Whenever this mod is enabled, configuration toggles appear under the **`JA2S`** tab in **Options $\rightarrow$ Advanced**:
* **Target Elevation / Aim High**: `YES` / `NO` (`ja2sAllowAimHigh` — toggles <kbd>Shift</kbd> aim high elevation targeting).
* **Cursor Sight & Range Tags**: `YES` / `NO` (`ja2sRangeVisualCursorTags` — toggles `[BVR]`, `[RANGE]`, `[dist: X/max: Y]`).
* **Pre-Shot Line-of-Fire (LOF)**: `YES` / `NO` (`ja2sVisualLOFIndicator` — toggles red `NO LOF` indicator on obstructed shots).

---

## 3. How It Affects Gameplay & Enemies

* **Player Gameplay**: Encourages establishing fortified fire-lanes, kneeling with heavy support weapons to deploy bipods, using sidearms for indoor room-clearing, and conducting methodical night advances with electro-optics.
* **Enemy Aliens**: Symmetrically cuts alien accuracy by $50\%$ when firing without personal line-of-sight, prevents cross-map plasma pistol snipes, and penalizes standing alien burst fire.

---

## 4. Related Documentation

* [ja2_stracciatella_tactical_guide.md](../../../docs_ja2s_mod/ja2_stracciatella_tactical_guide.md) — Full tactical guide covering ballistics, stances, and formulas.
* [ja2_ballistics_and_accuracy_guide.md](../../../docs_ja2s_mod/ja2_ballistics_and_accuracy_guide.md) — Comprehensive architectural breakdown comparing 1994 X-Com ballistics, UFOextender accuracy, and the 6-step resolution pipeline.
* [ja2_stracciatella_mods_overview.md](../../../docs_ja2s_mod/ja2_stracciatella_mods_overview.md) — Combined overview of all Jagged Alliance 2 Stracciatella mods vs. vanilla OpenXcom.
