# Jagged Alliance 2 Straciatella Tactical for OpenXcom (OXCE)
### Tactical Realism, Ballistics, Stances, and Weapon Balancing Inspired by Jagged Alliance 2

> [← Back to Documentation Overview](ja2_stracciatella_mods_overview.md)

---

## 1. Overview & Vision

**Jagged Alliance 2 Straciatella Tactical** bridges the gap between two of the greatest tactical turn-based games ever made: **OpenXcom** and **Jagged Alliance 2**.

While OpenXcom has always possessed an extraordinary **3D physical voxel ballistics engine** with fully destructible environments, its vanilla combat rules feature several notorious design flaws:
- **"Skyranger Ramp Sniping"**: Forward scouts reveal aliens 40–50 tiles away, allowing soldiers sitting safely on the transport craft ramp to eliminate the entire map with zero accuracy penalty.
- **Infinite Effective Range**: Every weapon—from sidearm pistols to heavy cannons—travels infinitely across the map without distance dropoff penalties.
- **Weapon Homogeneity**: Heavy cannons behave like slightly heavier rifles, with no incentive to set up defensive positions, deploy bipods, or brace for fire.
- **Uniform Auto-Burst Accuracy**: Burst fire experiences zero recoil or muzzle climb regardless of stance or weapon caliber.
- **Severe Night Vision Asymmetry**: Aliens possess 20 tiles of sight in pitch darkness while human operatives are capped at 9 tiles with no equipment progression to counter it.

This mod ports **Jagged Alliance 2's core ballistics, recoil, bipod, and tactical optics philosophy** directly into OpenXcom Extended (OXCE) as a completely toggleable, modular ruleset mod.

---

## 2. Complete List of Changes & Mechanics

### 2.1 Beyond-Visual-Range (BVR) & Out-of-Sight Penalty
*Inspired by JA2's `iChance /= 2` rule when targeting outside a soldier's personal sight range.*

- **The Problem in Vanilla**: In vanilla X-Com, line-of-sight is shared globally. If a rookie or hover-tank spots an alien, every soldier on the map can shoot at it as if they were looking directly through a telescopic sight.
- **The JA2 Mechanic**:
  - **Global 50% Accuracy Penalty (`noLOSAccuracyPenaltyGlobal: 50`)**: If a soldier fires on a target that they **cannot personally see with their own eyes** (the alien is beyond their 20-tile day / 9-tile night sight range, or outside their forward visual cone), their effective accuracy is **cut in half**.
  - **In-Game Visual Feedback**: When hovering over a target affected by this penalty, OpenXcom Extended automatically changes the cursor accuracy percentage color from **Green to Yellow**, warning you that the shot lacks direct line-of-sight.
  - **The Sniper Exception**: Dedicated precision rifles equipped with optical sights mitigate or eliminate this penalty, restoring authentic spotter-and-sniper teamwork.

---

### 2.2 Weapon Effective Ranges & Distance Dropoff
*Inspired by JA2's `GunRange` threshold and range dropoff formulas.*

In vanilla X-Com, weapons have no maximum effective range—bullets fly until they hit an obstacle or map edge. This mod introduces realistic maximum effective ranges and per-tile accuracy dropoffs beyond that threshold:

$$\text{Accuracy Loss} = \text{Dropoff} \times (\text{DistanceInTiles} - \text{MaxRange})$$

- **Pistols (`STR_PISTOL`, `STR_LASER_PISTOL`, `STR_PLASMA_PISTOL`)**:
  - Max effective range: **15–18 tiles**.
  - Dropoff beyond max range: **$-3\%$ to $-4\%$ per tile**.
  - Out-of-sight penalty: **$70\%\text{--}75\%$ penalty** (pistols cannot spot-snipe across maps).
- **Rifles (`STR_RIFLE`, `STR_LASER_RIFLE`, `STR_PLASMA_RIFLE`)**:
  - Max effective range: **32–36 tiles**.
  - Dropoff beyond max range: **$-2\%$ per tile**.
  - Out-of-sight penalty: $10\%\text{--}25\%$.
