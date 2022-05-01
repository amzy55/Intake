#pragma once

float TILE_SIZE_FLOAT = 80.0f;
int TILE_SIZE_INT = 80;
static const Tmpl8::vec2 TILE_SIZE_VEC2 = { 80.0f, 80.0f };

Tmpl8::vec2 playerStartPos = 0;
float playerTileMapSpeed = 240.0f; //60 * 4
float enemySpeed = 180.0f; //60 * 3
int playerHitsToDie = 3;
int enemyHitsToDie = 5;
float bulletSpeed = 1000.0f;
int bulletRate = 3; //bullets per second
int enemyPerSecond = 1;
int tilesAway = 5;
int nrOfEnemies = 10;

float bulletSpawnTime = 0;
float enemyDamageStart = 0;