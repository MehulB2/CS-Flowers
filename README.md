# CS-Flowers
CS Flowers
Turn-Based 2D Strategy Game

Overview
CS Flowers is a C-based, turn-based strategy game where the player, equipped with giant scissors, must battle aggressive mutant flowers that have taken over the UNSW campus. The player navigates the Physics Lawn, cuts down flowers, avoids obstacles, and triggers reactions from enemies as they evolve through different states. The game emphasises strategic planning, spatial awareness, and careful movement.

Game Phases
Setup Phase
Spawn Player – Player chooses a valid starting tile on the map edge.

Add Foliage – Player adds branches as obstacles within inner map boundaries.

Spawn Flowers – Dormant enemy flowers are placed on odd-only coordinate tiles. If 25+ flowers are requested, they fill all valid odd-only tiles.

Gameplay Phase (Turn-Based Loop)
Each turn has 4 steps:

Player Step – The player can move, idle, or cut adjacent tiles.

Upkeep Step – Updates map elements like BRANCH tiles (used in later stages).

Flower Step – Flowers take actions (fully implemented in Stage 3).

End Step – Game checks for win/loss state.

Features by Stages of Development
Stage 1 – Setup Mechanics
Spawn the player by scanning a position on the map boundary (row or column is 0 or max).

Validate spawn location, re-prompting until valid.

Add branch tiles as foliage on the interior of the map (not on edges).

Reject invalid foliage positions with an error message.

Branches are represented as _/– and must not overlap or appear on edges.

Stage 2 – Player Movement & Combat
Implements a command loop that reads and executes player commands:

w, a, s, d – Move (blocked by BUSH, FLOWER, or map edges)

i – Idle

c w/a/s/d – Cut in a direction

Player can cut adjacent FLOWER or BUSH tiles (they become EMPTY).

Walking over a BRANCH or cutting a flower triggers a 5x5 alert zone, causing nearby flowers to increase their state:

DORMANT → DISTURBED → AWAKENED

Stage 3 – Flower Behaviour
At the Flower Step of each turn:

Flowers in AWAKENED or ENERGETIC states take action.

A flower in AWAKENED places a ROOT wall at a random adjacent tile.

A flower in ENERGETIC places a POLLEN trap or transforms a nearby SEEDLING to a FLOWER.

Ensures enemy actions are bounded within the map and follow tile rules.

Adds depth and complexity to enemy strategy.

Controls Summary
w, a, s, d – Move

i – Idle

c w/a/s/d – Cut in the specified direction

