# Aiming, Shooting, Line of Sight, and Squad-Spotting: A Detailed Technical Comparison of Jagged Alliance 2 (Stracciatella) and OpenXcom

> [← Back to Documentation Overview](ja2_stracciatella_mods_overview.md)

This document provides a comprehensive technical breakdown of how weapon aiming, ballistics, hit calculations, vision (Line of Sight), and long-range squad-spotting work in **Jagged Alliance 2 (Stracciatella v0.22.1)** and **OpenXcom**.

---

## 1. Core Architectural Paradigm: Probability Check vs. 3D Voxel Ballistics

| Feature | Jagged Alliance 2 (Stracciatella) | OpenXcom |
| :--- | :--- | :--- |
| **Simulation Model** | **Hybrid RPG stat check + 3D obstacle tracing** | **Pure 3D voxel physical trajectory simulation** |
| **Hit Check Mechanism** | Evaluates a 1%–99% Chance-To-Hit (CTH) percentage; rolls `1d100`. | **No d100 hit/miss check**. Accuracy directly scales a 3D conical dispersion envelope. |
| **What a "Miss" Means** | If `DiceRoll > CTH`, an angular deflection is calculated based on how badly the roll failed, casting a stray bullet along that altered trajectory. | The trajectory point is deflected within the accuracy cone. If it strikes an obstacle or passes around the unit's voxel box, it is a miss. |
| **Can a Miss Still Hit?** | Yes, if a deflected stray bullet strikes another unit or bystander. | Yes. A shot with 0% accuracy or bad roll can still strike a huge unit (e.g. Cyberdisc/Sectopod) or point-blank enemy if their voxel hitbox intersects the cone. |
| **Blind Fire / Terrain Targeting** | Allowed, but heavily penalized by blind fire penalties. | Freely allowed on any visible or fog-of-war tile, ground, or wall. |

---

## 2. Hit Chance & Aiming Mechanics

