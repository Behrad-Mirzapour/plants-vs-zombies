#include "raylib.h"
#include "stdio.h"
#include "stdlib.h"

#include "defs.h"
#include "entities.h"
#include "helpers.h"
#include "data.h"
#include "game.h"
#include "drawing.h"

// -------------------------------------------------------------------------
// GLOBAL STATE DEFINITIONS
// -------------------------------------------------------------------------
GameScreen currentScreen = MENU;
bool notPaused = true;
int sunCurrency = 150;
float gameTime = 0;
float sunSpawnTimer = 0;
float zombieSpawnTimer = 0;
float cooldownTimer[5] = {999, 999, 999, 999, 999};
bool potatoMineUnlocked = false;

// Level 4 Scoring
int level4Score = 0;
int level4HighScore = 0;

// Coins
int Coins = 0;

// Entities
Plant plants[MAX_PLANTS];
Zombie zombies[MAX_ZOMBIES];
Projectile projectiles[MAX_PROJECTILES];
Sun suns[MAX_SUNS];
Explosion explosions[MAX_EXPLOSIONS];
Lawnmower mowers[GRID_ROWS];
RowWeight weights[GRID_ROWS];

// Fonts
Font markerTtf;

// Textures
Texture2D peaTexture;
Texture2D sunTexture;
Texture2D mowerTexture;
Texture2D frontyardTexture;
Texture2D frontyardnightTexture;
Texture2D SEEDsunflowerTexture;
Texture2D SEEDpeashooterTexture;
Texture2D SEEDchomperTexture;
Texture2D SEEDroseTexture;
Texture2D SEEDpotatoTexture;
Texture2D dashboardTexture;
Texture2D packetframeTexture;
Texture2D lockTexture;
Texture2D levelselectionTexture;
Texture2D pausescreenTexture;
Texture2D mainmenu1Texture;
Texture2D mainmenu2Texture;
Texture2D mainmenu3Texture;
Texture2D shopscreenTexture;
Texture2D coinbagTexture;
Texture2D soldoutTexture;
Texture2D coinboardTexture;
Texture2D statboardTexture;

// Animated GIF structures for each plant type
Image sunflowerAnimImage;
Texture2D sunflowerAnimTexture;
int sunflowerAnimFrames = 0;
int sunflowerAnimCurrentFrame = 0;
int sunflowerAnimFrameDelay = 2;
int sunflowerAnimFrameCounter = 0;
int sunflowerAnimFPS = 60;

Image peashooterAnimImage;
Texture2D peashooterAnimTexture;
int peashooterAnimFrames = 0;
int peashooterAnimCurrentFrame = 0;
int peashooterAnimFrameDelay = 2;
int peashooterAnimFrameCounter = 0;
int peashooterAnimFPS = 60;

Image chomperAnimImage;
Texture2D chomperAnimTexture;
int chomperAnimFrames = 0;
int chomperAnimCurrentFrame = 0;
int chomperAnimFrameDelay = 6;
int chomperAnimFrameCounter = 0;
int chomperAnimFPS = 60;

Image roseAnimImage;
Texture2D roseAnimTexture;
int roseAnimFrames = 0;
int roseAnimCurrentFrame = 0;
int roseAnimFrameDelay = 2;
int roseAnimFrameCounter = 0;
int roseAnimFPS = 60;

Image potatoAnimImage;
Texture2D potatoAnimTexture;
int potatoAnimFrames = 0;
int potatoAnimCurrentFrame = 0;
int potatoAnimFrameDelay = 2;
int potatoAnimFrameCounter = 0;
int potatoAnimFPS = 60;

Image normalzAnimImage;
Texture2D normalzAnimTexture;
int normalzAnimFrames = 0;
int normalzAnimCurrentFrame = 0;
int normalzAnimFrameDelay = 5;
int normalzAnimFrameCounter = 0;
int normalzAnimFPS = 60;

Image thinkerzAnimImage;
Texture2D thinkerzAnimTexture;
int thinkerzAnimFrames = 0;
int thinkerzAnimCurrentFrame = 0;
int thinkerzAnimFrameDelay = 6;
int thinkerzAnimFrameCounter = 0;
int thinkerzAnimFPS = 60;

Image EATnormalzAnimImage;
Texture2D EATnormalzAnimTexture;
int EATnormalzAnimFrames = 0;
int EATnormalzAnimCurrentFrame = 0;
int EATnormalzAnimFrameDelay = 5;
int EATnormalzAnimFrameCounter = 0;
int EATnormalzAnimFPS = 60;

