#include "data.h"
#include "entities.h"
#include <stdio.h>

void SaveLevel4HighScore() {
    FILE *file = fopen("../Data/HI.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d\n", level4HighScore);
        fclose(file);
    }
}

void LoadLevel4HighScore() {
    FILE *file = fopen("../Data/HI.txt", "r");
    if (file != NULL) {
        fscanf(file, "%d", &level4HighScore);
        fclose(file);
    } else {
        level4HighScore = 0;
    }
}

void SaveCoinBalance() {
    FILE *file = fopen("../Data/coins.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d\n", Coins);
        fclose(file);
    }
}

void LoadCoinBalance() {
    FILE *file = fopen("../Data/coins.txt", "r");
    if (file != NULL) {
        fscanf(file, "%d", &Coins);
        fclose(file);
    } else {
        Coins = 0;
    }
}

void SavePotatoMineUnlock() {
    FILE *file = fopen("../Data/Shopstat.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d\n", potatoMineUnlocked ? 1 : 0);
        fclose(file);
    }
}

void LoadPotatoMineUnlock() {
    FILE *file = fopen("../Data/Shopstat.txt", "r");
    if (file != NULL) {
        int unlocked;
        fscanf(file, "%d", &unlocked);
        potatoMineUnlocked = (unlocked == 1);
        fclose(file);
    } else {
        potatoMineUnlocked = false;
    }
}
