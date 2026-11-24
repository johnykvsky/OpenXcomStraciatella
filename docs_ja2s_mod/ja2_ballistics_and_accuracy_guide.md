# Jagged Alliance 2 Straciatella Ballistics & Accuracy Guide
### In-Depth Analysis of Point-Blank Mechanics, UFOextender Accuracy, and the JA2 Tactical Ballistics Pipeline

> [← Back to Documentation Overview](ja2_stracciatella_mods_overview.md)

---

## 1. Executive Summary & Design Philosophy

Classic **X-Com: UFO Defense (1994)** and **Jagged Alliance 2 (1999)** share turn-based tactical roots, but their approach to ballistics, accuracy, and engagement range could not be more different:

* **Vanilla X-Com (1994)**:
  * Features a **pure physical 3D voxel ballistics engine**, but with **flat, distance-blind accuracy**.
  * A soldier with a 45% Snap Shot has the exact same 45% chance to hit a target whether the alien is standing 1 tile away or 40 tiles across the map.
  * Weapons have infinite effective range and zero distance dropoff.
* **Jagged Alliance 2 (Stracciatella)**:
  * Features a **dynamic Chance-to-Hit (CTH) curve** that heavily factors physical distance, weapon effective range, barrel length, stance bracing (bipods), and aiming elevation.
  * At point-blank range (1–3 tiles), shooting with an appropriate firearm provides nearly 100% accuracy.
  * Shooting beyond weapon effective range causes accuracy to rapidly degrade.

The **JA2 Stracciatella Tactical** system bridges these philosophies within OpenXcom Extended (OXCE), implementing realistic weapon effective ranges, stance bonuses, recoil climb, anti-ramp sniping penalties, and sub-voxel elevation targeting.

---

## 2. The Point-Blank Paradox: Why Vanilla X-Com Misses at 1 Tile

One of the most infamous experiences in vanilla X-Com is having a trained soldier miss an alien standing directly in the adjacent tile. This is not a dice-roll bug, but a mathematical consequence of the original 1994 physical ballistics model:

```
[Shooter's Rifle Barrel] (offset right, chest height)
         \
          \  (15° angular miss scatter)
           \
            X ----> Misses past alien's shoulder into the floor!
          [Alien Hitbox] (only 8-10 voxels wide)
```

1. **Distance-Blind Accuracy**:
   Vanilla X-Com does not give any accuracy bonus for being close. If a rookie with 45% accuracy rolls an 80, the shot is a miss—regardless of proximity.
2. **The "Miss Cloud" Deviation**:
   When a shot misses, the engine calculates an angular scatter: `deviation = roll - accuracy + 50`.
3. **Physical 3D Voxel Raycasting**:
   * Bullets do not originate from the center of the tile; they emerge from the soldier's rifle muzzle (an offset 3D voxel coordinate).
   * In an adjacent tile (16 voxels away), an alien's 3D voxel collision cylinder is only about 8–10 voxels wide.
   * An angular deviation of just 15°–25° shifts the bullet by 5–7 voxels sideways or downward.
   * **Result**: The bullet physically bypasses the alien's shoulder or strikes the dirt between their feet, registering a clean miss despite standing nose-to-nose.

---

## 3. How Jagged Alliance 2 Handles Point-Blank Combat

In *Jagged Alliance 2*, missing a point-blank shot with a handgun or shotgun is virtually impossible:

1. **Massive Close-Range CTH Multiplier**:
   JA2 calculates CTH using a distance-to-range ratio:
   `Range Ratio = Distance in Tiles / Weapon Effective Range`
   At distances <= 3 tiles, JA2 applies a massive point-blank bonus (often +50% to +75% CTH). Even a low-marksmanship mercenary pointing a pistol or shotgun at an adjacent target reaches the **99% CTH hard cap**.