- **Heavy Weapons (`STR_HEAVY_CANNON`, `STR_AUTO_CANNON`, `STR_HEAVY_LASER`, `STR_HEAVY_PLASMA`)**:
  - Max effective range: **40–50 tiles**.
  - Dropoff beyond max range: **$-1\%$ per tile**.

---

### 2.3 Bipod Simulation via Enhanced Kneeling
*Inspired by JA2's `AIM_BONUS_PRONE` (+16%) and mounted `BIPOD` (+50% prone bonus).*

In vanilla X-Com, kneeling grants a generic $+15\%$ accuracy multiplier for all weapons (`kneelBonusGlobal: 115`). 

In this mod:
- **Baseline Kneeling**: Increased to **$+20\%$ (`kneelBonusGlobal: 120`)** to reward basic crouching behind low cover.
- **Heavy Weapon Bipods**: Heavy support weapons receive an enhanced **$+40\%$ to $+45\%$ accuracy multiplier when kneeling** (`kneelBonus: 140–145`).
- **Tactical Impact**: Firing a Heavy Cannon or Auto-Cannon while standing is clumsy and inaccurate on the move (`accuracySnap: 50%`). But when a soldier kneels—simulating deploying the bipod/tripod—their accuracy jumps dramatically, turning them into formidable fire-lane anchors.

---

### 2.4 Recoil & Cumulative Burst Climb
*Inspired by JA2's automatic weapon muzzle climb and recoil formulas.*

- **The Problem in Vanilla**: Auto-burst in OpenXcom fires a burst of 3 rounds where every bullet has identical accuracy, regardless of whether the shooter is standing, running, or shooting a heavy weapon.
- **The JA2 Mechanic (`burstRecoil: true`)**:
  - **1st Bullet**: 100% of base accuracy.
  - **2nd Bullet**: Incurs **$-15\%$ accuracy penalty** (`burstRecoilPenalty: 15`).
  - **3rd Bullet**: Incurs **$-30\%$ accuracy penalty**.
  - **Physical Muzzle Climb (`burstElevationClimb: 1`)**: Each successive shot shifts the aim voxel upward by $+1$ voxel, simulating barrel rise.
  - **Bipod & Kneeling Mitigation**: Deploying a bipod (kneeling with high `kneelBonus`) **completely neutralizes or drastically reduces recoil**.
  - **Strength Mitigation**: High soldier Strength (above 40) directly absorbs recoil shock, reducing the accuracy penalty.

---

### 2.5 Symmetrical Night Vision & Tactical Electro-Optics
*Inspired by JA2's Night Vision Goggles, UV Goggles, and ambient light balance.*

- **The Problem in Vanilla**: Aliens see 20 tiles into pitch darkness while human operatives see only 9 tiles, creating a frustrating and arbitrary night penalty.
- **The JA2 Mechanic (`symmetricalNightVision: true`)**:
  - **Baseline Player Dark Vision**: Increased from 9 to **12 tiles** (`playerBaseViewDistanceAtDark: 12`).
  - **Tactical Electro-Optics Gear**: Carrying tactical illumination/optics items (such as `STR_ELECTRO_FLARE`) grants an additional **$+4$ tiles of dark vision** (`visibilityAtDarkBonus: 4`), allowing a prepared squad to reach 16 tiles of vision in the dark and engage aliens on fair terms.

---

### 2.6 Sub-Voxel Precision Aiming: Aim High (`Shift + Click`)
*Inspired by JA2's head/torso/legs targeted shots.*

- Holding **`Shift`** while aiming elevates the target voxel into the upper head/torso hitbox ($+6$ voxels).
- **Low Cover Clearance**: Allows bullets to clear half-height walls, sandbags, and fences without hitting the barricade.
- **Precision Trade-off**: Applies a **$-15\%$ accuracy penalty** (`aimHighAccuracyPenalty: 15`) reflecting the smaller target silhouette.
- **Visual Feedback**: The cursor displays `[HIGH]` next to accuracy.

