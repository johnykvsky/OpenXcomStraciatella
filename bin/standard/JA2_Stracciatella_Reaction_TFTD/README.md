# Jagged Alliance 2 Straciatella Reaction (TFTD) for OpenXcom (OXCE)

This mod brings **interactive interrupts, tactical weapon mode selection, cooked grenades, and smart AI reaction fire** from Jagged Alliance 2 into **Terror from the Deep (xcom2)**.

---

## 1. Features Summary

1. **Interactive Reaction Modal Prompt (`interactiveReaction: true`)**: Pauses execution on reaction fire, allowing the player to select Snap Shot, Auto Burst, Aimed Shot, Throw Grenade, or Hold Fire.
2. **Physical Inventory Authenticity (`reactionAllowBeltGrenade: true`)**: Grenades can be reaction-thrown from hands or quick-draw belt.
3. **Cooked Grenades (`reactionImpactDetonation: true`)**: Reaction-thrown grenades detonate immediately upon impact ("cooked grenade"); dye grenades deploy instant smoke/dye cover.
4. **Hold Fire / Pass (`reactionAllowPass: true`)**: Conserves Time Units without breaking ambush positioning.
5. **Smart Alien AI Reactions (`reactionAIModeSelection: true`)**: Enemies unleash point-blank Auto Bursts at close range ($\le 6$ tiles) and take precise Aimed Shots at sniper range ($\ge 15$ tiles).

---

## 2. Related Documentation

* [ja2_stracciatella_reaction_guide.md](../../../docs_ja2s_mod/ja2_stracciatella_reaction_guide.md) — Comprehensive guide covering interrupt conditions, reaction score math, inventory rules, and AI behaviors.
* [ja2_stracciatella_mods_overview.md](../../../docs_ja2s_mod/ja2_stracciatella_mods_overview.md) — Combined overview of all Jagged Alliance 2 Stracciatella mods.