2. **Physical Weapon Length & Handling**:
   JA2 distinguishes weapon types by physical length:
   * **Pistols & SMGs**: Highly maneuverable in tight quarters. At 1 tile, they deliver lethal, instant hits with zero penalty.
   * **Long Rifles & Sniper Rifles**: Long-barreled weapons suffer an unwieldy penalty when standing chest-to-chest with an active enemy. In close-quarters combat, the enemy can attempt a martial-arts disarm or push the barrel aside.

---

## 4. UFOextender Accuracy vs. JA2 Stracciatella Tactical

A common question is whether the built-in **`UFOextender Accuracy`** option and our **`JA2 Stracciatella Tactical`** mod interfere, duplicate penalties, or do the same work in two places.

**The Answer: They do not interfere or duplicate work.**

The OpenXcom engine processes distance limits in a single centralized function: `RuleItem::calculateLimits()` in `Projectile.cpp`. Here is the exact architectural breakdown:

| Capability | UFOextender Accuracy | JA2 Stracciatella Tactical | How They Interact |
| :--- | :---: | :---: | :--- |
| **Enables Distance-Based Scaling** | ✅ Activates engine dropoff logic | ❌ Relies on engine dropoff hook | UFOextender turns on the engine calculation. |
| **Supplies Weapon Ballistic Data** | ❌ Vanilla weapons have no limits | ✅ Sets `maxRange` and `dropoff` per weapon | Our mod defines the actual ballistic numbers (e.g., Pistol: 15 tiles, Rifle: 34 tiles). |
| **Point-Blank Range Boost** | ✅ Replaces flat accuracy with range scaling | ✅ Inherits distance scaling | Resolves the point-blank miss problem naturally. |
| **BVR / Out-of-Sight Penalty** | ❌ No concept of personal LOS | ✅ Slashes accuracy by 50% on unseen targets | **Distinct**: Prevents Skyranger ramp sniping. |
| **Aim High Elevation (<kbd>Shift</kbd>)** | ❌ No elevation manipulation | ✅ Lifts target voxel +6 over low cover/sills | **Distinct**: Solves window/half-cover obstruction. |
| **Burst Recoil & Climb** | ❌ No burst recoil mechanics | ✅ Consecutive rounds suffer -15% & climb | **Distinct**: Penalizes undisciplined automatic fire. |
| **Bipod Bracing via Kneeling** | ❌ Flat kneel bonus (+15%) | ✅ Heavy weapons gain up to +45% & negate recoil | **Distinct**: Encourages defensive, fortified positions. |

---

## 5. The Unified 6-Step Ballistics Resolution Pipeline

When an operative pulls the trigger, the engine executes a clean, sequential 6-step pipeline. Every modifier handles a distinct physical factor without overlapping:

```
[Step 1: Base Accuracy]
       |
       v
[Step 2: Aim High Check] ---------> Shift held? (-15% precision, +6 voxel elevation)
       |
       v
[Step 3: Burst Recoil Check] ------> Auto burst shot #2, #3? (-15%/shot, cancelled by Bipod/Str)
       |
       v
[Step 4: Distance Dropoff] --------> Distance > maxRange? (Dropoff deducted ONCE via calculateLimits)
       |
       v
[Step 5: BVR / LOS Check] ---------> Target not seen by shooter? (Accuracy slashed by 50%)
       |
       v
[Step 6: Physical Voxel Raycast] --> Bullet flies! Near-misses apply 3D Suppression within 1 tile.
```

### Detailed Pipeline Stages:

1. **Step 1: Base Firing Accuracy**
   `Accuracy_1 = Soldier Firing Stat * Weapon Mode Multiplier * Stance Bonus`
   * Kneeling with heavy weapons (`kneelBonus: 145`) significantly increases this base value.