---

### 2.7 Pre-Shot Line-of-Fire (LOF) Obstacle Indicator
- **Real-Time 3D Raycasting**: Traces the bullet's exact physical trajectory from weapon barrel to target before firing.
- If an obstacle, fence, corner, or friendly soldier blocks the flight path, the cursor displays **`NO LOF` in Red**, preventing wasted Time Units on blocked shots.

---

### 2.8 Real-Time Cursor Sight & Range Indicators (`rangeVisualCursorTags: true`)
*Clear, intuitive on-screen awareness of sight limits, weapon ranges, and line of fire.*

To provide complete targeting clarity while keeping the tactical battlefield clean and free of terrain clutter (avoiding visual ground ring borders), all distance, vision, and obstruction feedback is integrated directly into the targeting crosshair HUD:

- **`[BVR]`**: Target tile is **Beyond Visual Range** (outside personal line-of-sight / vision cone). Accuracy is halved by the out-of-sight penalty.
- **`[RANGE]`**: Target tile is **Beyond Maximum Effective Range**. Accuracy suffers per-tile distance dropoff.
- **`[BVR+RANGE]`**: Target tile is both beyond personal sight AND beyond maximum effective range.
- **Distance Tag `[dist: X / max: Y]`**: Displays the 3D voxel distance to the target alongside the weapon's maximum effective range (e.g. `[dist: 24/max: 18]`).
- **Pre-Shot Line-of-Fire (LOF)**: The crosshair raycasts the 3D trajectory in real-time, flashing a red **`NO LOF`** warning whenever cover, window frames, or friendlies obstruct the bullet path.

#### Configuration Toggle
Configured in `JA2_Stracciatella_Tactical.rul`:
```yaml
rangeVisualCursorTags: true   # Set to false to hide cursor tags
visualLOFIndicator: true      # Set to false to hide pre-shot LOF warning
```

---

### 2.9 Smart Tactical AI: Weapon Range & Distance Awareness
*In vanilla OpenXcom, aliens evaluate firing modes based on simplistic hardcoded distance thresholds (`dist < 4` for auto, `dist > 12` for aimed). If an alien with a Plasma Pistol (effective range 16) spots an X-COM soldier at 22 tiles, vanilla AI stands in place and fires useless aimed shots.*

With **JA2 Tactical**, smart AI evaluation hooks (`extendedFireModeChoice: true` and `respectMaxRange: true`) are fully activated:
- **Range & Accuracy Calculation Before Firing**: The AI evaluates weapon effective range dropoff and out-of-sight penalties before pulling the trigger. If a target is outside the weapon's maximum range or has 0 accuracy, the AI cancels the attack (`BA_RETHINK`).
- **Tactical Movement & Closing the Distance**: Instead of wasting Time Units on impossible shots, the AI movement logic gets a direct score multiplier favoring tiles that close the distance toward the target (`score = score * distanceToTarget / proposedDistance`). Aliens with short-range weapons (pistols, SMGs, shotguns) aggressively push into effective range before firing!

---

## 3. How It Affects Gameplay (Player Tactics)

1. **Elimination of Ramp-Camping**: You can no longer leave snipers on the Skyranger ramp to shoot across the entire map. Snipers must move to elevated vantage points with direct sightlines or equip dedicated Scoped Rifles.
2. **Heavy Weapon Fire-Lane Anchoring**: Heavy weapons are devastating when entrenched. Position heavy gunners behind low cover, kneel to deploy bipods, and lock down choke points with high-accuracy, zero-recoil suppression bursts.
3. **Burst Fire Discipline**: Spraying auto bursts while standing at long range wastes valuable ammunition. Fire single snap shots on the move, or kneel down to control burst recoil.
4. **Viable Night Infiltration**: Night missions are no longer suicidal horror crawls. Equipping electro-optics and tactical flares allows soldiers to push forward with 16 tiles of night vision.