Image EATthinkerzAnimImage;
Texture2D EATthinkerzAnimTexture;
int EATthinkerzAnimFrames = 0;
int EATthinkerzAnimCurrentFrame = 0;
int EATthinkerzAnimFrameDelay = 6;
int EATthinkerzAnimFrameCounter = 0;
int EATthinkerzAnimFPS = 60;

Image explosionAnimImage;
Texture2D explosionAnimTexture;
int explosionAnimFrames = 0;
int explosionAnimFPS = 40;

// UI Selection
PlantType selectedPlant = P_PEASHOOTER;
int currentLevel = 1;

// Level Tracking
int zombiesSpawned = 0;
int zombiesToSpawnNormal = 0;
int zombiesToSpawnThinker = 0;
int totalLevelZombies = 0;

// -------------------------------------------------------------------------
// AUDIO GLOBALS 
// -------------------------------------------------------------------------
Music bgMusic;
int currentMusicLevelID = -1;

Sound sunSound;
Sound pauseSound;
Sound plantsound;
Sound buzzerSound;
Sound loseSound;
Sound winSound;
Sound seedliftSound;
Sound throwSound;
Sound tapSound;
Sound gravebuttonSound;
Sound coinSound;

Sound groanSounds[6];

// Arrays for concurrent sounds (Aliases)
Sound explosionSounds[MAX_SFX_INSTANCES];
int explosionSoundIdx = 0;

Sound splatSounds[MAX_SFX_INSTANCES];
int splatSoundIdx = 0;

Sound mowerSounds[5];
int mowerSoundIdx = 0;

Sound frozenSounds[5];
int frozenSoundIdx = 0;

Sound chimeSounds[5];
int chimeSoundIdx = 0;

Sound chompSounds[MAX_SFX_INSTANCES];
int chompSoundIdx = 0;