2. **Step 2: Aim High (<kbd>Shift</kbd>) Precision Adjustment**
   * If <kbd>Shift</kbd> is held and `allowAimHigh` is enabled:
     `Accuracy_2 = Accuracy_1 * (100 - aimHighAccuracyPenalty) / 100 = Accuracy_1 * 0.85`
   * The target voxel elevation is raised by +6 voxels toward the target's head.
3. **Step 3: Cumulative Burst Recoil & Climb**
   * If firing an Auto Burst (`BA_AUTOSHOT`):
     `Recoil Penalty = (Shot Index - 1) * 15%`
   * **Mitigation**: Deploying a bipod (kneeling with high `kneelBonus`) or possessing high soldier strength (> 40) offsets this penalty.
   * Physical muzzle rise elevates subsequent projectiles upward.
4. **Step 4: Distance Dropoff (Single Execution)**
   * Computed via `RuleItem::calculateLimits()`:
     `Accuracy Loss = Dropoff * (Distance - MaxRange)`
   * Only applies when distance exceeds the weapon's configured maximum effective range.
5. **Step 5: Beyond-Visual-Range (BVR) Penalty**
   * If the target is spotted by a teammate or sensor but is outside the shooter's personal line-of-sight:
     `Accuracy_5 = Accuracy_4 * (noLOSAccuracyPenalty / 100) = Accuracy_4 * 0.50`
6. **Step 6: 3D Physical Projectile Simulation & Suppression**
   * The bullet is cast through the 3D voxel grid.
   * If the bullet strikes an obstacle, it impacts and deals damage.
   * Any bullet passing within 1 tile of a unit applies **near-miss suppression**, depleting enemy TUs and forcing units into cover.

---

## 6. Case Study: The Window & Half-Cover Collision

A classic tactical scenario illustrates the difference between standard targeting and the JA2 Aim High system:

```
[Your Soldier]          [Window Wall]          [Alien]
      O                      | |                  O
     /|\        =============================    /|\
     / \        [Solid Wall]     [Solid Sill]    / \
=========================================================
```

### The Problem Without Shift (Vanilla LOF):
* A window tile consists of a solid sill (Z = 0–10), an open window gap (Z = 10–18), and a ceiling lintel (Z = 18–24).
* The default engine raycaster (`canTargetUnit`) aims from the gun barrel toward the **center of mass (torso)** of the enemy.
* At close range across a window, the direct line to the enemy's chest clips the top edge of the **solid window sill**.
* **Result**: The engine reports `NO LOF`. You cannot shoot, or if forced with `Ctrl+Click`, your bullet destroys the wall sill beneath the window.

### The Solution With Aim High (<kbd>Shift</kbd>):
* Holding <kbd>Shift</kbd> elevates the target coordinate by +6 voxels toward the alien's **head/upper silhouette**.
* The elevated trajectory passes cleanly through the **open glass aperture** of the window, completely clearing the sill.
* **Result**: `foundLoF = true`. The shot is allowed, displaying the accuracy percentage and `[HIGH]` on the cursor.

---

## 7. Tactical Recommendations for Players

1. **Point-Blank Engagements**:
   * With `UFOextender Accuracy` enabled, use Pistols and Shotguns for room-clearing and doorway ambushes. They enjoy high point-blank accuracy and low TU costs.
2. **Defensive Machine Gun Nests**:
   * Always **kneel** with heavy support weapons (Heavy Cannon, Auto-Cannon, Heavy Laser). The +45% bipod bonus eliminates burst recoil and allows pinpoint fire lanes across long corridors.
3. **Overcoming Half-Cover & Windows**:
   * Whenever an enemy is tucked behind low fences, sandbags, or window sills, hold <kbd>Shift</kbd> to aim high. The -15% precision trade-off is far better than an impossible `NO LOF` obstructed shot.
4. **Suppressing Snipers**:
   * When dealing with entrenched alien snipers at extreme distances, lay down automatic fire. Even if distance dropoff reduces direct hit chances, near-miss suppression strips their reaction TUs and forces them to dive into cover.
