#include "Bounds.h"
#include "game.h"
#include "surface.h"
#include "template.h"
#include "Timer.h"

#include <cstddef>
#include <cassert>


namespace Tmpl8
{
	Game* Game::theGame = nullptr;

	/*static const Tile WATER_TILE = { true, 10, 2, 32, 32 };
	static const Tile PATH_TILE = { false, 5, 1, 32, 32 };
	static const Tile WATER_BORDER = { true, 11, 2, 32, 32 };*/

	static const Tile SNOW_TILE = { false, 0, 0, 80, 80 };
	static const Tile ROCK_TILE = { true, 1, 0, 80, 80 };
	static const Tile RED_TILE = { true, 2, 0, 80, 80 };

	std::vector<Tile> map = {
		#include "snowMap.txt"
	};

	Game::Game()
		: screen(nullptr)
	{
		assert(theGame == nullptr); 

		theGame = this;

		tileMap = new TileMap("assets/TilesTexture.png");
		tileMap->SetTiles(map, 11); //second param = number of tiles in width
		vec2 tileMapSize = tileMap->GetSizeInPixels();
		tileMap->SetOffset({ (ScreenWidth - tileMapSize.x) / 2.0f, (ScreenHeight - tileMapSize.y) / 2.0f });

		playerTexture = new Surface("assets/80x80square.png");
		player = new Entity(playerTexture, 1, { ScreenWidth / 2, ScreenHeight / 2 });
		enemy = new Entity(playerTexture, 1, { 0.0f, 0.0f });

	}

	Game::~Game()
	{
		if (tileMap != nullptr)
			delete tileMap;

		delete playerTexture;
		delete player;

		theGame = nullptr;
	}

	Game& Game::Get()
	{
		assert(theGame != nullptr);
		return *theGame;
	}
	
	void Game::Init()
	{}

	void Game::Shutdown() {}

	void Game::Tick(float)
	{
		Timer::Get().Tick();
		float deltaTime = static_cast<float>(Timer::Get().ElapsedSeconds());

		screen->Clear(0);

		vec2 moveTileMap = 0;

		if (move.left) moveTileMap.x += playerTileMapSpeed * deltaTime;
		if (move.right) moveTileMap.x -= playerTileMapSpeed * deltaTime;
		if (move.up) moveTileMap.y += playerTileMapSpeed * deltaTime;
		if (move.down) moveTileMap.y -= playerTileMapSpeed * deltaTime;

		if (move.sprint) playerTileMapSpeed = 2 * 240.0f;
		else playerTileMapSpeed = 240.0f;

		tileMap->Translate(moveTileMap);
		tileMap->Draw(*screen);
		vec2 TileMapOffset = tileMap->GetOffset();
		
		vec2 PlayerPos = player->GetPosition();
		vec2 EnemyPos = enemy->GetPosition(TileMapOffset);

		float distancePlayerEnemy = player->DistancePlayerEnemy(enemy, TileMapOffset);
		vec2 enemyMoveBy = 0.0f;

		vec2 enemyVel = PlayerPos - EnemyPos;
		//normalize
		vec2 normalizedEnemyVel = enemyVel / distancePlayerEnemy;

		if (distancePlayerEnemy < SNOW_TILE.width * 5)
		{
			if (distancePlayerEnemy < SNOW_TILE.width)
			{
				vec2 enemyNewPos = { static_cast<float>(IRand(800)), static_cast<float>(IRand(512)) };
				enemy->SetPosition(enemyNewPos);
			}

			if (EnemyPos.x < PlayerPos.x)
				enemyMoveBy.x += (normalizedEnemyVel.x * enemySpeed) * deltaTime;

			if (EnemyPos.x > PlayerPos.x)
				enemyMoveBy.x += (normalizedEnemyVel.x * enemySpeed) * deltaTime;

			if (EnemyPos.y < PlayerPos.y)
				enemyMoveBy.y += (normalizedEnemyVel.y * enemySpeed) * deltaTime;

			if (EnemyPos.y > PlayerPos.y)
				enemyMoveBy.y += (normalizedEnemyVel.y * enemySpeed) * deltaTime;

			enemy->Move(enemyMoveBy);
		}

		/*if (distancePlayerEnemy < SNOW_TILE.width * 3)
		{
			if (distancePlayerEnemy < SNOW_TILE.width)
			{
				vec2 enemyNewPos = { static_cast<float>(IRand(800)), static_cast<float>(IRand(512)) };
				enemy->SetPosition(enemyNewPos);
			}

			if (EnemyPos.x < PlayerPos.x)
				enemyMoveBy.x += enemySpeed * deltaTime;

			if (EnemyPos.x > PlayerPos.x)
				enemyMoveBy.x -= enemySpeed * deltaTime;

			if (EnemyPos.y < PlayerPos.y)
				enemyMoveBy.y += enemySpeed * deltaTime;

			if (EnemyPos.y > PlayerPos.y)
				enemyMoveBy.y -= enemySpeed * deltaTime;

			enemy->Move(enemyMoveBy);
		}*/

		enemy->Draw(*screen, TileMapOffset.x, TileMapOffset.y);
		player->Draw(*screen);

		int tileMapCenterX = static_cast<int>(tileMap->GetSizeInPixels().x / 2 + TileMapOffset.x);
		int tileMapCenterY = static_cast<int>(tileMap->GetSizeInPixels().y / 2 + TileMapOffset.y);
		screen->Bar(tileMapCenterX - 5, tileMapCenterY - 5, tileMapCenterX + 5, tileMapCenterY + 5, 0xffff0000);

		screen->Line(PlayerPos.x, PlayerPos.y, EnemyPos.x, EnemyPos.y, 0xffff0000);

		int i = 3; //breakpoint
	}

	void Game::KeyDown(SDL_Scancode key)
	{
		switch (key)
		{
		case SDL_SCANCODE_A:
		case SDL_SCANCODE_LEFT:
			move.left = true;
			break;

		case SDL_SCANCODE_D:
		case SDL_SCANCODE_RIGHT:
			move.right = true;
			break;

		case SDL_SCANCODE_W:
		case SDL_SCANCODE_UP:
			move.up = true;
			break;

		case SDL_SCANCODE_S:
		case SDL_SCANCODE_DOWN:
			move.down = true;
			break;

		case SDL_SCANCODE_LSHIFT:
			move.sprint = true;
			break;

		default:
			break;
		}
	}

	void Game::KeyUp(SDL_Scancode key)
	{
		switch (key)
		{
		case SDL_SCANCODE_A:
		case SDL_SCANCODE_LEFT:
			move.left = false;
			break;

		case SDL_SCANCODE_D:
		case SDL_SCANCODE_RIGHT:
			move.right = false;
			break;

		case SDL_SCANCODE_W:
		case SDL_SCANCODE_UP:
			move.up = false;
			break;

		case SDL_SCANCODE_S:
		case SDL_SCANCODE_DOWN:
			move.down = false;
			break;

		case SDL_SCANCODE_LSHIFT:
			move.sprint = false;
			break;

		default:
			break;
		}
	}
};

