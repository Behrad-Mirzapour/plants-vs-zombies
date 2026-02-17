#include "helpers.h"
#include "entities.h"
#include "raylib.h"

bool RowHasPlant(int row, PlantType type) {
    for (int i = 0; i < MAX_PLANTS; i++) {
        if (plants[i].active && plants[i].row == row && plants[i].type == type) {
            return true;
        }
    }
    return false;
}

void SpawnSun(float x, float y, float targetY, bool isSun, int value) {
    for (int i = 0; i < MAX_SUNS; i++) {
        if (!suns[i].active) {
            suns[i].active = true;
            suns[i].x = x;
            suns[i].y = y;
            suns[i].targetY = targetY;
            suns[i].value = value;
            suns[i].lifeTime = 0;
            suns[i].isSun = isSun;
            return;
        }
    }
}

void SpawnProjectile(int row, float x) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projectiles[i].active) {
            projectiles[i].active = true;
            projectiles[i].row = row;
            projectiles[i].x = x;
            projectiles[i].damage = 20;
            return;
        }
    }
}

void SpawnExplosion(float x, float y) {
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        if (!explosions[i].active) {
            explosions[i].active = true;
            explosions[i].x = x;
            explosions[i].y = y;

            PlaySound(explosionSounds[explosionSoundIdx]);
            explosionSoundIdx++;
            if (explosionSoundIdx >= MAX_SFX_INSTANCES) explosionSoundIdx = 0;
            
            // Calculate duration based on frame count and FPS
            // Example: 15 frames / 60 FPS = 0.25 seconds
            if (explosionAnimFPS > 0) {
                explosions[i].duration = (float)explosionAnimFrames / (float)explosionAnimFPS;
            }
            explosions[i].elapsedTime = 0;
            return;
        }
    }
}

// Recalculate row weight 
void RecalculateRowWeight(int r) {
    if (r < 0 || r >= GRID_ROWS) return;

    int newPlantNumber = 0;
    float newPlantHealthRatio = 0.0f;
    float newSquaredPlantHealthRatio = 0.0f;

    for (int i = 0; i < MAX_PLANTS; i++) {
        if (plants[i].active && plants[i].row == r && plants[i].type != P_SUNFLOWER && plants[i].type != P_ROSE) {
            if (plants[i].maxHp > 0) {
                float ratio = plants[i].hp / plants[i].maxHp;
                newPlantHealthRatio += ratio;
                newSquaredPlantHealthRatio += ratio * ratio;
            }
            newPlantNumber++;
        }
    }

    int newMower = (mowers[r].active && !mowers[r].triggered) ? 1 : 0;

    weights[r].plantNumber = newPlantNumber;
    weights[r].plantHealthRatio = newPlantHealthRatio;
    weights[r].squaredPlantHealthRatio = newSquaredPlantHealthRatio;
    weights[r].mower = newMower;

    // detA logic
    float n = (float)weights[r].plantNumber;
    float Q = weights[r].plantHealthRatio;
    float S = weights[r].squaredPlantHealthRatio;
    weights[r].detA = 1 + (S * S) + (Q * Q) + (n * n);
    
    // Weight: Lower is weaker. 
    weights[r].weight = 10 * weights[r].mower + weights[r].detA;
}

// Find row with minimum weight (weakest defense)
int GetMinWeightRow() {
    int minRow = 0;
    float minVal = 99999.0f;
    
    for(int i=0; i<GRID_ROWS; i++) {
        if(weights[i].weight < minVal) {
            minVal = weights[i].weight;
            minRow = i;
        }
    }
    return minRow;
}

void SpawnZombie() {
    ZombieType typeToSpawn = Z_NORMAL;
    
    // Decide type based on pool
    if (zombiesToSpawnNormal > 0 && zombiesToSpawnThinker > 0) {
        if(GetRandomValue(0, 1) == 0) { typeToSpawn = Z_NORMAL; if (currentLevel != 4) zombiesToSpawnNormal--; }
        else { typeToSpawn = Z_THINKER; if (currentLevel != 4) zombiesToSpawnThinker--; }
    } else if (zombiesToSpawnNormal > 0) {
        typeToSpawn = Z_NORMAL; zombiesToSpawnNormal--;
    } else if (zombiesToSpawnThinker > 0) {
        typeToSpawn = Z_THINKER; zombiesToSpawnThinker--;
    } else {
        return; // No more to spawn
    }

    for (int i = 0; i < MAX_ZOMBIES; i++) {
        if (!zombies[i].active) {
            zombies[i].active = true;
            zombies[i].type = typeToSpawn;
            
            // Logic: Thinker spawns in weakest row, Normal spawns random
           /* if(typeToSpawn == Z_THINKER) {
                zombies[i].row = GetMinWeightRow();
            } else {
                zombies[i].row = GetRandomValue(0, GRID_ROWS - 1);
            } */

            zombies[i].row = GetRandomValue(0, GRID_ROWS - 1);
            zombies[i].x = SCREEN_WIDTH;
            zombies[i].y = GRID_OFFSET_Y + zombies[i].row * CELL_HEIGHT; 
            zombies[i].maxHp = 200;
            zombies[i].hp = zombies[i].maxHp;
            zombies[i].speed = ZOMBIE_SPEED;
            zombies[i].isEating = false;
            zombiesSpawned++;
            return;
        }
    }
}