---

## 4. How It Affects Enemies (Alien Behavior & Balance)

1. **No More Unseen Alien Squad Sniping**:
   - In vanilla, if one Sectoid spots an operative, a distant Muton 40 tiles away can fire with 100% accuracy through the dark.
   - Under this mod, aliens firing without personal line-of-sight suffer the **same 50% accuracy penalty**.
2. **Alien Weapons Obey Range Dropoffs**:
   - The **Plasma Pistol** loses accuracy rapidly beyond 16 tiles ($-4\%$ / tile). Aliens with sidearms must close the distance.
   - The **Plasma Rifle** and **Heavy Plasma** retain superior effective range (34–45 tiles), reflecting advanced alien technology.
3. **Alien Burst Recoil**:
   - Aliens firing automatic bursts while standing experience the same cumulative accuracy penalty, reducing random one-hit team wipes from distant standing Floater bursts.
4. **Smart AI Closes Distance**:
   - Rather than standing at 25 tiles wasting shots with short-range sidearms, aliens actively calculate weapon ranges and dropoffs via `extendedFireModeChoice` and `respectMaxRange`, moving forward to close the distance into lethal range.

---

## 5. Summary Configuration Matrix

| Hook Name | Default (Vanilla) | Mod Value | Effect |
| :--- | :---: | :---: | :--- |
| `noLOSAccuracyPenaltyGlobal` | `0` (off) | `50` | Halves accuracy on targets outside personal line of sight. |
| `kneelBonusGlobal` | `115` | `120` | Baseline kneeling accuracy multiplier (+20%). |
| `allowAimHigh` | `false` | `true` | Enables `Shift + Click` head/upper-profile sub-voxel aiming. |
| `aimHighAccuracyPenalty` | `0` | `15` | Precision penalty when aiming at head/upper torso. |
| `aimHighElevationOffset` | `0` | `6` | Sub-voxel elevation shift in voxels. |
| `visualLOFIndicator` | `false` | `true` | Real-time `NO LOF` crosshair trajectory indicator. |
| `burstRecoil` | `false` | `true` | Enables cumulative burst climb and recoil penalty. |
| `burstRecoilPenalty` | `0` | `15` | Accuracy penalty per consecutive shot in burst. |
| `burstElevationClimb` | `0` | `1` | Upward barrel rise per bullet in voxels. |
| `symmetricalNightVision` | `false` | `true` | Enhances human night vision via baseline and gear bonuses. |
| `playerBaseViewDistanceAtDark` | `9` | `12` | Baseline operative sight in darkness. |
| `rangeVisualCursorTags` | `false` | `true` | Informative cursor status tags (`[BVR]`, `[RANGE]`, `[dist: X/max: Y]`). |
| `ai.extendedFireModeChoice` | `false` | `true` | AI computes actual hit chances & range dropoffs before shooting. |
| `ai.respectMaxRange` | `false` | `true` | Forbids firing past max range; AI advances to close the distance. |

---

## 6. Terror from the Deep (TFTD) Support & Weapon Specifications

The companion mod **`JA2_Stracciatella_Tactical_TFTD`** applies this full ballistics and optics philosophy to *Terror from the Deep* (`master: xcom2`).

### 6.1 TFTD Arsenal Balance Table

