# Tactical Combat Mechanics: Investigation & Evolution Topics
### Deep Comparative Analysis & Future Modding Horizons: Jagged Alliance 2 Stracciatella vs. OpenXcom

> [← Back to Documentation Overview](ja2_stracciatella_mods_overview.md)

---

## 1. Executive Summary & Purpose

Having successfully analyzed and bridged two major foundational pillars—**Ballistics/Aiming/Line-of-Sight** (implemented in [*Jagged Alliance 2 Straciatella Tactical*](ja2_stracciatella_tactical_guide.md)) and **Acoustic Awareness & Alert Hierarchy** (implemented in [*Jagged Alliance 2 Straciatella Awareness*](ja2_stracciatella_awareness_guide.md))—this document outlines the remaining major **tactical combat mechanics** present in both engines.

The goal is to provide a structured roadmap for future deep-dive code investigations, game-design evaluations, and potential mod implementations.

---

## 2. Topic 1: Suppression & Pinning Under Fire
### *Morale & Action Point Denial Under Near-Misses*

### The Core Problem
In vanilla **OpenXcom**, a soldier can be targeted by an alien Auto-Cannon firing 30 rounds that chew through the wall 2 inches above their head. If none of those bullets physically intersect the soldier's voxel hitbox, the soldier suffers **zero penalty**: they enter their next turn with 100% Time Units, perfect morale, and pinpoint firing accuracy.

In **Jagged Alliance 2**, bullet volume itself is a weapon. Firing an LMG or burst into an enemy's cover suppresses them:
- **`Shock` Accumulation**: Every bullet passing within a defined radius adds shock points.
- **Action Point Denial**: High shock drains Action Points on the victim's upcoming turn, simulating staying pinned behind cover.
- **Cowering & Interrupt Loss**: Suppressed units drop their weapon ready stance, lose the ability to perform reaction interrupts, and can be forced into a defensive cower.

### Source Code Exploration Targets
- **JA2 Stracciatella**: [`Weapons.cc`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/ja2-stracciatella-0.22.1/src/game/Tactical/Weapons.cc) (`HandleBulletSuppression`, `ProcessSuppression`, `ShockPoints`).
- **OpenXcom**: [`Projectile.cpp`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/OpenXcomStraciatella/src/Battlescape/Projectile.cpp) (trajectory collision detection) and [`BattleUnit.cpp`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/OpenXcomStraciatella/src/Savegame/BattleUnit.cpp) (`moraleCheck`, `spendTimeUnits`).

> [!NOTE]
> **Status: Implemented as Mod 3 (`Jagged Alliance 2 Straciatella Suppression`)**
> - 3D voxel near-miss tracking, TU drain, morale shock, and forced kneeling.
> - Full HitLog suppression warnings and reaction overwatch disruption.
> - See documentation: [ja2_stracciatella_suppression_guide.md](ja2_stracciatella_suppression_guide.md).

---

## 3. Topic 2: Interactive Interrupts vs. Automated Reaction Fire
### *Tactical Counter-Play vs. Behind-the-Scenes Math Check*

### The Core Problem
- **OpenXcom**: Uses an automated reaction fire check:
  $$\text{Reaction Score} = \text{Unit Reaction} \times \frac{\text{Current TU}}{\text{Max TU}}$$
  If the watcher beats the moving target, the watcher automatically fires a Snap Shot. The player has no say in this action: you cannot choose to duck, retreat, throw a flare, switch weapons, or hold fire for a better target.
- **Jagged Alliance 2**: Features **Interactive Interrupt Duels**:
  - Evaluated through `ResolveInterruptsVs` and `CalculateInterruptDuelPoints`.
  - Factors in: Agility, Experience Level, sensory awareness (visual spotting vs. sound hearing), current APs, and facing angle.
  - **Player Agency**: When an interrupt triggers, the game **pauses the enemy turn and hands control to you**. You spend your accumulated APs to make decisions (shoot, step back into cover, change stance, or reload), then yield the turn back.

### Source Code Exploration Targets
- **JA2 Stracciatella**: [`OppList.cc`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/ja2-stracciatella-0.22.1/src/game/Tactical/OppList.cc) (`ResolveInterruptsVs`, `CalculateInterruptDuelPoints`, `StandardInterruptDuelPoints`).
- **OpenXcom**: [`TileEngine.cpp`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/OpenXcomStraciatella/src/Battlescape/TileEngine.cpp) (`checkReactionFire`) and [`BattlescapeGame.cpp`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/OpenXcomStraciatella/src/Battlescape/BattlescapeGame.cpp).

