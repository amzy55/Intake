#pragma once

constexpr float TILE_SIZE_FLOAT = 78.0f;
constexpr int TILE_SIZE_INT = static_cast<int>(TILE_SIZE_FLOAT);

constexpr float playerTileMapSpeedConst = 360.0f; //60 * 4
float playerTileMapSpeed = playerTileMapSpeedConst;
constexpr float enemySpeed = playerTileMapSpeedConst / 3 * 2;
constexpr int playerHitsToDie = 3;
constexpr int enemyHitsToDie = 5;
constexpr float bulletSpeed = 1000.0f;
constexpr int bulletRate = 3; //bullets per second
constexpr int tilesAway = 5;
constexpr int damageDelay = 2;
constexpr int totalEnemies = 10;
int enemiesDefeated = 0;

float bulletSpawnTime = 0;
float timeSinceDamage = 1;
float timeSinceDmgAnim = 2;