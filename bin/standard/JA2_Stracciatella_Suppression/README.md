# Jagged Alliance 2 Straciatella Suppression for OpenXcom (OXCE)

This mod brings **bullet suppression, pinning fire, reaction disruption, forced defensive stances, and combat UI notifications** from Jagged Alliance 2 into OpenXcom.

---

## 1. Features Summary

1. **3D Voxel Trajectory Suppression (`nearMissSuppression: true`)**:
   - Traces bullet flight paths in real-time through the 3D voxel grid.
   - Near-misses passing within 1 tile (`nearMissRadius: 1`) apply suppression points to nearby units based on weapon caliber:
     - Sidearms & Pistols: **1 point** per bullet
     - Infantry Rifles: **2 points** per bullet
     - Auto-Cannons: **3 points** per bullet
     - Heavy Cannons & Heavy Plasma: **4 points** per bullet
     - Rocket Launchers: **5 points** per rocket
2. **Time Unit Penalties & Pinning**:
   - Suppressed units lose up to **25% of their Time Units** on their next turn (`suppressionMaxTUPenalty: 25`).
   - Pinned enemies cannot easily peek, fire an aimed shot, and retreat back into cover.
3. **Immediate Overwatch Disruption**:
   - Every near-miss bullet deducts from the target's current TUs in real-time, directly degrading their reaction score and breaking deadly alien reaction overwatch.
4. **Forced Defensive Kneeling ("Hit the Dirt!") (`suppressionKneelThreshold: 3`)**:
   - Standing units that accumulate 3 or more suppression points immediately drop down (forced kneel), shrinking their vertical profile from 22 to 14 voxels to take shelter behind waist-high cover.
5. **Morale Shock (`suppressionMoraleLoss: 3`)**:
   - Heavy incoming fire erodes target morale by -3 per threshold, accelerating panic or retreat on unsupported units.
6. **Combat UI Notifications (`suppressionVisualFeedback: true`)**:
   - Logs `<Unit> IS PINNED DOWN!` to the Battlescape HitLog when a unit is pinned, providing instant confirmation of broken overwatch.

---

## 2. In-Game GUI Options: Options -> Advanced -> JA2S

Whenever this mod is enabled, configuration options appear under the **`JA2S`** tab in **Options -> Advanced**:
* **Suppression UI Alerts**: `YES` / `NO` (toggles on-screen HitLog notifications when units are pinned).

---

## 3. How It Affects Gameplay & Enemies

* **Player Gameplay**: Heavy weapons (Heavy Cannon, Auto-Cannon, Heavy Laser) become essential area-denial tools. Laying down a covering burst neutralizes alien snipers and allows friendly operatives to safely cross open terrain or breach guarded doors.
* **Enemy Aliens**: Pinned aliens abort aggressive charges and prioritize hard defensive cover (+80% ambush preference, -40% combat odds). Terrifying reaction snipers (e.g., Mutons, Sectopods) can be suppressed to break their overwatch without needing direct hits.

---

## 4. Related Documentation

* [ja2_stracciatella_suppression_guide.md](../../../docs_ja2s_mod/ja2_stracciatella_suppression_guide.md) — Complete guide on near-miss tracking, TU penalties, and tactical suppression applications.
* [ja2_stracciatella_mods_overview.md](../../../docs_ja2s_mod/ja2_stracciatella_mods_overview.md) — Combined overview of all Jagged Alliance 2 Stracciatella mods vs. vanilla OpenXcom.
