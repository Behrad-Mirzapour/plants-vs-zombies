#ifndef DEFS_H
#define DEFS_H

#include "raylib.h"
#include <stdbool.h>

// -------------------------------------------------------------------------
// MACROS
// -------------------------------------------------------------------------
#define LOAD_GIF(name, gifPath) \
    do { \
        name##AnimImage = LoadImageAnim(gifPath, &name##AnimFrames); \
        name##AnimTexture = LoadTextureFromImage(name##AnimImage); \
    } while(0)

#define UPDATE_ANIMATION(name) \
        do { \
            if (name##AnimFrames > 0) { \
                name##AnimFrameCounter++; \
                if (name##AnimFrameCounter >= name##AnimFrameDelay) { \
                    name##AnimCurrentFrame++; \
                    if (name##AnimCurrentFrame >= name##AnimFrames) name##AnimCurrentFrame = 0; \
                    unsigned int nextFrameDataOffset = name##AnimImage.width*name##AnimImage.height*4*name##AnimCurrentFrame; \
                    UpdateTexture(name##AnimTexture, ((unsigned char*)name##AnimImage.data) + nextFrameDataOffset); \
                    name##AnimFrameCounter = 0; \
                } \
            } \
        } while(0)

// -------------------------------------------------------------------------
// CONSTANTS & CONFIGURATION
// -------------------------------------------------------------------------
#define SCREEN_WIDTH 2560
#define SCREEN_HEIGHT 1600
#define GRID_ROWS 5
#define GRID_COLS 9
#define GRID_OFFSET_X 620
#define GRID_OFFSET_Y 420
#define CELL_WIDTH 208
#define CELL_HEIGHT 210

// Gameplay Constants
#define SUN_FALL_RATE 10.0f    
#define ZOMBIE_SPAWN_RATE 10.0f // Slightly faster for flow
#define PROJECTILE_SPEED 300.0f
#define ZOMBIE_SPEED 50.0f
#define LAWNMOWER_SPEED 400.0f
#define LANE_SWITCH_SPEED 40.0f // How fast Thinkers move up/down

// Plant Costs
#define COST_SUNFLOWER 50
#define COST_PEASHOOTER 100
#define COST_CHOMPER 150
#define COST_ROSE 200
#define COST_POTATO 25

// Plant Cooldowns (seconds)
#define COOLDOWN_SUNFLOWER 3.0f
#define COOLDOWN_PEASHOOTER 10.0f
#define COOLDOWN_CHOMPER 30.0f
#define COOLDOWN_ROSE 50.0f
#define COOLDOWN_POTATO 6.0f

// -------------------------------------------------------------------------
// ENUMS & STRUCTS
// -------------------------------------------------------------------------
typedef enum { MENU, LEVEL_SELECT, SHOP, GAME, GAME_OVER, WIN } GameScreen;
typedef enum { P_SUNFLOWER, P_PEASHOOTER, P_CHOMPER, P_ROSE, P_POTATO } PlantType;
typedef enum { Z_NORMAL, Z_THINKER } ZombieType;

typedef struct {
    bool active;
    float x, y;
    float targetY;
    float lifeTime;
    int value;
    bool isSun; // To differentiate between sun and coins
} Sun;

typedef struct {
    bool active;
    PlantType type;
    int col, row;
    float hp;
    float maxHp;
    float timer;      
    float lifeSpan;   
    bool isArmed;     
} Plant;

typedef struct {
    bool active;
    ZombieType type; // Normal or Thinker
    int row;
    float x;
    float y;         // Added Y for smooth movement
    float hp;
    float maxHp;
    float speed;
    float attackTimer;
    bool isEating;
} Zombie;

typedef struct {
    bool active;
    int row;
    float x;
    int damage;
} Projectile;

typedef struct {
    bool active;
    bool triggered;
    int row;
    float x;
} Lawnmower;

typedef struct {
    bool active;
    float x, y;
    float duration;
    float elapsedTime;
} Explosion;

typedef struct {
    int row;
    float weight;
    float plantNumber;
    float plantHealthRatio;
    float squaredPlantHealthRatio;
    float detA;
    int mower;
} RowWeight;

// Stage Configuration
typedef struct {
    int normalCount;
    int thinkerCount;
    bool mowersActive;
    bool sunflowersAllowed;
    bool naturalSun; // Does sun fall from sky?
    int startSun;
} LevelConfig;

// -------------------------------------------------------------------------
// ENTITY POOL SIZES
// -------------------------------------------------------------------------
#define MAX_PLANTS 45
#define MAX_ZOMBIES 100
#define MAX_PROJECTILES 100
#define MAX_SUNS 50
#define MAX_EXPLOSIONS 50
#define MAX_SFX_INSTANCES 20

#endif // DEFS_H
