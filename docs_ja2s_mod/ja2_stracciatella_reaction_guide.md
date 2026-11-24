# Jagged Alliance 2 Straciatella Reaction for OpenXcom (OXCE)
### Interactive Interrupts, Tactical Weapon Mode Selection, Cooked Grenades, and Smart AI Reactions

> [← Back to Documentation Overview](ja2_stracciatella_mods_overview.md)

---

## 1. Overview & Vision

In vanilla OpenXcom (1994), **Reaction Fire** is entirely automated and rigid:
* When an alien triggers reaction fire during their turn, the engine forces the reacting soldier to take a single **Snap Shot** (or melee attack if in contact).
* The player has **zero control**: you cannot choose to fire an Auto burst, take a deliberate Aimed Shot, toss a grenade at an advancing group, or **Hold Fire** to save Time Units for a bigger threat.
* Alien reaction fire is equally primitive, firing single snap shots regardless of whether the target is 2 meters away or 40 meters away.

**Jagged Alliance 2 Straciatella Reaction** transforms reaction fire into an **Interactive Interrupt System** inspired by *Jagged Alliance 2 Stracciatella*:
1. **Interactive Reaction Prompt**: When an X-Com soldier wins a reaction check against an active enemy, execution pauses and an interactive modal menu opens.
2. **Flexible Attack Modes**: Choose between **Snap Shot**, **Auto Burst**, or **Aimed Shot** based on available Time Units and tactical necessity.
3. **Prepared Grenade Reaction (Option A)**: Lob a grenade directly at the enemy. Reaction-thrown grenades **detonate immediately on impact** ("cooked grenade"), burst into instant smoke clouds, or arm proximity mines.
4. **Physical Inventory Authenticity**: Grenade throws are permitted from **Hand** or **Quick Belt** (with 1 free hand). Digging into heavy backpacks during a split-second enemy step is strictly locked out.
5. **Hold Fire / Pass**: Conserve your soldier's TUs and concealment without giving away your ambush position.
6. **Smart Alien AI Reactions**: Enemies unleash lethal Auto bursts at close quarters (<= 6 tiles) and take precise Aimed sniper shots at extreme range (>= 15 tiles).

---

## 2. How Interrupts Trigger: The 4 Conditions

An interrupt does **not** occur automatically just because an enemy comes into view. In OpenXcom Extended with the JA2 Reaction module, all **four** of the following conditions must be met:

### 1. Vision & Line of Sight (LOS & LOF)
* **View Cone**: The moving enemy must enter your soldier's visual arc (standard 90° forward cone, or the ~180° peripheral vision arc within 50% max distance).
* **Lighting & Distance**: The enemy must be within visible range (up to 20 tiles in daylight, or within night-vision distance in darkness/smoke).
* **Line of Fire (LOF)**: There must be an unobstructed path to target the enemy (not completely blocked by solid structural walls).

### 2. The Reaction Score Contest (The Initiative Roll)
Every time an alien takes a micro-step, turns, or performs an action in sight of your soldier, the engine calculates a **Reaction Score**:

`Reaction Score = Reactions Stat * (Current Remaining TUs / Max TUs)`

* To trigger an interrupt, **your soldier’s Reaction Score must be greater than or equal to the moving enemy's Reaction Score**.
* **Why this matters**:
  * An alien with high reactions who just started moving has nearly 100% of its TUs, making its score very high.
  * As the alien walks, turns, or opens doors, its remaining TUs drop with every action.
  * Once the alien's score drops below your soldier's score, your soldier **interrupts** them mid-stride.
  * *Tactical Rule*: Saving more TUs at the end of your turn directly raises your soldier's Reaction Score!

### 3. Weapon Range Constraints
The enemy **must be within your weapon's effective firing range**:
* **Rifles & Heavy Weapons**: Have long maximum ranges and can react across the entire visible battlefield.
* **Pistols & Shotguns**: Have hard maximum effective ranges (e.g., 15–18 tiles). If an alien is spotted 20 tiles away, a pistol cannot trigger an interrupt.
* **Grenades**: The enemy must be within the soldier's physical throw range (governed by soldier Strength and grenade weight).

