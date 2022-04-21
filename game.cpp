#include "Bounds.h"
#include "game.h"
#include "surface.h"
#include "template.h"
#include "Timer.h"
#include<SDL.h>

#include <cstddef>
#include <cassert>


namespace Tmpl8
{
	Game* Game::theGame = nullptr;

	/*static const Tile WATER_TILE = { true, 10, 2, 32, 32 };
	static const Tile PATH_TILE = { false, 5, 1, 32, 32 };
	static const Tile WATER_BORDER = { true, 11, 2, 32, 32 };*/

	static const vec2 TILE_SIZE = { 80.0f, 80.0f };

	static const Tile SNOW_TILE = { false, 0, 0, 80, 80 };
	static const Tile ROCK_TILE = { true, 1, 0, 80, 80 };
	static const Tile RED_TILE = { true, 2, 0, 80, 80 };

	static const Pixel BarColor[2] = { 0xbf42f5, 0x36c75c };

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

		playerTexture = new Surface("assets/PlayerSprite.png");
		player = new Entity(playerTexture, 1, { ScreenWidth / 2, ScreenHeight / 2 });
		enemy = new Entity(playerTexture, 1, { 480.0f, 160.0f });

		BulletTexture = new Surface("assets/snowballBullet.png");
	}

	Game::~Game()
	{
		if (tileMap != nullptr)
			delete tileMap;

		delete playerTexture;
		delete player;
		delete enemy;

		delete BulletTexture;

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

	vec2 TileMapOffset;

	void Game::Tick(float)
	{
		Timer::Get().Tick();
		float deltaTime = static_cast<float>(Timer::Get().ElapsedSeconds());

		screen->Clear(0);

		tileMap->Draw(*screen);

		vec2 moveTileMap = 0;

		if (input.left) moveTileMap.x += playerTileMapSpeed * deltaTime;
		if (input.right) moveTileMap.x -= playerTileMapSpeed * deltaTime;
		if (input.up) moveTileMap.y += playerTileMapSpeed * deltaTime;
		if (input.down) moveTileMap.y -= playerTileMapSpeed * deltaTime;

		if (input.sprint) playerTileMapSpeed = 2 * 240.0f;
		else playerTileMapSpeed = 240.0f;

		TileMapOffset = tileMap->GetOffset();

		vec2 playerPos = player->GetPosition();
		vec2 enemyPos = enemy->GetPosition(TileMapOffset);

		Pixel enemyBarColor = BarColor[1];
		Bounds enemyBounds(enemy->GetBounds().Add(TileMapOffset));
		if (tileMap->Collides(enemyBounds))
			enemyBarColor = BarColor[0];

		Pixel playerBarColor = BarColor[1];
		Bounds playerBounds(player->GetBounds()/* + Bounds{10.0f, -10.0f}*/);

		Bounds newPlayerBounds(playerBounds.min - moveTileMap, playerBounds.max - moveTileMap);

		std::vector<Bounds> tilesBounds(tileMap->GetTilesBounds(newPlayerBounds));

		if (!tilesBounds.empty())
		{
			playerBarColor = BarColor[0];
			moveTileMap = 0;

			//if ((newPlayerBounds.max.y > tilesBounds[0].min.y) || (newPlayerBounds.min.y > tilesBounds[0].max.y))
			//	moveTileMap.y = 0;

			//else if ((newPlayerBounds.max.x > tilesBounds[0].min.x) || (newPlayerBounds.min.x > tilesBounds[0].max.x))
			//	moveTileMap.x = 0;

			//for (auto& collidingTile : tilesBounds)
			//{
			//	if ((newPlayerBounds.max.y > collidingTile.min.y) || (newPlayerBounds.min.y < collidingTile.max.y) && 
			//		((newPlayerBounds.min.x > collidingTile.min.x && newPlayerBounds.min.x < collidingTile.max.x) || 
			//		(newPlayerBounds.max.x > collidingTile.min.x && newPlayerBounds.max.x < collidingTile.max.x)))
			//		moveTileMap.y = 0;

			//	else if ((newPlayerBounds.max.x > collidingTile.min.x) || (newPlayerBounds.min.x < collidingTile.max.x))
			//		moveTileMap.x = 0;
			//}

		}

		tileMap->Move(moveTileMap);
		
		float distancePlayerEnemy = enemy->GetDistancePlayerEnemy(player, TileMapOffset);
		vec2 enemyMoveBy = 0.0f;

		vec2 enemyDir = enemy->GetDirectionPlayerEnemy(player, TileMapOffset);

		if (distancePlayerEnemy < TILE_SIZE.x * 5) //if the player is close enough to the enemy
		{
			enemyMoveBy = (enemyDir * enemySpeed) * deltaTime;
			Bounds newEnemyBounds(enemyBounds.min + enemyMoveBy + 1.0f, enemyBounds.max + enemyMoveBy - 1.0f);
			std::vector<Bounds> enemyTilesBounds(tileMap->GetTilesBounds(newEnemyBounds));

			if (!enemyTilesBounds.empty())
			{
				enemyMoveBy = 0;
			}

			if (distancePlayerEnemy < TILE_SIZE.x) //if they are colliding
			{
				//vec2 enemyNewPos = { Rand(static_cast<float>(screen->GetWidth())), Rand(static_cast<float>(screen->GetHeight())) };
				enemyMoveBy = 0;
			}
			enemy->Move(enemyMoveBy);
		}

		if (!tilesBounds.empty())
			for (auto& collidingTile : tilesBounds)
				screen->Bar(collidingTile.MinX(), collidingTile.MinY(), collidingTile.MaxX(), collidingTile.MaxY(), 0xffff0000);

		enemy->Draw(*screen, TileMapOffset);
		//screen->Bar(enemyBounds.MinX(), enemyBounds.MinY(), enemyBounds.MaxX(), enemyBounds.MaxY(), enemyBarColor);
		
		player->Draw(*screen);
		screen->Bar(playerBounds.MinX(), playerBounds.MinY(), playerBounds.MaxX(), playerBounds.MaxY(), playerBarColor);
		screen->Box(newPlayerBounds.MinX(), newPlayerBounds.MinY(), newPlayerBounds.MaxX(), newPlayerBounds.MaxY(), 0);

		int tileMapCenterX = static_cast<int>(tileMap->GetSizeInPixels().x / 2 + TileMapOffset.x);
		int tileMapCenterY = static_cast<int>(tileMap->GetSizeInPixels().y / 2 + TileMapOffset.y);
		screen->Bar(tileMapCenterX - 5, tileMapCenterY - 5, tileMapCenterX + 5, tileMapCenterY + 5, 0xffff0000);

		screen->Line(playerPos.x, playerPos.y, enemyPos.x, enemyPos.y, 0xffff0000);

		vec2 bulletMoveBy = 0.0f;

		bulletSpawnTime += deltaTime;
		if (input.mouse)
		{
			if (bulletSpawnTime * bulletRate > 1)
			{
				bulletSpawnTime = 0;
				vec2 bulletDir = (input.mousePos - playerPos).normalized();

				playerBullets.push_back(new Bullet(BulletTexture, 1, bulletSpeed, playerPos - TileMapOffset, bulletDir));
			}
		}

		for (int i = 0; i < playerBullets.size(); i++)
		{
			playerBullets[i]->Move();
			playerBullets[i]->Draw(*screen, TileMapOffset);
		}

		for (auto iter = playerBullets.begin(); iter != playerBullets.end();)
		{	
			vec2 bulletPos = (*iter)->GetPosition(TileMapOffset);
			if (bulletPos.x < 0 || bulletPos.y < 0 || bulletPos.x > ScreenWidth || bulletPos.y > ScreenHeight)
			{	
				delete *iter;
				iter = playerBullets.erase(iter);
			}
			else iter++;
		}

		int i = 3; //breakpoint
	}

	void Game::KeyDown(SDL_Scancode key)
	{
		switch (key)
		{
		case SDL_SCANCODE_A:
		case SDL_SCANCODE_LEFT:
			input.left = true;
			break;

		case SDL_SCANCODE_D:
		case SDL_SCANCODE_RIGHT:
			input.right = true;
			break;

		case SDL_SCANCODE_W:
		case SDL_SCANCODE_UP:
			input.up = true;
			break;

		case SDL_SCANCODE_S:
		case SDL_SCANCODE_DOWN:
			input.down = true;
			break;

		case SDL_SCANCODE_LSHIFT:
			input.sprint = true;
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
			input.left = false;
			break;

		case SDL_SCANCODE_D:
		case SDL_SCANCODE_RIGHT:
			input.right = false;
			break;

		case SDL_SCANCODE_W:
		case SDL_SCANCODE_UP:
			input.up = false;
			break;

		case SDL_SCANCODE_S:
		case SDL_SCANCODE_DOWN:
			input.down = false;
			break;

		case SDL_SCANCODE_LSHIFT:
			input.sprint = false;
			break;

		default:
			break;
		}
	}
};

