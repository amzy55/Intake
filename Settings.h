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
constexpr int totalEnemies = 20;
constexpr int randomEnemies = 20;
constexpr int setEnemies = 10;
int enemiesDefeated = 0;
constexpr int gemCount = 100;

float bulletSpawnTime = 0;
float timeSinceDamage = 2;
float timeSinceDmgAnim = damageDelay;
Tmpl8::vec2 enemyPos[totalEnemies] = {};
float enemyDist[totalEnemies] = {};
int i = -1;
Tmpl8::vec2 linePos = 0.0f;

Tmpl8::vec2 setEnemyPos[setEnemies] = {
	{48.0f * TILE_SIZE_FLOAT + TILE_SIZE_FLOAT / 2.0f, 4.0f * TILE_SIZE_FLOAT + TILE_SIZE_FLOAT / 2.0f },
	{7.0f * TILE_SIZE_FLOAT + TILE_SIZE_FLOAT / 2.0f, 14.0f * TILE_SIZE_FLOAT + TILE_SIZE_FLOAT / 2.0f },
	{24.0f * TILE_SIZE_FLOAT + TILE_SIZE_FLOAT / 2.0f, 16.0f * TILE_SIZE_FLOAT + TILE_SIZE_FLOAT / 2.0f },
	{50.0f * TILE_SIZE_FLOAT + TILE_SIZE_FLOAT / 2.0f, 19.0f * TILE_SIZE_FLOAT + TILE_SIZE_FLOAT / 2.0f },
	{35.0f * TILE_SIZE_FLOAT + TILE_SIZE_FLOAT / 2.0f, 28.0f * TILE_SIZE_FLOAT + TILE_SIZE_FLOAT / 2.0f },
	{9.0f * TILE_SIZE_FLOAT + TILE_SIZE_FLOAT / 2.0f, 37.0f * TILE_SIZE_FLOAT + TILE_SIZE_FLOAT / 2.0f },
	{53.0f * TILE_SIZE_FLOAT + TILE_SIZE_FLOAT / 2.0f, 37.0f * TILE_SIZE_FLOAT + TILE_SIZE_FLOAT / 2.0f },
	{7.0f * TILE_SIZE_FLOAT + TILE_SIZE_FLOAT / 2.0f, 45.0f * TILE_SIZE_FLOAT + TILE_SIZE_FLOAT / 2.0f },
	{28.0f * TILE_SIZE_FLOAT + TILE_SIZE_FLOAT / 2.0f, 55.0f * TILE_SIZE_FLOAT + TILE_SIZE_FLOAT / 2.0f },
	{42.0f * TILE_SIZE_FLOAT + TILE_SIZE_FLOAT / 2.0f, 53.0f * TILE_SIZE_FLOAT + TILE_SIZE_FLOAT / 2.0f },
};