### 4. Time Unit (TU) Affordability & Menu Filtering
* Your soldier must have enough remaining TUs to afford at least one viable attack action (Snap Shot, Auto Burst, Aimed Shot, or Throw Grenade).
* **Smart Filtering**: The menu will **only display options your soldier can afford** with their current TUs. If your soldier has 15 TUs and an Auto Burst requires 24 TUs, the Auto Burst option will not be shown.
* **No Useless Popups**: If your soldier does not have enough TUs for *any* attack action, or if the enemy is completely out of range, the interrupt prompt will not appear at all, allowing the enemy turn to proceed smoothly.

---

## 3. Interactive Reaction Prompt & Controls

When a reaction opportunity occurs, the camera centers on the reacting soldier and the **REACTION INTERRUPT!** window appears:

```
+-------------------------------------------------------+
|                 REACTION INTERRUPT!                   |
|              Sgt. Michael Vance (42 TU)               |
|            Target: Sectoid (Distance: 7 tiles)        |
+-------------------------------------------------------+
| [1] SNAP SHOT (18 TU, 68%)                            |
| [2] AUTO BURST (28 TU, 45%)                           |
| [3] AIMED SHOT (38 TU, 85%)                           |
| [4] THROW GRENADE (26 TU)                             |
| [5] HOLD FIRE (0 TU)                                  |
+-------------------------------------------------------+
```

### Hotkeys:
* `[1]`: Execute **Snap Shot**
* `[2]`: Execute **Auto Burst** (if available)
* `[3]`: Execute **Aimed Shot** (if available)
* `[4]`: Execute **Throw Grenade** (if available)
* `[5]`, `[Space]`, `[Esc]`, or **Right-Click**: **Hold Fire / Pass**

---

## 4. The 3 Grenade Situations & Physical Rules

During an interactive interrupt, accessing inventory items is governed by strict physical and Time Unit constraints:

| Situation | Status | Mechanics & TU Cost |
| :--- | :---: | :--- |
| **Situation 1A: Primed in Hand** | **Allowed** | Incurs only `costThrow` (~25% Max TUs, ~14–18 TUs). Instant release into alien path. |
| **Situation 1B: Unprimed in Hand** | **Allowed** | Incurs `costPrime` (15 TUs) + `costThrow`. Soldier arms the grenade and lobs it (~30–35 TUs). |
| **Situation 2A: Grenade on Belt (1 Hand Free)** | **Allowed** | Incurs Quick-Draw (8 TUs) + Prime (15 TUs) + Throw (~16 TUs) = ~38 TUs. Transferred to free hand. |
| **Situation 2B: Grenade in Backpack (1 Hand Free)** | **Forbidden** | Rummaging through a backpack costs 24+ TUs, pushing total cost over 58+ TUs. Unfeasible during a 4-TU enemy step. |
| **Situation 3: Both Hands Full** | **Forbidden** | Cannot manipulate or draw a grenade without dropping equipped weapons. |

---

## 5. Option A: Immediate Impact Detonation ("Cooked Grenade")

In vanilla OpenXcom, thrown grenades sit inert on the ground until the turn ends, rendering grenade reaction throws useless against active enemies.

Under **`Jagged Alliance 2 Straciatella Reaction`**:
* **High Explosive & Frag Grenades (Option A)**:
  * Handled as "cooked" / impact-fused.
  * The instant the grenade touches down on the destination voxel, it triggers an immediate `ExplosionBState`.
  * Deals lethal blast damage, destroys cover, and instantly eliminates or suppresses the advancing enemy.
* **Smoke Grenades**:
  * Immediately detonate on landing, producing an instant dense smoke cloud that breaks line of sight to protect your operative.
* **Proximity Grenades**:
  * Arms on the ground tile immediately upon landing. The alien's next micro-step immediately triggers the mine point-blank.