// -------------------------------------------------------------------------
// MAIN LOOP
// -------------------------------------------------------------------------

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Plants vs Zombies Clone - Raylib C");
    SetTargetFPS(60);

    // Load Fonts
    markerTtf = LoadFontEx("../Resources/PermanentMarker.ttf", 32, 0, 250);

    // Load Textures
    peaTexture = LoadTexture("../Resources/Pea.png");
    sunTexture = LoadTexture("../Resources/Sun.png");
    mowerTexture = LoadTexture("../Resources/Mower.png");
    frontyardTexture = LoadTexture("../Resources/Frontyard.png");
    frontyardnightTexture = LoadTexture("../Resources/FrontyardNight.png");
    SEEDsunflowerTexture = LoadTexture("../Resources/SeedSunflower.png");
    SEEDpeashooterTexture = LoadTexture("../Resources/SeedPeashooter.png");
    SEEDchomperTexture = LoadTexture("../Resources/SeedChomper.png");
    SEEDroseTexture = LoadTexture("../Resources/SeedRose.png");
    SEEDpotatoTexture = LoadTexture("../Resources/SeedPotato.png");
    dashboardTexture = LoadTexture("../Resources/Dashboard.png");
    packetframeTexture = LoadTexture("../Resources/PacketFrame.png");
    lockTexture = LoadTexture("../Resources/Lock.png");
    levelselectionTexture = LoadTexture("../Resources/Levelselection.png");
    pausescreenTexture = LoadTexture("../Resources/Pausescreen.png");
    mainmenu1Texture = LoadTexture("../Resources/Mainmenu1.png");
    mainmenu2Texture = LoadTexture("../Resources/Mainmenu2.png");
    mainmenu3Texture = LoadTexture("../Resources/Mainmenu3.png");
    shopscreenTexture = LoadTexture("../Resources/Shopscreen.png");
    coinbagTexture = LoadTexture("../Resources/Coinbag.png");
    soldoutTexture = LoadTexture("../Resources/Soldout.png");
    coinboardTexture = LoadTexture("../Resources/Coinboard.png");
    statboardTexture = LoadTexture("../Resources/Statboard.png");


    // Load animated GIF frames for all plant types
    
    LOAD_GIF(sunflower, "../Resources/Sunflower.gif");
    LOAD_GIF(peashooter, "../Resources/Peashooter.gif");
    LOAD_GIF(chomper, "../Resources/Chomper.gif");
    LOAD_GIF(rose, "../Resources/Rose.gif");
    LOAD_GIF(potato, "../Resources/Potato.gif");
    LOAD_GIF(normalz, "../Resources/Normalz.gif");
    LOAD_GIF(thinkerz, "../Resources/Thinkerz.gif");
    LOAD_GIF(EATnormalz, "../Resources/EATNormalz.gif");
    LOAD_GIF(EATthinkerz, "../Resources/EATThinkerz.gif");
    LOAD_GIF(explosion, "../Resources/explosion.gif");

    InitAudioDevice(); // Initialize audio system

    // -- Load Saved Data ---
    LoadCoinBalance();
    LoadPotatoMineUnlock();
    LoadLevel4HighScore();

    // --- LOAD SOUNDS & MUSIC ---
    sunSound = LoadSound("../Resources/sound&music/points.mp3");
    pauseSound = LoadSound("../Resources/sound&music/pause.mp3");
    plantsound = LoadSound("../Resources/sound&music/plant.mp3");
    buzzerSound = LoadSound("../Resources/sound&music/buzzer.mp3");
    loseSound = LoadSound("../Resources/sound&music/losemusic.mp3");
    winSound = LoadSound("../Resources/sound&music/winmusic.mp3");
    seedliftSound = LoadSound("../Resources/sound&music/seedlift.mp3");
    throwSound = LoadSound("../Resources/sound&music/throw.mp3");
    tapSound = LoadSound("../Resources/sound&music/tap.mp3");
    gravebuttonSound = LoadSound("../Resources/sound&music/gravebutton.mp3");
    coinSound = LoadSound("../Resources/sound&music/coin.mp3");
    groanSounds[0] = LoadSound("../Resources/sound&music/groan.mp3");
    groanSounds[1] = LoadSound("../Resources/sound&music/groan2.mp3");
    groanSounds[2] = LoadSound("../Resources/sound&music/groan3.mp3");
    groanSounds[3] = LoadSound("../Resources/sound&music/groan4.mp3");
    groanSounds[4] = LoadSound("../Resources/sound&music/groan5.mp3");
    groanSounds[5] = LoadSound("../Resources/sound&music/groan6.mp3");

    // Load Aliases for concurrent SFX
    Sound baseExplosion = LoadSound("../Resources/sound&music/potato-mine.mp3");
    for (int i=0; i<MAX_SFX_INSTANCES; i++) explosionSounds[i] = LoadSoundAlias(baseExplosion);

    Sound baseSplat = LoadSound("../Resources/sound&music/splat2.mp3");
    for (int i=0; i<MAX_SFX_INSTANCES; i++) splatSounds[i] = LoadSoundAlias(baseSplat);

    Sound baseMower = LoadSound("../Resources/sound&music/lawnmower.mp3");
    for (int i=0; i<5; i++) mowerSounds[i] = LoadSoundAlias(baseMower);

    Sound baseFrozen = LoadSound("../Resources/sound&music/frozen.mp3");
    for (int i=0; i<5; i++) frozenSounds[i] = LoadSoundAlias(baseFrozen);

    Sound baseChime = LoadSound("../Resources/sound&music/chime.mp3");
    for (int i=0; i<5; i++) chimeSounds[i] = LoadSoundAlias(baseChime);

    Sound baseChomp = LoadSound("../Resources/sound&music/chomp.mp3");
    for (int i=0; i<MAX_SFX_INSTANCES; i++) chompSounds[i] = LoadSoundAlias(baseChomp);

    // Initial Music Load
    bgMusic = LoadMusicStream("../Resources/sound&music/Plants vs. Zombies - Choose Your Seeds.mp3");
    PlayMusicStream(bgMusic);

    int menuSelection = 0; // 0: Play, 1: Shop, 2: Exit
    int levelSelection = 1;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        UpdateMusicSystem();

        // ---------------- UPDATE ----------------
        switch(currentScreen) {
            case MENU:
                if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {menuSelection++; PlaySound(gravebuttonSound);}
                if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {menuSelection--; PlaySound(gravebuttonSound);}
                if (menuSelection > 2) menuSelection = 0;
                if (menuSelection < 0) menuSelection = 2;

                if (IsKeyPressed(KEY_ENTER)) {
                    PlaySound(tapSound);
                    if (menuSelection == 0) currentScreen = LEVEL_SELECT;
                    else if (menuSelection == 1) currentScreen = SHOP;
                }
                break;
            
            case SHOP:
                Vector2 mouse = GetMousePosition();
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (CheckCollisionPointRec(mouse, (Rectangle){1030, 1350, 500, 100})) {
                        if (Coins >= 500 && !potatoMineUnlocked) {
                            Coins -= 500;
                            potatoMineUnlocked = true;
                            SaveCoinBalance();
                            SavePotatoMineUnlock();
                            PlaySound(coinSound);
                        } else {
                            PlaySound(buzzerSound);
                        }
                    }
                }
                if (IsKeyPressed(KEY_Q)) currentScreen = MENU;
                break;

            case LEVEL_SELECT:
                if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {levelSelection++; PlaySound(gravebuttonSound);}
                if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {levelSelection--; PlaySound(gravebuttonSound);}
                if (levelSelection > 4) levelSelection = 1;
                if (levelSelection < 1) levelSelection = 4;

                if (IsKeyPressed(KEY_ENTER)) {
                    PlaySound(tapSound);
                    InitGame(levelSelection);
                    currentScreen = GAME;
                }

                if (IsKeyPressed(KEY_Q)) currentScreen = MENU;
                break;

                case GAME:
                    if (notPaused) UpdateGame(dt);
                    if (IsKeyPressed(KEY_SPACE)){
                        notPaused = !notPaused;
                        PlaySound(pauseSound);
                    }
                break;

            case GAME_OVER:
            case WIN:
                if (IsKeyPressed(KEY_Q)) currentScreen = LEVEL_SELECT;
                break;
        }

        // Update animation frames for all plant types
        
        if (notPaused){
            UPDATE_ANIMATION(sunflower);
            UPDATE_ANIMATION(peashooter);
            UPDATE_ANIMATION(chomper);
            UPDATE_ANIMATION(rose);
            UPDATE_ANIMATION(potato);
            UPDATE_ANIMATION(normalz);
            UPDATE_ANIMATION(thinkerz);
            UPDATE_ANIMATION(EATnormalz);
            UPDATE_ANIMATION(EATthinkerz);
        }

        // ---------------- DRAW ----------------
        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch(currentScreen) {
            case MENU:
                switch (menuSelection)
                {
                    case 0:
                        SetExitKey(0);
                        DrawTexture(mainmenu1Texture, 0, 0, WHITE);
                        break;
                    
                    case 1:
                        SetExitKey(0);
                        DrawTexture(mainmenu2Texture, 0, 0, WHITE);
                        break;
                    
                    case 2:
                        SetExitKey(KEY_ENTER);
                        DrawTexture(mainmenu3Texture, 0, 0, WHITE);
                        break;
                }
                break;

            case SHOP:
                UPDATE_ANIMATION(potato);
                    DrawTexture(shopscreenTexture, 0, 0, WHITE);
                    DrawTexturePro(coinboardTexture,
                    (Rectangle){0, 0, (float)statboardTexture.width, (float)statboardTexture.height},
                    (Rectangle){2080, -150, (float)statboardTexture.width/2, (float)statboardTexture.height/2},
                    (Vector2){0, 0}, 0.0f, WHITE);
                    DrawTextEx(markerTtf, TextFormat("$%d", Coins), (Vector2){2300, 90}, 60, 2, DARKGREEN);
                    DrawTextEx(markerTtf, TextFormat("$500"), (Vector2){1000, 300}, 80, 2, BLACK);
                    DrawTexturePro(potatoAnimTexture,
                                (Rectangle){0, 0, (float)potatoAnimTexture.width, (float)potatoAnimTexture.height},
                                (Rectangle){1180, 400, 200, 200},
                                (Vector2){0, 0}, 0.0f, WHITE);
                 if (potatoMineUnlocked){
                    DrawRectangle(1030, 1350, 500, 100, ColorAlpha(BLACK, 0.4));
                 } else{
                     Vector2 mouse = GetMousePosition();
                    if (CheckCollisionPointRec(mouse, (Rectangle){1030, 1350, 500, 100})) {
                        if (Coins >= 500) {
                            DrawRectangle(1030, 1350, 500, 100, ColorAlpha(GREEN, 0.3));
                        } else {
                            DrawRectangle(1030, 1350, 500, 100, ColorAlpha(RED, 0.3));
                        }
                    }
                 }
                
                break;

            case LEVEL_SELECT:
                DrawTexture(levelselectionTexture, 0, 0, WHITE);
                DrawTexturePro(packetframeTexture,
                              (Rectangle){0, 0, (float)packetframeTexture.width, (float)packetframeTexture.height},
                              (Rectangle){180 + (levelSelection -1) * (568), 110, 500, 560},
                              (Vector2){0, 0}, 0.0f, WHITE);
                
                DrawText("Use Arrows to Select, ENTER to Start", 640, 1143, 46, DARKGRAY);
                break;

            case GAME:
                DrawGameScreen();
                break;

            case GAME_OVER:
                DrawText("YOU WIN!", 896, 686, 137, GOLD);
                DrawText("The lawn is safe... for now.", 768, 914, 69, DARKGRAY);
                DrawText("Press Q", 896, 1143, 46, GRAY);

                break;

            case WIN:
                DrawText("YOU WIN!", 896, 686, 137, GOLD);
                DrawText("The lawn is safe... for now.", 768, 914, 69, DARKGRAY);
                DrawText("Press Q", 896, 1143, 46, GRAY);
                
                break;
        }

        EndDrawing();
    }

    // Unload resources
    UnloadTexture(peaTexture);
    UnloadTexture(sunTexture);
    UnloadTexture(mowerTexture);
    UnloadTexture(sunflowerAnimTexture);
    if (sunflowerAnimFrames > 0) UnloadImage(sunflowerAnimImage);
    UnloadTexture(peashooterAnimTexture);
    if (peashooterAnimFrames > 0) UnloadImage(peashooterAnimImage);
    UnloadTexture(chomperAnimTexture);
    if (chomperAnimFrames > 0) UnloadImage(chomperAnimImage);
    UnloadTexture(roseAnimTexture);
    if (roseAnimFrames > 0) UnloadImage(roseAnimImage);
    UnloadTexture(potatoAnimTexture);
    if (potatoAnimFrames > 0) UnloadImage(potatoAnimImage);
    UnloadTexture(normalzAnimTexture);
    if (normalzAnimFrames > 0) UnloadImage(normalzAnimImage);
    UnloadTexture(thinkerzAnimTexture);
    if (thinkerzAnimFrames > 0) UnloadImage(thinkerzAnimImage);
    UnloadTexture(EATnormalzAnimTexture);
    if (EATnormalzAnimFrames > 0) UnloadImage(EATnormalzAnimImage);
    UnloadTexture(EATthinkerzAnimTexture);
    if (EATthinkerzAnimFrames > 0) UnloadImage(EATthinkerzAnimImage);
    UnloadTexture(explosionAnimTexture);
    if (explosionAnimFrames > 0) UnloadImage(explosionAnimImage);
    UnloadTexture(frontyardTexture);
    UnloadTexture(frontyardnightTexture);
    UnloadTexture(SEEDsunflowerTexture);
    UnloadTexture(SEEDpeashooterTexture);
    UnloadTexture(SEEDchomperTexture);
    UnloadTexture(SEEDroseTexture);
    UnloadTexture(SEEDpotatoTexture);
    UnloadTexture(dashboardTexture);
    UnloadTexture(packetframeTexture);
    UnloadTexture(lockTexture);
    UnloadTexture(levelselectionTexture);
    UnloadTexture(pausescreenTexture);
    UnloadTexture(mainmenu1Texture);
    UnloadTexture(mainmenu2Texture);
    UnloadTexture(mainmenu3Texture);
    UnloadTexture(shopscreenTexture);
    UnloadTexture(coinbagTexture);
    UnloadTexture(soldoutTexture);
    UnloadTexture(coinboardTexture);
    UnloadTexture(statboardTexture);
    UnloadFont(markerTtf);
    
    // Unload sounds
    UnloadSound(sunSound);
    UnloadSound(pauseSound);
    UnloadSound(plantsound);
    UnloadSound(buzzerSound);
    UnloadSound(loseSound);
    UnloadSound(winSound);
    UnloadSound(seedliftSound);
    UnloadSound(throwSound);
    UnloadSound(tapSound);
    UnloadSound(gravebuttonSound);
    UnloadSound(coinSound);
    
    for (int i = 0; i < 6; i++) UnloadSound(groanSounds[i]);
    for (int i = 1; i < MAX_SFX_INSTANCES; i++) UnloadSoundAlias(explosionSounds[i]);
    for (int i = 1; i < MAX_SFX_INSTANCES; i++) UnloadSoundAlias(splatSounds[i]);
    for (int i = 1; i < 5; i++) UnloadSoundAlias(mowerSounds[i]);
    for (int i = 1; i < 5; i++) UnloadSoundAlias(frozenSounds[i]);
    for (int i = 1; i < 5; i++) UnloadSoundAlias(chimeSounds[i]);
    for (int i = 1; i < MAX_SFX_INSTANCES; i++) UnloadSoundAlias(chompSounds[i]);

    UnloadSound(explosionSounds[0]);
    UnloadSound(splatSounds[0]);
    UnloadSound(mowerSounds[0]);
    UnloadSound(frozenSounds[0]);
    UnloadSound(chimeSounds[0]);
    UnloadSound(chompSounds[0]);
    
    // Unload music
    UnloadMusicStream(bgMusic);
    
    CloseAudioDevice();

    CloseWindow();
    return 0;
}