> [!NOTE]
> **Status: Implemented as Mod 4 (`Jagged Alliance 2 Straciatella Reaction`)**
> - Full interactive interrupt prompt with hotkeys `[1]`-`[5]`.
> - Selection between Snap, Auto Burst, Aimed Shot, Throw Grenade, or Hold Fire.
> - Physical inventory checks (Hand vs. Quick Belt slot, backpack locked out).
> - Immediate impact detonation on landing ("Cooked Grenade" / Option A).
> - Immediate smoke cloud cover on landing.
> - Immediate proximity mine arming on tile.
> - Alien AI distance-based reaction selection (Auto bursts in CQC, Aimed snipes at long range).
> - See documentation: [ja2_stracciatella_reaction_guide.md](ja2_stracciatella_reaction_guide.md).

---

## 4. Topic 3: Stance Dynamics & The Missing "Prone / Crawling" Stance
### *Hitbox Profiles, Recoil Stability, and Low Infiltration*

### The Core Problem
- **OpenXcom**: Supports only two stances: **Standing** and **Kneeling**.
  - Kneeling reduces vertical voxel height from 22 voxels to $\sim 14$ voxels and grants accuracy bonuses.
  - However, there is no Prone stance. Soldiers cannot crawl under fences, through ventilation, or flatten themselves in high grass.
- **Jagged Alliance 2**: Built around three distinct stances:
  1. **Standing**: Fastest mobility, full view range, largest target profile.
  2. **Crouching**: Moderate mobility, covers waist-high barriers.
  3. **Prone**: Drastic reduction in target silhouette, massive recoil reduction (especially when combined with a Bipod), ability to crawl silently under fire, but high AP cost to stand back up.

### Source Code Exploration Targets
- **JA2 Stracciatella**: [`Overhead.cc`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/ja2-stracciatella-0.22.1/src/game/Tactical/Overhead.cc) and `gAnimControl` (`ANIM_STAND`, `ANIM_CROUCH`, `ANIM_PRONE`).
- **OpenXcom**: [`BattleUnit.h`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/OpenXcomStraciatella/src/Savegame/BattleUnit.h) (`getHeight`, `_kneeled`), [`TileEngine.cpp`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/OpenXcomStraciatella/src/Battlescape/TileEngine.cpp) (voxel bounding boxes).

---

## 5. Topic 4: Terminal Ballistics, Ammo Specialization & Armor Degradation
### *Hollow Point vs. Armor Piercing & Structural Armor Wear*

### The Core Problem
- **OpenXcom**: Uses a flat damage reduction model:
  $$\text{Damage Received} = \text{Weapon Damage} \times \text{DamageVariance}(0\%\text{--}200\%) - \text{ArmorValue}$$
  Directional armor plates (Front, Sides, Rear, Under) never degrade when hit by standard fire (unless destroyed by corrosive/shredding script rules in OXCE).
- **Jagged Alliance 2**: Features authentic terminal ballistics:
  - **Ammunition Physics**:
    - **Hollow Point (HP)**: Devastating against flesh (1.5x to 2x tissue trauma) but completely stops against soft Kevlar or steel plates.
    - **Armor Piercing (AP)**: Hardened tungsten/steel core slices through ceramic inserts and body armor, but over-penetrates soft tissue, dealing lower flesh damage.
  - **Armor Wear**: Ceramic plates crack and shatter under heavy fire; Kevlar weaves fray, permanently losing protective rating under repeated hits.

### Source Code Exploration Targets
- **JA2 Stracciatella**: [`Weapons.cc`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/ja2-stracciatella-0.22.1/src/game/Tactical/Weapons.cc) (`BulletHit`, `Damage`, `HandleArmorDegradation`).
- **OpenXcom**: [`BattleUnit.cpp`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/OpenXcomStraciatella/src/Savegame/BattleUnit.cpp) (`damage`, `getArmor`) and [`RuleDamageType.cpp`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/OpenXcomStraciatella/src/Mod/RuleDamageType.cpp).

---

## 6. Topic 5: Combat Trauma, Real-Time Bleeding & Field First Aid
### *Immediate Wound Care vs. Strategic Medical Recovery*

### The Core Problem
- **OpenXcom**: Implements discrete "Fatal Wounds" across 6 body parts (Head, Torso, Left/Right Arm, Left/Right Leg).
  - Each fatal wound deals 1 HP of bleeding per turn.
  - Using a MediKit instantly stops the wound and restores stability.
- **Jagged Alliance 2**: Implements a continuous trauma and bleeding rate:
  - Every gunshot opens bleeding relative to bullet caliber and hit location.
  - **First Aid (Bandaging)**: A combat medic applies gauze/bandages to convert active bleeding (red HP) into stabilized bandaged wounds (pink HP).
  - **Real-Time Pain & Shock**: Severely wounded mercs lose APs each turn due to agonizing pain (unless administered a morphine shot).
  - Healing bandaged pink HP back into functional combat HP cannot be done instantly in the middle of battle—it requires a doctoring assignment or surgical recovery over days.

