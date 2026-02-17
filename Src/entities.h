#ifndef ENTITIES_H
#define ENTITIES_H

#include "raylib.h"
#include "defs.h"

// -------------------------------------------------------------------------
// GLOBAL STATE
// -------------------------------------------------------------------------
extern GameScreen currentScreen;
extern bool notPaused;
extern int sunCurrency;
extern float gameTime;
extern float sunSpawnTimer;
extern float zombieSpawnTimer;
extern float cooldownTimer[5];
extern bool potatoMineUnlocked;

// Level 4 Scoring
extern int level4Score;
extern int level4HighScore;

// Coins
extern int Coins;

// Entities
extern Plant plants[MAX_PLANTS];
extern Zombie zombies[MAX_ZOMBIES];
extern Projectile projectiles[MAX_PROJECTILES];
extern Sun suns[MAX_SUNS];
extern Explosion explosions[MAX_EXPLOSIONS];
extern Lawnmower mowers[GRID_ROWS];
extern RowWeight weights[GRID_ROWS];

// Fonts
extern Font markerTtf;

// Textures
extern Texture2D peaTexture;
extern Texture2D sunTexture;
extern Texture2D mowerTexture;
extern Texture2D frontyardTexture;
extern Texture2D frontyardnightTexture;
extern Texture2D SEEDsunflowerTexture;
extern Texture2D SEEDpeashooterTexture;
extern Texture2D SEEDchomperTexture;
extern Texture2D SEEDroseTexture;
extern Texture2D SEEDpotatoTexture;
extern Texture2D dashboardTexture;
extern Texture2D packetframeTexture;
extern Texture2D lockTexture;
extern Texture2D levelselectionTexture;
extern Texture2D pausescreenTexture;
extern Texture2D mainmenu1Texture;
extern Texture2D mainmenu2Texture;
extern Texture2D mainmenu3Texture;
extern Texture2D shopscreenTexture;
extern Texture2D coinbagTexture;
extern Texture2D soldoutTexture;
extern Texture2D coinboardTexture;
extern Texture2D statboardTexture;

// Animated GIF structures for each plant type
extern Image sunflowerAnimImage;
extern Texture2D sunflowerAnimTexture;
extern int sunflowerAnimFrames;
extern int sunflowerAnimCurrentFrame;
extern int sunflowerAnimFrameDelay;
extern int sunflowerAnimFrameCounter;
extern int sunflowerAnimFPS;

extern Image peashooterAnimImage;
extern Texture2D peashooterAnimTexture;
extern int peashooterAnimFrames;
extern int peashooterAnimCurrentFrame;
extern int peashooterAnimFrameDelay;
extern int peashooterAnimFrameCounter;
extern int peashooterAnimFPS;

extern Image chomperAnimImage;
extern Texture2D chomperAnimTexture;
extern int chomperAnimFrames;
extern int chomperAnimCurrentFrame;
extern int chomperAnimFrameDelay;
extern int chomperAnimFrameCounter;
extern int chomperAnimFPS;

extern Image roseAnimImage;
extern Texture2D roseAnimTexture;
extern int roseAnimFrames;
extern int roseAnimCurrentFrame;
extern int roseAnimFrameDelay;
extern int roseAnimFrameCounter;
extern int roseAnimFPS;

extern Image potatoAnimImage;
extern Texture2D potatoAnimTexture;
extern int potatoAnimFrames;
extern int potatoAnimCurrentFrame;
extern int potatoAnimFrameDelay;
extern int potatoAnimFrameCounter;
extern int potatoAnimFPS;

extern Image normalzAnimImage;
extern Texture2D normalzAnimTexture;
extern int normalzAnimFrames;
extern int normalzAnimCurrentFrame;
extern int normalzAnimFrameDelay;
extern int normalzAnimFrameCounter;
extern int normalzAnimFPS;

extern Image thinkerzAnimImage;
extern Texture2D thinkerzAnimTexture;
extern int thinkerzAnimFrames;
extern int thinkerzAnimCurrentFrame;
extern int thinkerzAnimFrameDelay;
extern int thinkerzAnimFrameCounter;
extern int thinkerzAnimFPS;

extern Image EATnormalzAnimImage;
extern Texture2D EATnormalzAnimTexture;
extern int EATnormalzAnimFrames;
extern int EATnormalzAnimCurrentFrame;
extern int EATnormalzAnimFrameDelay;
extern int EATnormalzAnimFrameCounter;
extern int EATnormalzAnimFPS;

extern Image EATthinkerzAnimImage;
extern Texture2D EATthinkerzAnimTexture;
extern int EATthinkerzAnimFrames;
extern int EATthinkerzAnimCurrentFrame;
extern int EATthinkerzAnimFrameDelay;
extern int EATthinkerzAnimFrameCounter;
extern int EATthinkerzAnimFPS;

extern Image explosionAnimImage;
extern Texture2D explosionAnimTexture;
extern int explosionAnimFrames;
extern int explosionAnimFPS;

// UI Selection
extern PlantType selectedPlant;
extern int currentLevel;

// Level Tracking
extern int zombiesSpawned;
extern int zombiesToSpawnNormal;
extern int zombiesToSpawnThinker;
extern int totalLevelZombies;

// -------------------------------------------------------------------------
// AUDIO GLOBALS 
// -------------------------------------------------------------------------
extern Music bgMusic;
extern int currentMusicLevelID;

extern Sound sunSound;
extern Sound pauseSound;
extern Sound plantsound;
extern Sound buzzerSound;
extern Sound loseSound;
extern Sound winSound;
extern Sound seedliftSound;
extern Sound throwSound;
extern Sound tapSound;
extern Sound gravebuttonSound;
extern Sound coinSound;

extern Sound groanSounds[6];

// Arrays for concurrent sounds (Aliases)
extern Sound explosionSounds[MAX_SFX_INSTANCES];
extern int explosionSoundIdx;

extern Sound splatSounds[MAX_SFX_INSTANCES];
extern int splatSoundIdx;

extern Sound mowerSounds[5];
extern int mowerSoundIdx;

extern Sound frozenSounds[5];
extern int frozenSoundIdx;

extern Sound chimeSounds[5];
extern int chimeSoundIdx;

extern Sound chompSounds[MAX_SFX_INSTANCES];
extern int chompSoundIdx;

#endif // ENTITIES_H