### 2.1 Jagged Alliance 2 (Stracciatella v0.22.1)
*Source references:*
- [`Weapons.cc`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/ja2-stracciatella-0.22.1/src/game/Tactical/Weapons.cc#L1939-L2507) — `CalcChanceToHitGun`
- [`Weapons.cc`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/ja2-stracciatella-0.22.1/src/game/Tactical/Weapons.cc#L672-L830) — `FireWeapon`
- [`LOS.cc`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/ja2-stracciatella-0.22.1/src/game/Tactical/LOS.cc#L3074-L3130) — `CalculateFiringIncrements`

#### Hit Chance Formula
In JA2, Chance-To-Hit is an integer percentage clamped strictly between **1% (`MINCHANCETOHIT`)** and **99% (`MAXCHANCETOHIT`)**:
1. **Base Marksmanship & Gun Status**:
   - Uses `EffectiveMarksmanship(pSoldier)`.
   - Modified by morale and penalized by fatigue.
   - If the weapon's condition is lower than Marksmanship:
     `Base CTH = (EffectiveMarksmanship + GunCondition) / 2`
2. **Aiming Time (Continuous AP Investment)**:
   - Players can right-click the targeting cursor to spend additional Action Points (APs), adding up to 4 aiming rings.
   - Each spent aiming AP adds a flat bonus:
     `Bonus = AIM_BONUS_PER_AP * AimTime`
     *(Typically +10% to +12% per aiming click, provided the target is within sight).*
3. **Targeted Body Parts**:
   - **Torso**: Baseline target size (no extra modifier).
   - **Head**: Scaled distance penalty: -3% per tile of distance (`3 * iSightRange / 10`). Much harder at long range, but ignores body armor and deals severe critical damage.
   - **Legs**: Scaled distance penalty: -1% per tile of distance (`iSightRange / 10`).
4. **Stance & Attachments**:
   - **Crouch**: Bonus up to +8% (`AIM_BONUS_CROUCHING`).
   - **Prone**: Bonus up to +16% (`AIM_BONUS_PRONE`). A mounted **Bipod** increases this prone bonus by up to +50%.
   - **Sniper Scope**: When aiming (`ubAimTime > 0`) at targets beyond minimum scope range (`MIN_SCOPE_RANGE`), each aim AP reduces the perceived distance by 20% (up to an 80% distance reduction), drastically mitigating range penalties.
   - **Laser Scope**: Flat bonus (up to +20% in dim light, scaled down in full daylight).
5. **Distance & Gun Effective Range**:
   - Weapon effective range penalty:
     `Penalty = ((GunMaxRange - Distance * 3) * 10) / (17 * 10)`
   - Sight distance modifier: `+3% * (8 tiles - EffectiveSightRange)`.
6. **Target Status Penalties**:
   - Target movement: Up to -30% penalty based on tiles the target moved in their turn.
   - Target dodge: If the target sees the shooter, the target's Agility and Experience reduce the shooter's CTH unless countered by the shooter's Dexterity and Experience.
7. **Threshold Halving**:
   - **Target beyond Gun Effective Range**: Final CTH is **cut in half** (`iChance / 2`).
   - **Target beyond Shooter's Vision Range**: Final CTH is **cut in half** (`iChance / 2`).

#### Resolution
- The game rolls `uiDiceRoll = PreRandom(100)`.
- If `uiDiceRoll <= uiHitChance` (`sHitBy > 0`): The bullet is aimed straight at the target coordinate. An obstacle check (`MoveBullet` / `ChanceToGetThrough`) runs: if a wall, window frame, or another soldier blocks the 3D ray, the bullet hits that obstacle instead.
- If `uiDiceRoll > uiHitChance` (`sHitBy <= 0`): `CalculateFiringIncrements` applies an angular deflection scaled to `iMissedBy`. The stray bullet travels along this deflected ray.

---

### 2.2 OpenXcom
*Source references:*
- [`BattleUnit.cpp`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/OpenXcomStraciatella/src/Savegame/BattleUnit.cpp#L2542-L2601) — `BattleUnit::getFiringAccuracy`
- [`BattleUnit.cpp`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/OpenXcomStraciatella/src/Savegame/BattleUnit.cpp#L2609-L2637) — `BattleUnit::getAccuracyModifier`
- [`Projectile.cpp`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/OpenXcomStraciatella/src/Battlescape/Projectile.cpp#L114-L205) — `Projectile::calculateTrajectory`
- [`Projectile.cpp`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/OpenXcomStraciatella/src/Battlescape/Projectile.cpp#L332-L479) — `Projectile::applyAccuracy`

#### Accuracy Rating Formula
OpenXcom calculates an effective accuracy rating for the attack action:
`Final Accuracy = SoldierFiringAccuracy * WeaponModeAccuracy * HealthModifier * KneelBonus * OneHandPenalty`

1. **Fire Modes (Discrete AP / TU Investment)**:
   Instead of fine-tuning aim click by click, you select from discrete weapon firing modes:
   - **Aimed Shot**: Highest accuracy multiplier (typically 80%–110%), highest TU cost (typically 50%–80% of max TUs).
   - **Snap Shot**: Moderate accuracy multiplier (typically 50%–70%), low TU cost (typically 25%–40% of max TUs).
   - **Auto Shot**: Lowest accuracy multiplier per bullet (typically 30%–50%), fires a 3-round burst at moderate TU cost.
2. **Kneeling Bonus**:
   - Kneeling multiplies the weapon's accuracy by the kneel bonus (vanilla default: **+15%**, `result * 115 / 100`).
3. **One-Handed Penalty**:
   - Firing a two-handed weapon while holding any item in the off-hand incurs a **-20% penalty** (`result * 80 / 100`).
4. **Health and Fatal Wounds Modifier**:
   `HealthModifier = max(10, (25 * CurrentHealth / BaseHealth) + 75 - 10 * (Wounds_head + Wounds_arms))`
   Lower health and untreated fatal wounds on the head or shooting arms heavily degrade aim.

#### Trajectory Dispersion & Raycasting
Rather than rolling for an abstract hit, OpenXcom passes the accuracy rating into `Projectile::applyAccuracy`:
1. **Deviation Calculation**:
   `deviation = RNG(0, 100) - (Accuracy * 100)`
   - If `deviation >= 0` ("miss" roll): `deviation += 50` (widens spread).
   - If `deviation < 0` ("hit" roll): `deviation += 10` (tightens spread).
2. **Distance Scaling**:
   `SpreadRadius = max(1, (DistanceVoxels * deviation) / 200)`
3. **Voxel Raycast**:
   - The target coordinates (x, y, z) are displaced by random offsets within `SpreadRadius`.
   - The bullet trajectory is extended along this angle (`calculateLineVoxel`).
   - The first physical 3D voxel obstacle the ray meets (ground, fence, wall, innocent bystander, friendly soldier, or alien voxel bounding box) takes the impact.

---

## 3. Vision, Line of Sight, and Environmental Conditions

### 3.1 Jagged Alliance 2 (Stracciatella)
*Source references:*
- [`OppList.cc`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/ja2-stracciatella-0.22.1/src/game/Tactical/OppList.cc#L874-L1031) — `MaxDistanceVisible` & `DistanceVisible`
- [`LOS.cc`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/ja2-stracciatella-0.22.1/src/game/Tactical/LOS.cc#L2896-L2935) — `SoldierToSoldierBodyPartChanceToGetThrough`

- **Field of View (FOV)**:
  - Central cone of 45° forward view (`STRAIGHT`) provides maximum sight distance.
  - Peripheral angles have reduced sight distance unless the soldier is alerted (Status Red).
  - Blind to the rear.
- **Sight Distances**:
  - **Daylight**: Normal max sight is **26 tiles** (`STRAIGHT * 2`).
  - **Night / Darkness**: Dynamic light levels reduce view distance down to **6–10 tiles**.
- **Equipment & Traits**:
  - **Night Vision Goggles**: +2 tiles of sight in darkness.
  - **UV Goggles**: +4 tiles of sight in darkness (outdoors only).
  - **Night-Ops Trait**: +1 tile per trait level in darkness.
  - **Sun Goggles**: +1 to +2 tiles in bright desert daylight.
  - **Muzzle Flash**: Firing a firearm without a silencer/suppressor reveals the shooter at daytime light levels for that frame.
  - **Smoke & Mustard Gas**: Reduces visibility to a maximum of **2 tiles** unless wearing a gas mask.
  - **Roofs**: Perched soldiers gain +2 tiles of sight distance.

### 3.2 OpenXcom
*Source references:*
- [`TileEngine.cpp`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/OpenXcomStraciatella/src/Battlescape/TileEngine.cpp#L1470-L1530) — `TileEngine::calculateLineOfSight`
- [`TileEngine.cpp`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/OpenXcomStraciatella/src/Battlescape/TileEngine.cpp#L1730-L1770) — Visibility distance helper

- **Field of View (FOV)**:
  - Fixed 90° forward visual quadrant.
  - Units cannot see outside their 90° arc; turning costs 1 TU per 45°.
- **Sight Distances**:
  - **Daylight**: **20 tiles** for both X-Com soldiers and aliens.
  - **Night / Darkness**:
    - **X-Com Soldiers**: Reduced to **9 tiles** in unlit areas.
    - **Aliens**: Retain full **20 tiles** in darkness (inherent night vision advantage).
- **Lighting & Smoke**:
  - **Flares & Fire**: Throwing flares or igniting terrain illuminates tiles, restoring human vision up to the full 20 tiles against illuminated targets.
  - **Smoke Density**: Each tile of smoke accumulates a density penalty along the ray, cutting sight distance drastically.

---

## 4. The Long-Range Squad-Spotting Scenario

### Scenario Setup
```
[Shooter / Sniper] ------------------ Unobstructed LOS -----------------> [Target Enemy]
      ^                                                                         ^
      | (Target is beyond Shooter's personal sight range)                       | (Within 9–20 tiles)
                                                                    [Scout / Spotter]
```
*An enemy is spotted by a forward scout (Team Member A). A rear sniper (Team Member B) is 35–45 tiles away, has an unobstructed geometric line of fire (no physical obstacles), but cannot personally see the enemy due to distance or darkness.*

---

### In OpenXcom: Full Shared "Squad Sight"
*Source references:*
- [`BattlescapeGame.cpp`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/OpenXcomStraciatella/src/Battlescape/BattlescapeGame.cpp#L1729-L1932) — `primaryAction`
- [`TileEngine.cpp`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/OpenXcomStraciatella/src/Battlescape/TileEngine.cpp#L1490-L1505) — Unit visibility setting

1. **Global Visibility**:
   When Member A spots the alien, the game sets `bu->setVisible(true)`. The alien model is rendered on the player's screen.
2. **Direct Targeting**:
   The player selects Member B and clicks directly on the alien.
3. **No Stat or Accuracy Penalty**:
   - In vanilla OpenXcom rules, standard rifles, laser rifles, heavy cannons, and plasma weapons have **no maximum range cap**.
   - The ruleset option `noLOSAccuracyPenalty` is disabled by default (`-1`).
   - Member B fires with their full Aimed or Snap shot accuracy rating.
4. **Natural Geometric Dispersion**:
   The only drawback is physical: because the trajectory spread is an angular cone originating from Member B, the dispersion envelope widens linearly with distance (Spread ∝ Distance). Hitting a 1-tile target at 40 tiles is hard simply because the cone covers a wider area, but there is **no artificial stat penalty**.
5. **Blind Fire Allowed**:
   Member B does not even need Member A to spot the alien. The player can target dark, unexplored fog-of-war tiles and fire blindly into the darkness.

---

### In JA2 Stracciatella: Severe Out-of-Sight Penalties
*Source references:*
- [`Weapons.cc`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/ja2-stracciatella-0.22.1/src/game/Tactical/Weapons.cc#L2088-L2124) — Spotting & out-of-sight range adjustments
- [`Weapons.cc`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/ja2-stracciatella-0.22.1/src/game/Tactical/Weapons.cc#L2471-L2486) — Out-of-sight and max-range halving
- [`Weapons.cc`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/ja2-stracciatella-0.22.1/src/game/Tactical/Weapons.cc#L2210-L2225) — Sniper scope compensation

1. **Team Opplist Integration**:
   - When Member A spots an enemy, the enemy is flagged in `gbPublicOpplist[OUR_TEAM]` and rendered on the tactical map.
   - In [`Weapons.cc` line 2103](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/ja2-stracciatella-0.22.1/src/game/Tactical/Weapons.cc#L2103), the engine explicitly checks `gbPublicOpplist` to permit spotting for snipers, evaluating line of sight up to 2 * MaxDistanceVisible (up to **52 tiles**).
2. **Virtual Range Doubling Penalty**:
   If the target is farther than Member B's personal vision distance (`sDistVis`), the excess distance is doubled for distance-dropoff calculations:
   ```cpp
   if (iSightRange > (sDistVis * CELL_X_SIZE))
   {
       iSightRange += (iSightRange - sDistVis * CELL_X_SIZE);
   }
   ```
   This heavily inflates range penalties and severely punishes headshots.
3. **Calculated Hit Chance Cut in Half**:
   At [line 2481](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/ja2-stracciatella-0.22.1/src/game/Tactical/Weapons.cc#L2481):
   ```cpp
   if (iSightRange > (sDistVis * CELL_X_SIZE))
   {
       iChance /= 2; // Penalize out of sight shots
   }
   ```
   The final Chance-To-Hit is **halved (/ 2)** solely because the shooter cannot personally see the target.
4. **Cumulative Gun Range Halving**:
   If the distance also exceeds the gun's effective range statistic, `iChance /= 2` triggers **again**, reducing hit chance to a quarter (/ 4) of base value.
5. **The Necessary Countermeasure (Sniper Scope & Aiming)**:
   - Sniping an out-of-sight target spotted by a teammate in JA2 is only viable with a **Sniper Scope** and full aiming investment.
   - At [line 2217](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/ja2-stracciatella-0.22.1/src/game/Tactical/Weapons.cc#L2217), spending 4 aiming APs with a scope shrinks the effective sight distance by up to 80%, offsetting the out-of-sight distance penalties.

---

## 5. Comparative Summary Matrix

| Metric / Action | OpenXcom | Jagged Alliance 2 (Stracciatella) |
| :--- | :--- | :--- |
| **Can you shoot an enemy spotted only by a teammate?** | **Yes**, completely unhindered. | **Yes**, supported via `gbPublicOpplist`. |
| **Penalty for lack of personal LOS?** | **None** (vanilla rules). | **Massive**: Virtual distance doubled, and final CTH is **halved (/ 2)**. |
| **Weapon range limits?** | Effectively infinite for rifles/heavy weapons. | Strict range stats; exceeding max range **halves CTH again (/ 2)**. |
| **Aiming refinement?** | Discrete fire mode choice (Aimed vs. Snap vs. Auto). | Granular AP investment (up to 4 aiming rings per shot). |
| **Stance bonuses?** | Kneeling grants a +15% accuracy multiplier. | Crouch grants +8%; Prone grants +16% (+24% with Bipod). |
| **Night combat balance?** | Asymmetrical: aliens see 20 tiles in the dark, humans see 9 tiles unless using flares. | Symmetrical: ambient light affects everyone; mitigated by Night/UV goggles and traits. |
| **Shooting through fog of war?** | Permitted anywhere (ground, walls, dark tiles). | Allowed, but penalized by `AIM_PENALTY_BLIND`. |


---

## 6. Architectural Evaluation: Which Engine is the Better Solution?

Neither solution is universally superior in a vacuum; rather, each represents an architectural **masterclass tailored to fundamentally different gameplay priorities and design goals**.

### 6.1 Architectural Trade-Off Matrix

| Evaluation Dimension | OpenXcom (3D Voxel Ballistics) | JA2 Stracciatella (Hybrid RPG + Tracing) | Superior Model |
| :--- | :--- | :--- | :--- |
| **Emergent Physics & Ballistics** | **Flawless**: Projectiles are true 3D voxel rays; every obstacle, bystander, and terrain structure interacts naturally. | **Artificial**: High CTH forces straight-line hits; misses deviate mathematically via formula. | **OpenXcom** |
| **Destructive Environments** | **Superior**: Stray projectiles naturally punch holes through walls, floors, and roofs, opening dynamic lines of fire. | **Limited**: Scenery damage exists, but is not organically coupled to every routine bullet trajectory. | **OpenXcom** |
| **Tactical Granularity & Depth** | **Coarse**: 3 discrete fire modes (Snap/Aimed/Auto), no body-part targeting, no weapon attachment synergies. | **Extremely Deep**: Granular AP aiming clicks, head/torso/leg calls, scopes, bipods, laser sights, fatigue, shock. | **JA2** |
| **Predictability & Player Agency** | **Can feel arbitrary**: Low-voxel obstacles (windowsills, fences, dirt mounds) can block shots unexpectedly. | **Transparent & Fair**: Displays explicit Chance-To-Hit (CTH) % and Chance-To-Get-Through (CTGT) % before firing. | **JA2** |
| **Squad Sight / Sniping Realism** | **Simplistic / Arcady**: Infinite effective range; snipers can shoot across entire maps with zero LOS stat penalty. | **Authentic & Nuanced**: Shooting beyond personal sight range halves CTH; requires specialized sniper builds to be viable. | **JA2** |
| **Implementation Simplicity** | Clean mathematical pipeline: Accuracy -> Cone Deviation -> Raycast. | High ruleset complexity: hundreds of conditional modifiers, special cases, and stat interactions. | **OpenXcom** |

---

### 6.2 Where OpenXcom's Pure 3D Voxel Engine Excels

1. **Organic Emergence and Collateral Damage**:
   In OpenXcom, a bullet is an autonomous entity moving step-by-step through voxel space. If an auto-fire burst misses its primary alien target:
   - Stray bullets can blow through a church wall, collapse a roof, trigger an explosive power source, or accidentally kill an alien concealed behind the target.
   - Target size is naturally resolved by voxel geometry: a Sectopod or Cyberdisc occupies a large bounding box, meaning even low-accuracy rookie spray fire will reliably hit it without requiring arbitrary "giant target" stat modifiers.
2. **Elegant, Maintainable Code Architecture**:
   OpenXcom's trajectory pipeline ([`Projectile::applyAccuracy`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/OpenXcomStraciatella/src/Battlescape/Projectile.cpp#L332-L479)) is remarkably compact and unified. Melee, firearms, heavy launchers, and thrown items share the same spatial raycasting logic, avoiding the hundreds of bespoke branching checks found in traditional RPGs.

---

### 6.3 Where JA2 Stracciatella's Hybrid Engine Excels

1. **Tactical Agency and Deep Mercenary Differentiation**:
   OpenXcom cannot match JA2's granular control over every single attack:
   - **Continuous Aiming APs**: Deciding whether to take a quick snap shot or invest 1–4 extra APs into aiming creates crucial risk/reward dynamics every turn.
   - **Called Shots (Body Parts)**: The choice between an easy body shot or a high-risk, high-reward headshot (-3% CTH per tile of distance penalty vs. lethal critical damage bypassing torso armor).
   - **Modular Weapon Attachments**: Attachments in JA2 (Bipods, Sniper Scopes, Laser Sights, Duckbills, Barrel Extenders) directly modify specific mathematical terms in the CTH equation rather than functioning as generic flat-percentage buffs.
2. **Elimination of "Micro-Voxel Frustrations"**:
   Pure physical raycasting engines suffer from micro-geometry quirks: a soldier kneeling behind a window or low barricade might hit the windowsill because the gun's voxel origin is 2 units too low. JA2's hybrid engine pre-calculates both Chance-To-Hit (CTH) and Chance-To-Get-Through (CTGT) ([`LOS.cc`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/ja2-stracciatella-0.22.1/src/game/Tactical/LOS.cc#L2896-L2935)), clearly showing the player whether cover will obstruct the bullet *before* they waste APs.
3. **Militarily Authentic Long-Range Engagement**:
   In OpenXcom, a rookie with a basic rifle can shoot an alien spotted by a tank 40 tiles away with zero accuracy penalty. JA2 explicitly avoids this unrealistic "map-sniping": shooting beyond personal vision range cuts final CTH in half (/ 2) and doubles distance penalties. Spotting for a sniper is only effective if the player equips a dedicated sniper rifle, mounts a bipod, goes prone, attaches a Sniper Scope, and spends full APs aiming.

---

### 6.4 The Verdict: Selecting the Best Architecture

#### Choose **OpenXcom's 3D Voxel Simulation** when:
- **Environmental destruction is a core pillar** of the game design (collapsible buildings, explosive breaches, dynamic cover removal).
- You want **emergent, chaotic firefights** where stray rounds, friendly fire, and collateral damage drive player drama.
- Your roster consists of **expendable military squads** where fast-paced, high-casualty tactical turns take precedence over micromanaging weapon attachments and body parts.

#### Choose **JA2 Stracciatella's Hybrid Engine** when:
- You are developing an **RPG-tactical combat system focused on distinct characters** whose individual stats (dexterity, wisdom, marksmanship, fatigue, shock, morale) must directly influence combat efficacy.
- You want **deep tactical agency and modular equipment tuning** (called headshots, aiming AP investments, scopes, bipods, suppressors, stance bonuses).
- You prioritize **clear, deterministic feedback** (transparent CTH / CTGT percentages) so tactical success feels driven by careful planning and preparation rather than physical collision quirks.
