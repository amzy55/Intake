#pragma once

const float TILE_SIZE_FLOAT = 78.0f;
const int TILE_SIZE_INT = static_cast<int>(TILE_SIZE_FLOAT);

float playerTileMapSpeed = 240.0f; //60 * 4
const float enemySpeed = 220.0f; //60 * 3
const int playerHitsToDie = 3;
const int enemyHitsToDie = 5;
const float bulletSpeed = 1000.0f;
int bulletRate = 3; //bullets per second
int tilesAway = 5;
const int totalEnemies = 10;
int enemiesAlive = totalEnemies;
int score = 0;
int damageDelay = 2;

float bulletSpawnTime = 0;
float timeSinceDamage = 1;