| Weapon | Type | Max Range | Dropoff / Tile | Out-of-Sight Penalty | Kneel Bonus (Bipod) | Recoil Absorption |
| :--- | :--- | :---: | :---: | :---: | :---: | :---: |
| **Dart Gun / Pistol** | Underwater Sidearm | 14–16 tiles | $-4\%$ | $70\%$ | $+20\%$ (Baseline) | Standard |
| **Harpoon Gun** | Compact Carbine | 28 tiles | $-3\%$ | $40\%$ | $+25\%$ | Moderate |
| **Gas Cannon** | Heavy Ballistic Support | 40 tiles | $-2\%$ | $25\%$ | **$+40\%$ (Bipod)** | Full when kneeling |
| **Hydro-Jet Cannon** | Aquatic Assault LMG | 35 tiles | $-2\%$ | $25\%$ | **$+35\%$ (Bipod)** | Full when kneeling |
| **Torpedo Launcher** | Heavy Ordinance | 45 tiles | $-1\%$ | $20\%$ | **$+40\%$ (Bipod)** | Full |
| **Gauss Pistol** | Advanced Sidearm | 18 tiles | $-3\%$ | $70\%$ | $+20\%$ | Standard |
| **Gauss Rifle** | Battle Rifle | 34 tiles | $-2\%$ | $25\%$ | $+25\%$ | Moderate |
| **Heavy Gauss** | Support Gun | 42 tiles | $-1\%$ | $20\%$ | **$+40\%$ (Bipod)** | Full when kneeling |
| **Sonic Pistol** | Alien Energy Pistol | 20 tiles | $-3\%$ | $60\%$ | $+20\%$ | Standard |
| **Sonic Blaster** | Alien Carbine | 36 tiles | $-2\%$ | $20\%$ | $+30\%$ | High |
| **Sonic Cannon** | Alien Heavy Cannon | 45 tiles | $-1\%$ | $15\%$ | **$+45\%$ (Bipod)** | Full when kneeling |
| **Disrupter Pulse** | Guided Launcher | 50 tiles | $-1\%$ | $15\%$ | **$+45\%$ (Bipod)** | Full |

### 6.2 Tactical Illumination: Chemical Flares
* Carrying a `STR_CHEMSIGHT` (Chemical Flare) grants **$+4$ tiles of vision in the dark** (`visibilityAtDarkBonus: 4`), boosting human diver vision to 16 tiles to match hostile deep-sea predators.

---

## 7. How to Enable / Disable

1. Start **OpenXcom** (`make ufo`, `make tftd`, or `make openxcom`).
2. Go to **Options** $\rightarrow$ **Mods**.
3. Under the active master mod:
   - For **UFO / xcom1**: Check `[X] Jagged Alliance 2 Straciatella Tactical`
   - For **TFTD / xcom2**: Check `[X] Jagged Alliance 2 Straciatella Tactical TFTD`
4. Click **OK** (uncheck at any time to restore 100% vanilla mechanics).

---

## 8. In-Game GUI Options: Options $\rightarrow$ Advanced $\rightarrow$ JA2S

Whenever any **Jagged Alliance 2 Stracciatella** mod is enabled, an in-game configuration tab labeled **`JA2S`** appears directly at the top of the **Options $\rightarrow$ Advanced** screen, positioned next to `OXC` and `OXCE`:

```text
[ OXC ]   [ OXCE ]   [ JA2S ]
```

### In-Game JA2S Settings List
Under the **Battlescape** category in the `JA2S` tab, players can click to toggle settings in real-time:
* **Target Elevation / Aim High**: `YES` / `NO` (`ja2sAllowAimHigh` — toggles <kbd>Shift</kbd> aim high precision offset).
* **Cursor Sight & Range Tags**: `YES` / `NO` (`ja2sRangeVisualCursorTags` — toggles `[BVR]`, `[RANGE]`, and `[dist: X/max: Y]` crosshair tags).
* **Pre-Shot Line-of-Fire (LOF)**: `YES` / `NO` (`ja2sVisualLOFIndicator` — toggles real-time `NO LOF` indicator on blocked trajectories).
* **Suppression UI Alerts**: `YES` / `NO` (`ja2sSuppressionVisualFeedback` — toggles HitLog pinned warnings).

> [!NOTE]
> **Vanilla Safety**: If a specific JA2 mod for a given setting is disabled in your mod list, that setting automatically defaults to `NO`, is greyed out, and cannot be clicked. This guarantees that your settings never affect vanilla gameplay when mods are turned off.
