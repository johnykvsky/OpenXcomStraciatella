# Jagged Alliance 2 Straciatella Aliens (TFTD) for OpenXcom (OXCE)

This mod brings **asymmetrical night vision tiers, thermal and psionic senses, suppression immunities, acoustic movement stealth, auditory hearing acuity, and spotter/sniper AI roles** to all **Terror from the Deep (xcom2)** enemies.

---

## 1. Features Summary

1. **Asymmetrical Night Vision & Murky Water Optics**:
   - Replaces flat 20-tile alien omniscience with authentic tiers:
     - Biological Grunts: Aquatoids (14), Gillmen (15), Lobstermen (15).
     - Elite Warriors & Terror Beasts: Tasoths (16), Triscenes (16), Calcinite (16), Tentaculats (18).
     - Automated & Deep-Sea Masters: Bio-Drones (20).
2. **Thermal Smoke / Silt Vision (`heatVision`)**:
   - Bio-Drones & Hallucinoids possess 100% thermal optics, ignoring smoke/dye screens.
   - Gillmen & Deep Ones possess 50% thermal vision via aquatic sensory pits.
   - Xarquids possess 75% thermal sense.
3. **Suppression Resistances & Forced Kneeling**:
   - Humanoids (Aquatoids, Gillmen, Tasoths) dive for cover under heavy fire (`allowsKneeling: true`). Tasoths leverage kneeling for bipod bonuses on heavy sonic weapons.
   - Heavy armored horrors (Lobstermen, Tentaculats, Bio-Drones, Triscenes, Xarquids) possess suppression immunity (`fearImmune: true`, `allowsKneeling: false`).
4. **Species Acoustic Footprints & Auditory Acuity**:
   - **Stealth Hunters**: Tentaculats (`noiseMovementFactor: 0.5`) and Gillmen (`0.6`) glide and slither with muffled contact sound. Hallucinoids drift in near silence (`0.2`).
   - **Heavy Machinery & Beasts**: Triscenes (`1.8`) and Lobstermen (`1.4`) produce loud underwater vibrations.
   - **Hearing Acuity**: Triscenes (`+5`) and Tentaculats (`+4`) detect subtle vibrations and door breaches from afar; sealed Tasoth helmets (`-2`) dampen ambient sounds.
5. **Spotter & Sniper AI Networks**:
   - Fast scouts (Bio-Drones, Aquatoid Navigators, Lobsterman Navigators) designate player positions (`spotter: 1–2`).
   - Heavy gunners (Tasoths with Sonic Cannons, Xarquids) act as snipers (`sniper: 1–2`), neutralizing the 50% BVR penalty when firing on spotted targets.

---

## 2. Related Documentation

* [ja2_stracciatella_alien_rebalance_guide.md](../../../docs_ja2s_mod/ja2_stracciatella_alien_rebalance_guide.md) — Comprehensive tactical manual detailing species stats, sensory formulas, and mechanics.
* [ja2_stracciatella_mods_overview.md](../../../docs_ja2s_mod/ja2_stracciatella_mods_overview.md) — Combined overview of all Jagged Alliance 2 Stracciatella mods.
