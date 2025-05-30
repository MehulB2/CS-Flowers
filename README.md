# 🌼 CS Flowers
**Turn-Based 2D Strategy Game (C Language)**

## Overview
**CS Flowers** is a turn-based strategy game developed in C where you play as a lone warrior equipped with giant scissors. Your mission: stop the invasion of mutant flowers taking over the UNSW campus. Using strategic movement, precise cutting, and environmental awareness, you must outwit and outlast the evolving floral threat.

---

## Game Phases

### Setup Phase
- **Spawn Player**: Choose a valid edge tile as your starting position.
- **Add Foliage**: Add *branch* obstacles to the map's inner tiles.
- **Spawn Flowers**: Place dormant enemy flowers on *odd-only* coordinates. If 25+ are requested, all odd-only tiles will be filled.

### Gameplay Phase (Turn-Based Loop)
Each turn follows this structure:
1. **Player Step** – Move, idle, or cut adjacent tiles.
2. **Upkeep Step** – (Used in later stages)
3. **Flower Step** – Flowers perform actions based on their state.
4. **End Step** – Check for game completion.

---

## Features by Stages of Development

### Stage 1 – Map Setup
- Spawn player on edge tiles (`row == 0` or `MAX_ROW - 1`, or column on edges).
- Validate spawn location and re-prompt if invalid.
- Add *branches* as foliage (non-edge tiles only).
- Reject invalid foliage positions with `Invalid foliage position!`.

### Stage 2 – Player Mechanics
- Implement command loop:
  - `w`, `a`, `s`, `d`: Move
  - `i`: Idle
  - `c [direction]`: Cut in direction (`w`, `a`, `s`, `d`)
- Player can't move into `BUSH`, `FLOWER`, or off-map tiles.
- Can walk over and consume `BRANCH`.
- Cutting a flower or stepping on a branch triggers a **5x5 alert zone**.
- Flowers in the zone evolve their state:
  - `DORMANT → DISTURBED → AWAKENED`

### Stage 3 – Enemy Flower Behaviour
- In **Flower Step**, flowers act based on state:
  - `AWAKENED`: Places `ROOT` walls nearby.
  - `ENERGETIC`: Places `POLLEN` traps or grows `SEEDLING` into `FLOWER`.
- Enemy actions are constrained within the map and follow tile rules.

---

## Controls Summary
| Command | Action |
|---------|--------|
| `w`, `a`, `s`, `d` | Move up, left, down, right |
| `i` | Idle |
| `c w/a/s/d` | Cut in the given direction |
| `Ctrl+D` | Exit game loop |

---

