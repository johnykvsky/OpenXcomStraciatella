# Jagged Alliance 2 Straciatella Reaction for OpenXcom (OXCE)

This mod brings **interactive interrupts, tactical weapon mode selection, cooked grenades, and smart AI reaction fire** from Jagged Alliance 2 into OpenXcom.

---

## 1. How Interrupts Trigger: The 4 Conditions

An interrupt does **not** occur automatically just because an enemy comes into view. All **four** of the following conditions must be met:

1. **Vision & Line of Sight (LOS & LOF)**:
   - The moving enemy must enter your soldier's visual arc (standard 90° forward cone, or ~180° peripheral vision within 50% max distance).
   - The enemy must be within visible range (up to 20 tiles in daylight, or within night-vision distance in darkness/smoke).
   - Clear line of fire: not blocked by solid walls.

2. **The Reaction Score Contest (The Initiative Roll)**:
   - Every time an alien takes a micro-step, turns, or performs an action in sight of your soldier, the engine calculates:
     $$\text{Reaction Score} = \text{Reactions Stat} \times \frac{\text{Current Remaining TUs}}{\text{Max TUs}}$$
   - Your soldier's Reaction Score must be **greater than or equal to** the moving enemy's score.
   - As aliens spend TUs walking and opening doors, their score drops until your soldier interrupts them mid-stride. Saving more TUs on your turn increases your interrupt score!

3. **Weapon Range Constraints**:
   - The enemy must be within your weapon's effective range (Rifles/Cannons cover the whole visible map; Pistols/Shotguns have max ranges of 15–18 tiles; Grenades require the enemy to be within throw range).

4. **Time Unit (TU) Affordability & Smart Filtering**:
   - The soldier must have enough TUs to afford at least one attack action.
   - **Unaffordable actions are hidden**: If Auto Burst costs 24 TUs and your soldier has 18 TUs, Auto Burst will not appear in the menu.
   - If no attack action can be afforded or the enemy is out of range, the prompt does not appear, keeping the game smooth.

---

## 2. Features Summary

1. **Interactive Reaction Modal Prompt (`interactiveReaction: true`)**:
   - Pauses execution when reaction fire triggers, letting the player choose Snap Shot, Auto Burst, Aimed Shot, Throw Grenade, or Hold Fire.
2. **Physical Inventory Authenticity (`reactionAllowBeltGrenade: true`)**:
   - Grenades may be reaction-thrown from hands or quick-draw belt (requires 1 free hand). Heavy backpacks are strictly locked out during split-second interrupts.
3. **Cooked Grenades (`reactionImpactDetonation: true`)**:
   - Reaction-thrown high-explosive grenades detonate immediately upon impact ("cooked grenade"); smoke grenades deploy instant smoke cover; proximity mines arm immediately.
4. **Hold Fire / Pass (`reactionAllowPass: true`)**:
   - Conserves Time Units and preserves sniper concealment without breaking ambush positioning.
5. **Smart Alien AI Reactions (`reactionAIModeSelection: true`)**:
   - Enemies unleash point-blank Auto Bursts at close range ($\le 6$ tiles, `reactionAutoRange: 6`) and take precise Aimed Shots at sniper range ($\ge 15$ tiles, `reactionAimedRange: 15`).

---

## 3. In-Game GUI Options: Options $\rightarrow$ Advanced $\rightarrow$ JA2S

Whenever this mod is enabled, configuration options appear under the **`JA2S`** tab in **Options $\rightarrow$ Advanced** allowing players to customize reaction preferences and Battlescape settings.

---

## 4. How It Affects Gameplay & Enemies

* **Player Gameplay**: Grants complete tactical authority during reaction opportunities, enabling lethal doorway auto-burst ambushes, emergency cooked smoke screens against rushing terror units, and disciplined fire control.
* **Enemy Aliens**: Aliens become vastly more lethal at close range with automatic weapons and at extreme sniper ranges, ending predictable alien snap-shot reaction routines.

---

## 5. Related Documentation

* [ja2_stracciatella_reaction_guide.md](../../../docs_ja2s_mod/ja2_stracciatella_reaction_guide.md) — Comprehensive guide covering interrupt conditions, reaction score math, inventory rules, and AI behaviors.
* [ja2_stracciatella_mods_overview.md](../../../docs_ja2s_mod/ja2_stracciatella_mods_overview.md) — Combined overview of all Jagged Alliance 2 Stracciatella mods vs. vanilla OpenXcom.
