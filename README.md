# plants-vs-zombies

A Plants vs. Zombies clone, build in C using Raylib

## Overview

This is a turn-based strategy game where you defend your lawn from waves of advancing zombies by strategically placing plants that attack or slow down incoming threats. Each plant has unique abilities and costs, requiring careful resource management and tactical planning.

## Features

### Game Modes

- **Level 1 - Normal Day**: Face regular zombies with standard gameplay
- **Level 2 - Dark Night**: Limited visibility with thinker-type zombies; no sunflowers available but high starting sun
- **Level 3 - Day Without Gas**: No lawnmowers available; challenging no-mower scenario
- **Level 4 - The Endless Day**: Infinite wave survival mode with continuous zombie spawns

### Playable Plants

| Plant | Cost | Cooldown | Effect |
|-------|------|----------|--------|
| **Sunflower** | 50 | 3s | Generates sun currency |
| **Peashooter** | 100 | 10s | Fires projectiles at zombies |
| **Chomper** | 150 | ? | Bites and instantly kills zombies |
| **Rose** | 200 | ? | Area effect attack plant |
| **Potato Mine** | 25 | ? | Explosive trap (unlockable) |

### Enemy Types

- **Normal Zombies**: Standard enemies with regular movement and attack patterns
- **Thinker Zombies**: Intelligent zombies that switch lanes strategically to avoid plants

### Gameplay Mechanics

- **Sun Currency System**: Collect falling sun drops to purchase plants
- **Tower Defense Grid**: 5 rows × 9 columns placement system
- **Lawnmowers**: Automatic defense mechanism that activates when zombies reach the end
- **Pause/Resume**: Manage the game flow with pause functionality
- **Row Weight System**: Tracks zombie distribution to inform defense strategy
- **Explosive Effects**: Visual explosion system for plant attacks

## Project Structure

```
Src/
├── main.c             # Entry point and game loop
├── game.c/h           # Core game logic and level initialization
├── entities.h         # Global entity definitions and declarations
├── data.c/h           # Game data structures and utilities
├── drawing.c/h        # Rendering and animation system
├── helpers.c/h        # Utility functions
├── defs.h             # Constants, macros, and configuration
└── README.md          # This file
```

### Key Files

- **main.c**: Contains the main game loop, global state management, and screen transitions
- **game.c/h**: Implements game initialization, updates, and level configuration
- **entities.h**: Declares all game entities (plants, zombies, projectiles, etc.)
- **drawing.c/h**: Handles rendering of sprites, animations, UI elements, and visual effects
- **helpers.c/h**: Provides utility functions for game calculations and entity management

## Technical Details

### Dependencies

- **Raylib**: Graphics and audio library for game development

### Game Constants

- **Screen Resolution**: 2560 × 1600
- **Grid Layout**: 5 rows × 9 columns (208×210 pixel cells)
- **Object Limits**:
  - Max Plants: Configurable
  - Max Zombies: Configurable
  - Max Projectiles: Configurable
  - Max Sun Drops: Configurable
  - Max Explosions: Configurable

### Movement Speeds

- Sun Fall Rate: 10.0 units/frame
- Zombie Speed: 50.0 units/frame
- Projectile Speed: 300.0 units/frame
- Lawnmower Speed: 400.0 units/frame
- Lane Switch Speed: 40.0 units/frame (Thinker zombies)

## Building and Running

### Prerequisites

- C compiler (GCC/Clang)
- Raylib library installed

### Compilation

```bash
gcc main.c game.c drawing.c data.c helpers.c -o plants_vs_zombies -lraylib -lm
```

### Execution

```bash
./plants_vs_zombies
```

## Gameplay Tips

1. **Sunflowers First**: Plant sunflowers early to generate income for better plants
2. **Layer Your Defense**: Mix attack and support plants for optimal coverage
3. **Watch the Rows**: Pay attention to which lanes zombies prefer (tracked by row weight)
4. **Plan Your Economy**: Manage sun carefully—don't waste it on poorly positioned plants
5. **Level Select**: Each level has unique challenges; adapt your strategy accordingly

## Game Controls

- **Mouse**: Click to select and place plants
- **Keyboard**: Navigate menus and manage game state
- **P Key**: Pause/Resume gameplay

## Architecture Highlights

### Entity System

The game uses parallel arrays for efficient entity management:
- Plants, Zombies, Projectiles, Sun drops, Explosions, Lawnmowers

### Animation System

Macro-based animation system supporting:
- GIF loading and playback
- Frame-by-frame animation updates
- Efficient texture switching

### Game State

Centralized global state management for:
- Current screen (Menu, Game, Pause, etc.)
- Resource tracking (Sun currency, Coins)
- Entity collections
- Game configuration per level

## Future Enhancements

- Additional plant varieties
- More zombie types with special abilities
- Power-ups and special items
- Difficulty scaling
- Leaderboard system
- Customizable difficulty settings
- Save/Load game states

## License

This is an educational implementation of the Plants vs Zombies game concept.

## Authors

Behrad Mirzapour

---

**Status**: Active Development

For questions or contributions, refer to the project structure and existing code patterns.
