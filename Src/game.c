#include "game.h"
#include "entities.h"
#include "helpers.h"
#include "data.h"
#include "raylib.h"
#include <math.h>

void InitGame(int level) {
    currentLevel = level;
    gameTime = 0;
    zombieSpawnTimer = 0;
    sunSpawnTimer = 0;
    zombiesSpawned = 0;
    notPaused = true;
    selectedPlant = P_PEASHOOTER;
    
    // Define Level Configs
    LevelConfig config;
    if (level == 1) { 
        // Normal Day
        config.normalCount = 30; config.thinkerCount = 0; 
        config.mowersActive = true; config.sunflowersAllowed = true; 
        config.startSun = 150; config.naturalSun = true;
    } else if (level == 2) { 
        // Dark Night
        config.normalCount = 15; config.thinkerCount = 15; 
        config.mowersActive = true; config.sunflowersAllowed = false; 
        config.startSun = 1000; config.naturalSun = false; // Night time
    } else if (level == 3) { 
        // Day without Gas
        config.normalCount = 0; config.thinkerCount = 30; 
        config.mowersActive = false; config.sunflowersAllowed = true; 
        config.startSun = 150; config.naturalSun = true;
    } else {
        // The Endless Day
        config.normalCount = 1; config.thinkerCount = 1; 
        config.mowersActive = true; config.sunflowersAllowed = true; 
        config.startSun = 150; config.naturalSun = true;
    }

    sunCurrency = config.startSun;
    zombiesToSpawnNormal = config.normalCount;
    zombiesToSpawnThinker = config.thinkerCount;
    totalLevelZombies = config.normalCount + config.thinkerCount;
    
    // Clear Arrays
    for(int i=0; i<MAX_PLANTS; i++) plants[i].active = false;
    for(int i=0; i<MAX_ZOMBIES; i++) zombies[i].active = false;
    for(int i=0; i<MAX_PROJECTILES; i++) projectiles[i].active = false;
    for(int i=0; i<MAX_SUNS; i++) suns[i].active = false;
    for(int i=0; i<MAX_EXPLOSIONS; i++) explosions[i].active = false;
    for(int i=0; i<5; i++) cooldownTimer[i] = 999; // Ready to plant immediately

    // Reset Mowers And Weights
        for(int i=0; i<GRID_ROWS; i++) {
        // Mowers
        mowers[i].active = config.mowersActive;
        mowers[i].triggered = false;
        mowers[i].row = i;
        mowers[i].x = GRID_OFFSET_X - 50;
        
        // Initial Weight Calc
        RecalculateRowWeight(i);
    }
}

void UpdateMusicSystem() {
    char* targetMusicPath = "";
    
    // Determine which song SHOULD be playing
    if (currentScreen == MENU || currentScreen == LEVEL_SELECT || currentScreen == SHOP) {
        if (currentMusicLevelID != 0) {
            targetMusicPath = "resources/sound&music/Plants vs. Zombies - Choose Your Seeds.mp3";
            currentMusicLevelID = 0;
        }
    } else {
        // Game, Win, or Lose - Music depends on Level
        if (currentLevel == 1 && currentMusicLevelID != 1) {
            targetMusicPath = "resources/sound&music/Plants vs. Zombies - Grasswalk.mp3";
            currentMusicLevelID = 1;
        } else if (currentLevel == 2 && currentMusicLevelID != 2) {
            targetMusicPath = "resources/sound&music/Plants vs. Zombies - Cerebrawl.mp3";
            currentMusicLevelID = 2;
        } else if (currentLevel == 3 && currentMusicLevelID != 3) {
            targetMusicPath = "resources/sound&music/Plants vs. Zombies - Graze the Roof.mp3";
            currentMusicLevelID = 3;
        } else if (currentLevel == 4 && currentMusicLevelID != 4) {
            targetMusicPath = "resources/sound&music/Plants vs. Zombies - Loonboon.mp3";
            currentMusicLevelID = 4;
        }
    }


    // If we need to switch tracks
    if (targetMusicPath[0] != '\0') {
        StopMusicStream(bgMusic);
        UnloadMusicStream(bgMusic);
        bgMusic = LoadMusicStream(targetMusicPath);
        PlayMusicStream(bgMusic);
    }

    // PAUSE LOGIC: 
    if ((currentScreen == GAME && !notPaused) || currentScreen == GAME_OVER || currentScreen == WIN) {
        PauseMusicStream(bgMusic);
    } else {
        ResumeMusicStream(bgMusic);
    }   

    UpdateMusicStream(bgMusic);
}

