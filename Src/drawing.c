#include "drawing.h"
#include "entities.h"
#include "helpers.h"
#include "game.h"
#include "raylib.h"
#include <stddef.h>

void DrawGameScreen() {
    // Draw Background
    if (currentLevel == 2) {
        DrawTexture(frontyardnightTexture, 0, 0, WHITE);
    } else {
        DrawTexture(frontyardTexture, 0, 0, WHITE);
    }
    if (currentLevel == 4) {
        DrawTexturePro(statboardTexture,
            (Rectangle){0, 0, (float)statboardTexture.width, (float)statboardTexture.height},
            (Rectangle){2100, -100, (float)statboardTexture.width/2, (float)statboardTexture.height/2},
            (Vector2){0, 0}, 0.0f, WHITE);
        DrawTextEx(markerTtf, TextFormat("$%d", Coins), (Vector2){2320, 90}, 60, 2, DARKGREEN);
        DrawTextEx(markerTtf, TextFormat("HI: %d", level4HighScore), (Vector2){2250, 225}, 60, 2, BLACK);
        DrawTextEx(markerTtf, TextFormat("Score: %d", level4Score), (Vector2){2250, 360}, 60, 2, BLACK);
    } else{
        DrawTexturePro(coinboardTexture,
        (Rectangle){0, 0, (float)statboardTexture.width, (float)statboardTexture.height},
        (Rectangle){2100, -150, (float)statboardTexture.width/2, (float)statboardTexture.height/2},
        (Vector2){0, 0}, 0.0f, WHITE);
        DrawTextEx(markerTtf, TextFormat("$%d", Coins), (Vector2){2320, 90}, 60, 2, DARKGREEN);
    }

    // --- HUD ---
    DrawTexture(dashboardTexture, 50, 50, WHITE);
    DrawTextEx(markerTtf, TextFormat("%d", sunCurrency), (Vector2){105 + 50, 220 + 50}, 60, 2, BLACK);

    // Plant Selection Buttons
    const Texture2D* seedTextures[] = {&SEEDsunflowerTexture, &SEEDpeashooterTexture, &SEEDchomperTexture, &SEEDroseTexture, &SEEDpotatoTexture};
    int costs[] = {COST_SUNFLOWER, COST_PEASHOOTER, COST_CHOMPER, COST_ROSE, COST_POTATO};
    float cooldowns[] = {COOLDOWN_SUNFLOWER, COOLDOWN_PEASHOOTER, COOLDOWN_CHOMPER, COOLDOWN_ROSE, COOLDOWN_POTATO};
    PlantType types[] = {P_SUNFLOWER, P_PEASHOOTER, P_CHOMPER, P_ROSE, P_POTATO};

    for(int i=0; i<5; i++) {
        int bx = 322 + 50 + i * (27 + 189);
        int by = 25 + 50;
        int bw = 189;
        int bh = 263;
        
        Color btnColor = WHITE;
        
        // Special Case: Disable Sunflower visual in Stage 3
        if((currentLevel == 2 && types[i] == P_SUNFLOWER) || (types[i] == P_POTATO && !potatoMineUnlocked)) {
            btnColor = DARKGRAY;
        } else if(sunCurrency < costs[i] || cooldownTimer[types[i]] < cooldowns[i]) {
            btnColor = GRAY;
        }


        DrawTexturePro(*seedTextures[i],
                      (Rectangle){0, 0, (float)seedTextures[i]->width, (float)seedTextures[i]->height},
                      (Rectangle){bx, by, bw + 5, bh + 5},
                      (Vector2){0, 0}, 0.0f, btnColor);
        
        if((currentLevel == 2 && types[i] == P_SUNFLOWER) || (types[i] == P_POTATO && !potatoMineUnlocked)) {
            DrawTexturePro(lockTexture,
                          (Rectangle){0, 0, (float)lockTexture.width, (float)lockTexture.height},
                          (Rectangle){bx - 20, by - 20, bw + 50, bh + 50},
                          (Vector2){0, 0}, 0.0f, WHITE);
        }
        
        if (cooldownTimer[types[i]] < cooldowns[i]) {
            DrawRectangle(bx, by, bw + 5, (bh - 3)*(1 - cooldownTimer[types[i]] / cooldowns[i]), (Color){0, 0, 0, 150});
        }

        if (selectedPlant == types[i]){
            DrawTexturePro(packetframeTexture,
                          (Rectangle){0, 0, (float)packetframeTexture.width, (float)packetframeTexture.height},
                          (Rectangle){bx - 20, by - 20, bw + 40, bh + 40},
                          (Vector2){0, 0}, 0.0f, WHITE);
        }

    }

    // Keyboard navigation for plant selection
    if (notPaused) {
        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
            PlaySound(seedliftSound);
            int currentIndex = selectedPlant;
            currentIndex++;
            if (currentIndex >= 5) currentIndex = 0;
            // Skip Sunflower if Stage 3
            if (!potatoMineUnlocked && types[currentIndex] == P_POTATO) {
                currentIndex++;
                if (currentIndex >= 5) currentIndex = 0;
            }
            if (currentLevel == 2 && types[currentIndex] == P_SUNFLOWER) {
                currentIndex++;
                if (currentIndex >= 5) currentIndex = 0;
            }
            selectedPlant = types[currentIndex];
        }
        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
            PlaySound(seedliftSound);
            int currentIndex = selectedPlant;
            currentIndex--;
            if (currentIndex < 0) currentIndex = 4;
            // Skip Sunflower if Stage 3
            if (currentLevel == 2 && types[currentIndex] == P_SUNFLOWER) {
                currentIndex--;
                if (currentIndex < 0) currentIndex = 4;
            }
            if (!potatoMineUnlocked && types[currentIndex] == P_POTATO) {
                currentIndex--;
                if (currentIndex < 0) currentIndex = 4;
            }
            selectedPlant = types[currentIndex];
        }
    }

    // Draw Mowers
    for(int i=0; i<GRID_ROWS; i++) {
        if(mowers[i].active) {
            float mowerY = GRID_OFFSET_Y + mowers[i].row * CELL_HEIGHT + 46;
            DrawTexturePro(mowerTexture,
                          (Rectangle){0, 0, (float)mowerTexture.width, (float)mowerTexture.height},
                          (Rectangle){mowers[i].x - 150/2, mowerY - 150/2, 150, 150},
                          (Vector2){0, 0}, 0.0f, WHITE);
        }
    }

    // Draw Plants
    for (int i = 0; i < MAX_PLANTS; i++) {
        if (!plants[i].active) continue;
        int px = GRID_OFFSET_X + plants[i].col * CELL_WIDTH + 20;
        int py = GRID_OFFSET_Y + plants[i].row * CELL_HEIGHT - 10;
        
        int plantW = 150;
        int plantH = 150;
        Texture2D* plantTexture = NULL;

        switch(plants[i].type) {
            case P_SUNFLOWER:
                plantTexture = &sunflowerAnimTexture;
                break;
            case P_PEASHOOTER:
                plantTexture = &peashooterAnimTexture;
                break;
            case P_CHOMPER:
                plantTexture = &chomperAnimTexture;
                break;
            case P_ROSE:
                plantTexture = &roseAnimTexture;
                break;
            case P_POTATO:
                plantTexture = &potatoAnimTexture;
                break;
        }

        if (plantTexture != NULL && plantTexture->id != 0) {
            if (plants[i].type == P_POTATO && plants[i].isArmed) {
                DrawTexturePro(*plantTexture,
                               (Rectangle){0, 0, (float)plantTexture->width, (float)plantTexture->height},
                               (Rectangle){px, py, plantW, plantH},
                               (Vector2){0, 0}, 0.0f, RED);
            } else {
                DrawTexturePro(*plantTexture,
                               (Rectangle){0, 0, (float)plantTexture->width, (float)plantTexture->height},
                               (Rectangle){px, py, plantW, plantH},
                               (Vector2){0, 0}, 0.0f, (RowHasPlant(plants[i].row, P_ROSE) && plants[i].type != P_ROSE) ? PINK : WHITE);
            }
        }

        DrawRectangle(px, py - 11, 154 * (plants[i].hp / plants[i].maxHp), 11, RED);
    }

    // Draw Zombies
    for (int i = 0; i < MAX_ZOMBIES; i++) {
        if (!zombies[i].active) continue;
        int zx = (int)zombies[i].x;
        int zy = (int)zombies[i].y;

        int zombieW = 300;
        int zombieH = 300;
        Texture2D* zombieTexture = NULL;

        switch(zombies[i].type) {
            case Z_NORMAL:
                if (zombies[i].isEating)
                {
                    zombieTexture = &EATnormalzAnimTexture;
                }
                else
                {
                    zombieTexture = &normalzAnimTexture;
                }
                break;
            case Z_THINKER:
                if (zombies[i].isEating)
                {
                    zombieTexture = &EATthinkerzAnimTexture;
                }
                else
                {
                    zombieTexture = &thinkerzAnimTexture;
                }
                break;
        }

        if (zombieTexture != NULL && zombieTexture->id != 0) {
            if (RowHasPlant(zombies[i].row, P_CHOMPER)) {
                DrawTexturePro(*zombieTexture,
                               (Rectangle){0, 0, (float)zombieTexture->width, (float)zombieTexture->height},
                               (Rectangle){zx, zy - 150, zombieW, zombieH},
                               (Vector2){0, 0}, 0.0f, BLUE);
            } else {
                DrawTexturePro(*zombieTexture,
                            (Rectangle){0, 0, (float)zombieTexture->width, (float)zombieTexture->height},
                            (Rectangle){zx, zy - 150, zombieW, zombieH},
                            (Vector2){0, 0}, 0.0f, WHITE);
            }
        }

        DrawRectangle(zx, zy - 23, 128 * (zombies[i].hp / zombies[i].maxHp), 11, RED);
    }

    // Draw Projectiles
    for(int i=0; i<MAX_PROJECTILES; i++) {
        if(projectiles[i].active) {
            float projY = GRID_OFFSET_Y + projectiles[i].row * CELL_HEIGHT + CELL_HEIGHT/2;
            int peaSize = 50;  // Size of pea projectile
            DrawTexturePro(peaTexture,
                          (Rectangle){0, 0, (float)peaTexture.width, (float)peaTexture.height},
                          (Rectangle){projectiles[i].x + 30, projY  - 90, peaSize, peaSize},
                          (Vector2){0, 0}, 0.0f, WHITE);
        }
    }

    // Draw Sun
    for(int i=0; i<MAX_SUNS; i++) {
        if(suns[i].active) {
            if (suns[i].isSun) {
                DrawTexturePro(sunTexture,
                          (Rectangle){0, 0, (float)sunTexture.width, (float)sunTexture.height},
                          (Rectangle){suns[i].x - 100/2, suns[i].y - 100/2, 100, 100},
                          (Vector2){0, 0}, 0.0f, WHITE);
            } else {
                DrawTexturePro(coinbagTexture,
                            (Rectangle){0, 0, (float)coinbagTexture.width, (float)coinbagTexture.height},
                            (Rectangle){suns[i].x - 100/2, suns[i].y - 100/2, 100, 100},
                            (Vector2){0, 0}, 0.0f, WHITE);
            }
        }
    }

    // Draw Explosions
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        if (!explosions[i].active) continue;

        // 1. Calculate which frame this specific explosion should be on
        if (explosions[i].duration > 0) {
            float lifePercent = explosions[i].elapsedTime / explosions[i].duration;
            int frameToDraw = (int)(lifePercent * explosionAnimFrames);

            // Safety clamp
            if (frameToDraw >= explosionAnimFrames) frameToDraw = explosionAnimFrames - 1;
            if (frameToDraw < 0) frameToDraw = 0;

            // 2. Update the texture to this specific frame
            // Note: UpdateTexture is somewhat expensive. If you have 50 explosions, 
            // this might slow down. For a few explosions, it is fine.
            unsigned int nextFrameDataOffset = explosionAnimImage.width * explosionAnimImage.height * 4 * frameToDraw;
            UpdateTexture(explosionAnimTexture, ((unsigned char*)explosionAnimImage.data) + nextFrameDataOffset);
            
            // 3. Draw the texture
            DrawTexturePro(explosionAnimTexture,
                          (Rectangle){0, 0, (float)explosionAnimTexture.width, (float)explosionAnimTexture.height},
                          (Rectangle){explosions[i].x - 330, explosions[i].y - 280, 800, 400},
                          (Vector2){0, 0}, 0.0f, WHITE);
        }
    }

    // Draw Pause Screen
    if (!notPaused) {
        DrawTexture(pausescreenTexture, (SCREEN_WIDTH - pausescreenTexture.width)/2, 200, WHITE);
        Vector2 mouse = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mouse, (Rectangle){(SCREEN_WIDTH - pausescreenTexture.width)/2 + 100, 200 + 800, 366, 116})) {
                PlaySound(tapSound);
                StopMusicStream(bgMusic);
                PlayMusicStream(bgMusic);
                InitGame(currentLevel);
            } else if (CheckCollisionPointRec(mouse, (Rectangle){(SCREEN_WIDTH - pausescreenTexture.width)/2 + 500, 200 + 800, 325, 116})) {
                PlaySound(tapSound);
                currentScreen = MENU;
            }
        }
    }
}