---

## 6. Hold Fire / Tactical Pass

In vanilla X-Com, soldiers with reaction capability fire compulsively at the first enemy that enters their view, often:
* Giving away a hidden sniper's position.
* Wasting valuable TUs or the last round in a magazine on a harmless scout rather than the heavily armed terror unit behind it.

Choosing **`[5] HOLD FIRE`**:
* Deducts **0 Time Units** and spends **0 ammunition**.
* Smoothly passes priority back to the active turn without interrupting player control on upcoming turns.
* Leaves the soldier prepared to react against higher-priority targets later in the turn.

---

## 7. How It Affects Gameplay (Player Tactics)

1. **Decisive Ambush Lethality**: Set up deadly doorway traps with an operative carrying an Auto-Cannon or Heavy Plasma ready to unload an Auto burst the moment an alien steps through.
2. **Emergency Smoke Evasion**: If a dangerous terror unit (like a Chryssalid) rushes your squad, a soldier with a belt smoke grenade can react by popping an instant smoke cloud to break LOS.
3. **Fire Control**: Holding fire preserves sniper concealment, allowing you to let scouts pass unharmed and take down the alien commander following behind them.

---

## 8. How It Affects Enemies (Alien AI Behavior)

1. **Deadly Alien Close-Quarters Bursts**:
   - In vanilla, aliens at point-blank range fire single snap shots. Under this mod, aliens within 6 tiles unleash lethal **Auto Bursts**. Rushing a cornered alien without suppression is extremely dangerous.
2. **Extreme-Range Alien Sniping**:
   - Distant aliens outside 15 tiles take calculated **Aimed Shots** on reaction, turning long open sightlines into true danger zones.

---

## 9. Terror from the Deep (TFTD) Companion Mod (`JA2_Stracciatella_Reaction_TFTD`)

The companion mod **`JA2_Stracciatella_Reaction_TFTD`** enables interactive reaction interrupts across all *Terror from the Deep* operations (`master: xcom2`):
* **Cooked Aquatic Explosives**:
  - Reaction-thrown **Magna-Blast Grenades** and **Sonic Pulsars** detonate on impact when landing, dealing immediate blast damage to rushing Deep Ones, Calcinites, or Tentaculats.
  - **Particle Displacement Grenades**: Instant cloud dispersion to break line-of-sight underwater.
  - **Proximity Mines**: Arms immediately upon touching the seabed.
* **AI Reaction Ranges**:
  - Aquatic aliens fire deadly Auto Bursts at <= 6 tiles and take calculated Aimed Shots at >= 15 tiles.

---

## 10. Summary Configuration Matrix

| Hook Name | Default (Vanilla) | Mod Value | Effect |
| :--- | :---: | :---: | :--- |
| `interactiveReaction` | `false` | `true` | Enables interactive modal prompt for player reactions. |
| `reactionAIModeSelection` | `false` | `true` | Enables intelligent mode selection for AI reactions. |
| `reactionAutoRange` | `0` | `6` | Distance threshold for AI auto burst reactions (<= 6 tiles). |
| `reactionAimedRange` | `0` | `15` | Distance threshold for AI aimed shot reactions (>= 15 tiles). |
| `reactionImpactDetonation` | `false` | `true` | Reaction thrown grenades detonate immediately upon landing. |
| `reactionAllowBeltGrenade` | `false` | `true` | Allows quick-draw belt grenades if one hand is free. |
| `reactionAllowPass` | `false` | `true` | Enables Hold Fire / Pass option. |

---

## 11. How to Enable / Disable

1. Start **OpenXcom** (`make ufo`, `make tftd`, or `make openxcom`).
2. Go to **Options** -> **Mods**.
3. Under the active master mod:
   - For **UFO / xcom1**: Check `[X] Jagged Alliance 2 Straciatella Reaction`
   - For **TFTD / xcom2**: Check `[X] Jagged Alliance 2 Straciatella Reaction TFTD`
4. Click **OK**.
