#ifndef HELPERS_H
#define HELPERS_H

#include "defs.h"

bool RowHasPlant(int row, PlantType type);
void SpawnSun(float x, float y, float targetY, bool isSun, int value);
void SpawnProjectile(int row, float x);
void SpawnExplosion(float x, float y);
void RecalculateRowWeight(int r);
int GetMinWeightRow();
void SpawnZombie();

#endif // HELPERS_H