### Source Code Exploration Targets
- **JA2 Stracciatella**: [`Soldier_Control.cc`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/ja2-stracciatella-0.22.1/src/game/Tactical/Soldier_Control.cc) (`HandleBleeding`, `PainMultiplier`) and [`Interface_Panels.cc`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/ja2-stracciatella-0.22.1/src/game/Tactical/Interface_Panels.cc).
- **OpenXcom**: [`BattleUnit.cpp`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/OpenXcomStraciatella/src/Savegame/BattleUnit.cpp) (`healFatalWound`, `applyStimulant`, `painkiller`).

---

## 7. Topic 6: Explosives, Breaching & Physical Shrapnel vs. Voxel Blast Waves
### *Fragmentation Rays vs. 3D Destructible Pressure Waves*

### The Core Problem
- **OpenXcom's Superpower**: Features one of the greatest fluid blast pressure models ever created in a turn-based engine:
  - Concussive energy propagates outward in 3D voxel space, breaking through soft wood, fracturing concrete, and being stopped by alien alloys.
  - Leaves behind smoke and fire plumes.
- **JA2's Superpower**: Simulates **Physical Fragmentation Shrapnel**:
  - In addition to concussive blast radius, an explosion fires dozens of invisible high-velocity shrapnel rays.
  - A soldier 20 tiles away from a grenade can still catch a stray steel fragment and suffer bleeding wounds.
  - Breaching charges (C4, TNT) specifically target doors, hinges, and structure walls.

### Source Code Exploration Targets
- **JA2 Stracciatella**: [`Explosion_Control.cc`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/ja2-stracciatella-0.22.1/src/game/TileEngine/Explosion_Control.cc) (`GenerateExplosion`, `SpreadExplosionShrapnel`).
- **OpenXcom**: [`TileEngine.cpp`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/OpenXcomStraciatella/src/Battlescape/TileEngine.cpp) (`explode`, `hitTile`).

---

## 8. Topic 7: Field Weapon Customization & Modular Attachments
### *Modular Firearms vs. Monolithic Weapons*

### The Core Problem
- **OpenXcom**: Every weapon is a rigid, monolithic inventory item. A Heavy Cannon is always a Heavy Cannon; you cannot screw on a suppressor, mount a laser pointer, or clip on a high-magnification scope.
- **Jagged Alliance 2**: Pioneer of the modular firearm attachment system:
  - **Scopes**: Boost CTH at long ranges.
  - **Laser Sights**: Boost CTH at short/medium ranges by projecting a red dot.
  - **Bipods**: Massive stability multiplier when firing prone.
  - **Silencers**: Cut acoustic footprint, enabling true covert operations.
  - **Duckbills**: Reshape shotgun pellet spreads from circular cones into wide horizontal sweeps.

### Source Code Exploration Targets
- **JA2 Stracciatella**: [`Items.cc`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/ja2-stracciatella-0.22.1/src/game/Tactical/Items.cc) (`AttachObject`, `CanObjectBeAttached`).
- **OpenXcom**: [`RuleItem.h`](file:///home/jcx/wrk-ai/JA2-OpenXcom%20-%20sources/OpenXcomStraciatella/src/Mod/RuleItem.h) and OXCE's custom weapon script container.

---

## 9. Investigation & Implementation Feasibility Matrix

| Topic | Complexity | Implementation Route | Gameplay Impact |
| :--- | :---: | :--- | :--- |
| **1. Suppression & Pinning** | **Medium** | Engine Hook (`Projectile.cpp` near-miss) + YAML Rule | ⭐⭐⭐⭐⭐ Eliminates fearless charges; makes machine guns viable for area denial. |
| **2. Interrupts vs. Reaction** | **High** | Core Turn Engine Redesign (`BattlescapeGame.cpp`) | ⭐⭐⭐⭐⭐ Transforms defensive turns into tense, interactive tactical duels. |
| **3. Prone / Crawling Stance** | **Very High** | Voxel Engine & Sprite Animation overhaul | ⭐⭐⭐⭐ Massive stealth/defense boost; heavy asset requirement. |
| **4. Ammo Types & Armor Wear** | **Low–Medium** | YAML Ruleset + Armor Health tracking | ⭐⭐⭐⭐ Adds tactical depth to loadout choices against armored aliens. |
| **5. Trauma & Bleeding** | **Low–Medium** | Engine `BattleUnit` pain calculation + YAML Rule | ⭐⭐⭐ Increases tension of medical emergencies in the field. |
| **6. Explosive Shrapnel** | **Medium** | Engine raycasting in `TileEngine::explode` | ⭐⭐⭐⭐ Makes grenades lethal even outside the immediate blast crater. |
| **7. Modular Attachments** | **Medium–High** | Inventory GUI + Item combination logic | ⭐⭐⭐⭐ Deep personalization and tactical weapon tailoring. |
