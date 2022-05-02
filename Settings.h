#pragma once

float TILE_SIZE_FLOAT = 80.0f;
int TILE_SIZE_INT = 80;
static const Tmpl8::vec2 TILE_SIZE_VEC2 = { 80.0f, 80.0f };

Tmpl8::vec2 playerStartPos = 0;
float playerTileMapSpeed = 240.0f; //60 * 4
const float enemySpeed = 220.0f; //60 * 3
const int playerHitsToDie = 3;
const int enemyHitsToDie = 5;
const float bulletSpeed = 1000.0f;
int bulletRate = 3; //bullets per second
int tilesAway = 5;
const int totalEnemies = 10;
int enemiesAlive = totalEnemies;
int scoreNumber = 0;

float bulletSpawnTime = 0;
float enemyDamageStart = 1;