void UpdateGame(float dt) {
    gameTime += dt;
    
    // Determine if natural sun falls
    bool naturalSun = (currentLevel != 2); 

    // --- Sunlight Spawning ---
    if (naturalSun) {
        sunSpawnTimer += dt;
        if (sunSpawnTimer > SUN_FALL_RATE) {
            float randX = GetRandomValue(GRID_OFFSET_X, SCREEN_WIDTH - 256);
            float randY = GetRandomValue(GRID_OFFSET_Y, SCREEN_HEIGHT - 229);
            SpawnSun(randX, -114, randY, true, 25);
            sunSpawnTimer = 0;
        }
    }

    // --- Zombie Spawning ---
    if (zombiesSpawned < totalLevelZombies || currentLevel == 4) {
        zombieSpawnTimer += dt;
        if (zombieSpawnTimer > ZOMBIE_SPAWN_RATE) {
            SpawnZombie();
            zombieSpawnTimer = 0;
            // Ramp up difficulty slightly
            if(zombieSpawnTimer > 2.0f) zombieSpawnTimer -= 0.1f; 
        }
    } else {
        // Check Win Condition: All spawned AND none active
        bool zombiesAlive = false;
        for(int i=0; i<MAX_ZOMBIES; i++) if(zombies[i].active) zombiesAlive = true;
        if(!zombiesAlive && zombiesSpawned >= totalLevelZombies){
            currentScreen = WIN;
            PlaySound(winSound);
        }
    }

    // --- Update Plants ---
    for (int i = 0; i < MAX_PLANTS; i++) {
        if (!plants[i].active) continue;
        Plant *p = &plants[i];
        p->timer += dt;

        if (p->type == P_ROSE) {
            p->lifeSpan -= dt;
            bool ThereIsWeightedPlants = false;
            if(p->lifeSpan <= 0) p->active = false;
            if(p->timer > 1.0f) {
                for(int j=0; j<MAX_PLANTS; j++) {
                    if(plants[j].active && plants[j].row == p->row && i != j) {
                        plants[j].hp += (plants[j].maxHp * 0.10f);
                        if(plants[j].hp > plants[j].maxHp) plants[j].hp = plants[j].maxHp;
                        if (plants[j].type != P_SUNFLOWER && plants[j].type != P_ROSE) ThereIsWeightedPlants = true;
                    }
                }
                p->timer = 0;
                 if (ThereIsWeightedPlants) RecalculateRowWeight(p->row);
            }
        } 
        else if (p->type == P_SUNFLOWER) {
            if (p->timer > 10.0f) { 
                    SpawnSun(GRID_OFFSET_X + p->col * CELL_WIDTH, GRID_OFFSET_Y + p->row * CELL_HEIGHT, GRID_OFFSET_Y + p->row * CELL_HEIGHT + 91, true, 25);
                p->timer = 0;
            }
        } 
        else if (p->type == P_PEASHOOTER) {
            float fireRate = 2.0f;
            if (RowHasPlant(p->row, P_ROSE)) fireRate *= 0.8f;
            
            bool zombieInRow = false;
            for(int z=0; z<MAX_ZOMBIES; z++) {
                if(zombies[z].active && zombies[z].row == p->row && zombies[z].x > GRID_OFFSET_X + p->col * CELL_WIDTH) {
                    zombieInRow = true; break;
                }
            }
            if (zombieInRow && p->timer > fireRate) {
                PlaySound(throwSound);
                SpawnProjectile(p->row, GRID_OFFSET_X + p->col * CELL_WIDTH + 102);
                p->timer = 0;
            }
        }
        else if (p->type == P_CHOMPER) {
             p->lifeSpan -= dt;
             if(p->lifeSpan <= 0) { p->active = false; RecalculateRowWeight(p->row); }
        }
        else if (p->type == P_POTATO) {
            if (p->timer > 14.0f) p->isArmed = true;
        }

        if(p->hp <= 0) {
            if (p->active) {
                p->active = false;
                if (p->type != P_SUNFLOWER && p->type != P_ROSE) RecalculateRowWeight(p->row);
            }
        }
    }

    // --- Update Projectiles ---
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projectiles[i].active) continue;
        projectiles[i].x += PROJECTILE_SPEED * dt;

        for (int z = 0; z < MAX_ZOMBIES; z++) {
            // Check collision using geometry, not just row index, because thinkers move
            Rectangle zRect = {zombies[z].x, zombies[z].y, 128, 183};
            Vector2 pPos = {projectiles[i].x - 30, GRID_OFFSET_Y + projectiles[i].row * CELL_HEIGHT + CELL_HEIGHT/2};
            
            if (zombies[z].active && CheckCollisionCircleRec(pPos, 1, zRect)) {
                // Only hit if roughly in same row visually (tolerance)
                if(fabs(zombies[z].y - (GRID_OFFSET_Y + projectiles[i].row * CELL_HEIGHT)) < 46) {
                    PlaySound(splatSounds[splatSoundIdx]);
                    splatSoundIdx++;
                    if (splatSoundIdx >= MAX_SFX_INSTANCES) splatSoundIdx = 0;
                    zombies[z].hp -= projectiles[i].damage;
                    projectiles[i].active = false;
                    if (zombies[z].hp <= 0) {
                        zombies[z].active = false;
                        int coinSpawnGenerator = GetRandomValue(0, 100);
                        if (coinSpawnGenerator < 10){
                            SpawnSun(zombies[z].x + 100, zombies[z].y, zombies[z].y + 91, false, 10);
                        } else if (coinSpawnGenerator < 15) {
                            SpawnSun(zombies[z].x + 100, zombies[z].y, zombies[z].y + 91, false, 25);
                        } else if (coinSpawnGenerator < 18) {
                            SpawnSun(zombies[z].x + 100, zombies[z].y, zombies[z].y + 91, false, 50);
                        } else if (coinSpawnGenerator < 19) {
                            SpawnSun(zombies[z].x + 100, zombies[z].y, zombies[z].y + 91, false, 100);
                        }
                        // Add score for level 4
                        if (currentLevel == 4) {
                            if (zombies[z].type == Z_NORMAL) {
                                level4Score += 100;
                            } else if (zombies[z].type == Z_THINKER) {
                                level4Score += 200;
                            }
                        }
                    }
                    break;
                }
            }
        }
        if (projectiles[i].x > SCREEN_WIDTH) projectiles[i].active = false;
    }

    // --- Update Mowers ---
    for(int i=0; i<GRID_ROWS; i++) {
        if(mowers[i].active && mowers[i].triggered) {
            mowers[i].x += LAWNMOWER_SPEED * dt;
            for(int z=0; z<MAX_ZOMBIES; z++) {
                if(zombies[z].active && zombies[z].row == i) {
                     if(mowers[i].x - 70 > zombies[z].x) {
                        // Add score for zombie killed by mower in level 4
                        if (currentLevel == 4) {
                            if (zombies[z].type == Z_NORMAL) {
                                level4Score += 100;
                            } else if (zombies[z].type == Z_THINKER) {
                                level4Score += 200;
                            }
                        }
                        zombies[z].active = false;
                        int coinSpawnGenerator = GetRandomValue(0, 100);
                        if (coinSpawnGenerator < 10){
                            SpawnSun(zombies[z].x + 100, zombies[z].y, zombies[z].y + 91, false, 10);
                        } else if (coinSpawnGenerator < 15) {
                            SpawnSun(zombies[z].x + 100, zombies[z].y, zombies[z].y + 91, false, 25);
                        } else if (coinSpawnGenerator < 18) {
                            SpawnSun(zombies[z].x + 100, zombies[z].y, zombies[z].y + 91, false, 50);
                        } else if (coinSpawnGenerator < 19) {
                            SpawnSun(zombies[z].x + 100, zombies[z].y, zombies[z].y + 91, false, 100);
                        }
                     }
                }
            }
            if(mowers[i].x > SCREEN_WIDTH + 100){
                mowers[i].active = false;
                RecalculateRowWeight(i);
            }
        }
    }

    // --- Update Zombies ---
    for (int i = 0; i < MAX_ZOMBIES; i++) {
        if (!zombies[i].active) continue;

        // --- zombies groan sound ---
        switch (GetRandomValue(0, 8000))
        {
        case 0:
            PlaySound(groanSounds[0]);
            break;
        case 1:
            PlaySound(groanSounds[1]);
            break;
        case 2:
            PlaySound(groanSounds[2]);
            break;
        case 3:
            PlaySound(groanSounds[3]);
            break;
        case 4:
            PlaySound(groanSounds[4]);
            break;
        case 5:
            PlaySound(groanSounds[5]);
            break;
        default:
            break;
        }


        // --- Thinker AI ---
        if (zombies[i].type == Z_THINKER && !zombies[i].isEating) {
            int curR = zombies[i].row;
            int bestR = curR;
            float minW = weights[curR].weight;

            // Check Above
            if (curR > 0) {
                if (weights[curR-1].weight < minW) {
                    minW = weights[curR-1].weight;
                    bestR = curR - 1;
                }
            }
            // Check Below
            if (curR < GRID_ROWS - 1) {
                if (weights[curR+1].weight < minW) {
                    bestR = curR + 1;
                }
            }

            // Move Y towards best row
            float targetY = GRID_OFFSET_Y + bestR * CELL_HEIGHT;
            
            // Smooth movement
            if (zombies[i].y < targetY) zombies[i].y += LANE_SWITCH_SPEED * dt;
            if (zombies[i].y > targetY) zombies[i].y -= LANE_SWITCH_SPEED * dt;

            // Update Discrete Row Index based on actual Y position
            zombies[i].row = (int)((zombies[i].y - GRID_OFFSET_Y + CELL_HEIGHT/2) / CELL_HEIGHT);
            
            // Clamp Row
            if(zombies[i].row < 0) zombies[i].row = 0;
            if(zombies[i].row >= GRID_ROWS) zombies[i].row = GRID_ROWS - 1;
        } 
        else {
            // Normal zombies just ensure Y aligns with Row
            zombies[i].y = GRID_OFFSET_Y + zombies[i].row * CELL_HEIGHT;
        }

        zombies[i].isEating = false;
        float currentSpeed = zombies[i].speed;
        if(RowHasPlant(zombies[i].row, P_CHOMPER)) currentSpeed /= 3.0f;

        // Collision with Plants
        Rectangle zombieRect = {zombies[i].x, zombies[i].y, 128, 183};
        
        for (int p = 0; p < MAX_PLANTS; p++) {
            if (plants[p].active && plants[p].row == zombies[i].row) {
                Rectangle plantRect = {GRID_OFFSET_X + plants[p].col * CELL_WIDTH, GRID_OFFSET_Y + plants[p].row * CELL_HEIGHT, 90, CELL_HEIGHT};
                
                if (CheckCollisionRecs(zombieRect, plantRect)) {
                    if(plants[p].type == P_POTATO && plants[p].isArmed) {
                        zombies[i].hp -= 500; 
                        float explosionX = GRID_OFFSET_X + plants[p].col * CELL_WIDTH + 45;
                        float explosionY = GRID_OFFSET_Y + plants[p].row * CELL_HEIGHT + CELL_HEIGHT / 2;
                        SpawnExplosion(explosionX, explosionY);
                        plants[p].active = false; 
                        RecalculateRowWeight(plants[p].row);
                        if(zombies[i].hp <= 0) {
                            // Add score for zombie killed by potato in level 4
                            if (currentLevel == 4) {
                                if (zombies[i].type == Z_NORMAL) {
                                    level4Score += 100;
                                } else if (zombies[i].type == Z_THINKER) {
                                    level4Score += 200;
                                }
                            }
                            zombies[i].active = false;
                            int coinSpawnGenerator = GetRandomValue(0, 100);
                            if (coinSpawnGenerator < 10){
                                    SpawnSun(zombies[i].x + 100, zombies[i].y, zombies[i].y + 91, false, 10);
                                } else if (coinSpawnGenerator < 15) {
                                    SpawnSun(zombies[i].x + 100, zombies[i].y, zombies[i].y + 91, false, 25);
                                } else if (coinSpawnGenerator < 18) {
                                    SpawnSun(zombies[i].x + 100, zombies[i].y, zombies[i].y + 91, false, 50);
                                } else if (coinSpawnGenerator < 19) {
                                    SpawnSun(zombies[i].x + 100, zombies[i].y, zombies[i].y + 91, false, 100);
                            }
                        }
                    } else {
                        zombies[i].isEating = true;

                        if (GetRandomValue(0, 30) == 0) { // Approx twice a second at 60FPS
                            PlaySound(chompSounds[chompSoundIdx]);
                            chompSoundIdx++;
                            if (chompSoundIdx >= MAX_SFX_INSTANCES) chompSoundIdx = 0;
                        }
                        plants[p].hp -= 30.0f * dt; 
                        if (plants[p].hp <= 0) {
                            plants[p].active = false;
                            if (plants[p].type != P_SUNFLOWER && plants[p].type != P_ROSE) RecalculateRowWeight(plants[p].row);
                        } else {
                            if (plants[p].type != P_SUNFLOWER && plants[p].type != P_ROSE) RecalculateRowWeight(plants[p].row);
                        }
                    }
                }
            }
        }

        // Mower Collision Trigger
        if (mowers[zombies[i].row].active && !mowers[zombies[i].row].triggered) {
            // Only trigger if roughly aligned vertically
            if (fabs(zombies[i].y - (GRID_OFFSET_Y + zombies[i].row * CELL_HEIGHT)) < 23) {
                if (zombies[i].x < mowers[zombies[i].row].x - 102) {
                    PlaySound(mowerSounds[mowerSoundIdx]);
                    mowerSoundIdx++;
                    if (mowerSoundIdx >= 5) mowerSoundIdx = 0;
                    mowers[zombies[i].row].triggered = true;
                    // Deduct score for triggering mower in level 4
                    if (currentLevel == 4) {
                        level4Score -= 5000;
                        if (level4Score < 0) level4Score = 0;
                    }
                }
            }
        }

        if (!zombies[i].isEating) {
            zombies[i].x -= currentSpeed * dt;
        }

        if (zombies[i].x < 350) {
            currentScreen = GAME_OVER;
            PlaySound(loseSound);
        }
    }

    // --- Update Sun ---
    for (int i = 0; i < MAX_SUNS; i++) {
        if (!suns[i].active) continue;
        if (suns[i].y < suns[i].targetY) {
            suns[i].y += 229 * dt;
        }
        suns[i].lifeTime += dt;
        if (suns[i].lifeTime > 10.0f) suns[i].active = false;
    }

    // --- Update Explosions ---
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        if (!explosions[i].active) continue;
        explosions[i].elapsedTime += dt;
        if (explosions[i].elapsedTime >= explosions[i].duration) {
            explosions[i].active = false;
        }
    }

    // --- Update Cooldowns ---
    for (int i = 0; i < 5; i++) {
        if (cooldownTimer[i] < 999) cooldownTimer[i] += dt;
    }

    // --- Update HI ---
    if (currentLevel == 4) {
        if (level4Score > level4HighScore) {
            level4HighScore = level4Score;
            SaveLevel4HighScore();
        }
    }

    // --- Input: Planting ---
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();

        // Check Sun Click
        for (int i = 0; i < MAX_SUNS; i++){
            if (!suns[i].active) continue;
            if (CheckCollisionPointCircle(mouse, (Vector2){suns[i].x, suns[i].y}, 61)) {
                if (suns[i].isSun) {
                    sunCurrency += suns[i].value;
                    PlaySound(sunSound);
                } else {
                    Coins += suns[i].value;
                    SaveCoinBalance();
                    PlaySound(coinSound);
                }
                suns[i].active = false;
                return; // Prioritize sun collection over planting if both clicked
            }
        }
        
        // Grid Click
        if (mouse.x > GRID_OFFSET_X && mouse.x < GRID_OFFSET_X + GRID_COLS * CELL_WIDTH &&
            mouse.y > GRID_OFFSET_Y && mouse.y < GRID_OFFSET_Y + GRID_ROWS * CELL_HEIGHT) {
            
            int c = (mouse.x - GRID_OFFSET_X) / CELL_WIDTH;
            int r = (mouse.y - GRID_OFFSET_Y) / CELL_HEIGHT;

            // Prevent Sunflowers in Stage 3 and unlocked potatomine
            if ((currentLevel == 2 && selectedPlant == P_SUNFLOWER) || (!potatoMineUnlocked && selectedPlant == P_POTATO)) {
                // Do nothing
            } else {
                bool spotTaken = false;
                for(int i=0; i<MAX_PLANTS; i++) {
                    if(plants[i].active && plants[i].col == c && plants[i].row == r) spotTaken = true;
                }

                if (!spotTaken) {
                    int cost = 0;
                    float cooldown = 0;
                    if(selectedPlant == P_SUNFLOWER) {cost = COST_SUNFLOWER; cooldown = COOLDOWN_SUNFLOWER;}
                    if(selectedPlant == P_PEASHOOTER) {cost = COST_PEASHOOTER; cooldown = COOLDOWN_PEASHOOTER;}
                    if(selectedPlant == P_CHOMPER) {cost = COST_CHOMPER; cooldown = COOLDOWN_CHOMPER;}
                    if(selectedPlant == P_ROSE) {cost = COST_ROSE; cooldown = COOLDOWN_ROSE;}
                    if(selectedPlant == P_POTATO) {cost = COST_POTATO; cooldown = COOLDOWN_POTATO;}

                    if (sunCurrency >= cost && cooldownTimer[selectedPlant] >= cooldown) {
                        sunCurrency -= cost;
                        PlaySound(plantsound);
                        cooldownTimer[selectedPlant] = 0; // Reset cooldown for this plant

                        if (selectedPlant == P_CHOMPER) {
                            bool zombieInRow = false;
                            for(int z=0; z<MAX_ZOMBIES; z++) {
                                if (zombies[z].active && zombies[z].row == r) zombieInRow = true;
                            }
                            if (zombieInRow) {
                                PlaySound(frozenSounds[frozenSoundIdx]);
                                frozenSoundIdx++;
                                if (frozenSoundIdx >= 5) frozenSoundIdx = 0;
                            }
                        }

                        if (selectedPlant == P_ROSE) {
                            bool hasPlantsInRow = false;
                            for(int j=0; j<MAX_PLANTS; j++) {
                                if(plants[j].active && plants[j].row == r) {
                                    hasPlantsInRow = true; break;
                                }
                            }
                            if (hasPlantsInRow) {
                                PlaySound(chimeSounds[chimeSoundIdx]);
                                chimeSoundIdx++;
                                if (chimeSoundIdx >= 5) chimeSoundIdx = 0;
                            }
                        }
                        
                        for(int i=0; i<MAX_PLANTS; i++) {
                            if(!plants[i].active) {
                                plants[i].active = true;
                                plants[i].type = selectedPlant;
                                plants[i].col = c;
                                plants[i].row = r;
                                plants[i].maxHp = 100;
                                if(selectedPlant == P_POTATO) plants[i].maxHp = 50; 
                                plants[i].hp = plants[i].maxHp;
                                plants[i].timer = 0;
                                plants[i].isArmed = false;
                                
                                if(selectedPlant == P_CHOMPER) plants[i].lifeSpan = 20.0f;
                                if(selectedPlant == P_ROSE) plants[i].lifeSpan = 10.0f;
                                
                                if (plants[i].type != P_SUNFLOWER && plants[i].type != P_ROSE) RecalculateRowWeight(r);
                                break;
                            }
                        }
                    } else PlaySound(buzzerSound);
                }
            }
        }
    }
}
