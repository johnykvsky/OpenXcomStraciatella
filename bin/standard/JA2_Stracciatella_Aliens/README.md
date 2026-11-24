# Jagged Alliance 2 Straciatella Aliens for OpenXcom (OXCE)

This mod brings **asymmetrical night vision tiers, thermal and psionic senses, suppression immunities, acoustic movement stealth, auditory hearing acuity, and spotter/sniper AI roles** to all UFO Defense enemies.

---

## 1. Features Summary

1. **Asymmetrical Night Vision & Optics**:
   - Replaces the flat 20-tile alien night omniscience with authentic tiers:
     - **Biological Infantry (14–16 tiles)**: Sectoids (14), Floaters (14–15), Mutons (15), Snakemen (16).
     - **Apex Predators (16–18 tiles)**: Reapers (16), Chryssalids (18).
     - **Technological & Psionic Masters (20 tiles)**: Cyberdiscs (20), Sectopods (20), Ethereals (20).
2. **Thermal Smoke Vision (`heatVision`)**:
   - Cyberdiscs and Sectopods possess 100% thermal optics, ignoring smoke screens.
   - Snakemen possess 50% thermal vision via pit organs.
   - Ethereals pierce smoke with 75%–100% psionic clairvoyance.
3. **Psionic Wall Penetration (`psiVision`)**:
   - Ethereals detect human minds directly through solid walls (12–16 tiles).
4. **Suppression Resistances & Forced Kneeling**:
   - Humanoids (Mutons, Sectoids) can be pinned down, forced to kneel, and lose overwatch. Mutons leverage kneeling for heavy weapon bipod bonuses.
   - Autonomous machines (Cyberdiscs, Sectopods) and mindless horrors (Chryssalids, Silacoids, Zombies) possess total suppression immunity (`fearImmune: true`, `allowsKneeling: false`).
5. **Species Acoustic Footprints & Auditory Acuity**:
   - **Stealth Hunters**: Snakemen (`noiseMovementFactor: 0.5`) and Chryssalids (`0.6`) move with muffled footsteps (2–3 tiles). Ethereals levitate with near-silent drift (`0.3`).
   - **Heavy Machinery**: Sectopods (`2.0`) and Cyberdiscs/Reapers (`1.5`) clank loudly through terrain (8–12 tiles).
   - **Hearing Acuity**: Chryssalids (`hearingBonus: 4`) and Reapers (`5`) hear subtle footsteps from further away, while sealed Muton helmets (`-2`) and Sectopod mechanical audio filters (`-3`) muffle subtle sounds.
6. **Spotter & Sniper AI Networks**:
   - Fast scouts (Floaters, Cyberdiscs, Sectoid Navigators) designate player positions (`spotter: 1–2`).
   - Heavy gunners (Muton Engineers with Heavy Plasma, Sectopods) act as snipers (`sniper: 1–3`), neutralizing the 50% BVR penalty when firing on spotted targets.

---

## 2. Related Documentation

* [ja2_stracciatella_alien_rebalance_guide.md](../../../docs_ja2s_mod/ja2_stracciatella_alien_rebalance_guide.md) — Comprehensive tactical manual detailing species stats, sensory formulas, and mechanics.
* [ja2_stracciatella_awareness_guide.md](../../../docs_ja2s_mod/ja2_stracciatella_awareness_guide.md) — Tactical acoustics, noise attenuation formulas, and alert states.
* [ja2_stracciatella_mods_overview.md](../../../docs_ja2s_mod/ja2_stracciatella_mods_overview.md) — Combined overview of all Jagged Alliance 2 Stracciatella mods.
