#pragma once

const float TILE_SIZE_FLOAT = 78.0f;
const int TILE_SIZE_INT = static_cast<int>(TILE_SIZE_FLOAT);

const float playerTileMapSpeedConst = 360.0f; //60 * 4
float playerTileMapSpeed = playerTileMapSpeedConst;
const float enemySpeed = playerTileMapSpeedConst / 3 * 2;
const int playerHitsToDie = 3;
const int enemyHitsToDie = 5;
const float bulletSpeed = 1000.0f;
int bulletRate = 3; //bullets per second
int tilesAway = 5;
const int totalEnemies = 10;
int enemiesDefeated = 0;
int score = 0;
int damageDelay = 2;

float bulletSpawnTime = 0;
float timeSinceDamage = 1;