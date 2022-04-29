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

	static const vec2 TILE_SIZE = { 80.0f, 80.0f };

	static const Tile SNOW_TILE = { false, 0, 0, 80, 80 };
	static const Tile ROCK_TILE = { true, 3, 0, 80, 80 };
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
		tileMap->SetTiles(map, 44); //second param = number of tiles in width
		vec2 tileMapSize = tileMap->GetSizeInPixels();
		tileMap->SetOffset({ (ScreenWidth - tileMapSize.x) / 2.0f, (ScreenHeight - tileMapSize.y) / 2.0f });

		playerTexture = new Surface("assets/DoublePlayerSprite.png");
		player = new Entity(playerTexture, 2, { ScreenWidth / 2, ScreenHeight / 2 });
		for (int i = 1; i <= 3; i++)
			enemies.push_back(new Enemy(playerTexture, 2, enemySpeed, { Rand(tileMapSize.x), Rand(tileMapSize.y) }));

		BulletTexture = new Surface("assets/snowballBullet.png");
	}

	Game::~Game()
	{
		if (tileMap != nullptr)
			delete tileMap;

		delete playerTexture;
		delete player;
		for (Enemy* enemy : enemies)
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

	vec2 tileMapOffset;

	void Game::Tick(float)
	{
		Timer::Get().Tick();
		float deltaTime = static_cast<float>(Timer::Get().ElapsedSeconds());

		screen->Clear(0);

		tileMap->Draw(*screen);

		vec2 moveTileMap = 0;

		if (input.left)
		{
			moveTileMap.x += playerTileMapSpeed * deltaTime;
			player->SetFrame(0);
		}
		if (input.right)
		{
			moveTileMap.x -= playerTileMapSpeed * deltaTime;
			player->SetFrame(1);
		}
		if (input.up) moveTileMap.y += playerTileMapSpeed * deltaTime;
		if (input.down) moveTileMap.y -= playerTileMapSpeed * deltaTime;

		if (input.sprint) playerTileMapSpeed = 2 * 240.0f;
		else playerTileMapSpeed = 240.0f;

		tileMapOffset = tileMap->GetOffset();

		Pixel playerBarColor = BarColor[1];

		Bounds playerBounds(player->GetBounds() + Bounds{ 4.0f, -4.0f });
		Bounds newPlayerBounds(playerBounds.min - moveTileMap, playerBounds.max - moveTileMap);
		std::vector<Bounds> tilesBounds(tileMap->GetTilesBounds(newPlayerBounds));

		if (!tilesBounds.empty())
		{
			playerBarColor = BarColor[0];

			for (auto& collidingTile : tilesBounds)
			{
				if (((playerBounds.min.x > collidingTile.min.x && playerBounds.min.x < collidingTile.max.x) ||
					(playerBounds.max.x > collidingTile.min.x && playerBounds.max.x < collidingTile.max.x)))
					moveTileMap.y = 0;

				else if ((playerBounds.min.y > collidingTile.min.y && playerBounds.min.y < collidingTile.max.y) ||
					(playerBounds.max.y > collidingTile.min.y && playerBounds.max.y < collidingTile.max.y))
					moveTileMap.x = 0;
			}
		}

		tileMap->Move(moveTileMap);

		Pixel enemyBarColor = BarColor[1];
		for (auto enemy : enemies)
		{
			if (enemy->CheckIfAlive())
			{
			if (tileMap->Collides(enemy->GetBounds(tileMapOffset)))
				enemyBarColor = BarColor[0];

			vec2 enemyMoveBy = 0.0f;
			enemy->SetDirectionPlayerEnemy(player, tileMapOffset);
			float distance = enemy->GetDistancePlayerEnemy(player, tileMapOffset);

			if (distance < TILE_SIZE.x * tilesAway) //if the player is close enough to the enemy
			{
				enemyMoveBy = enemy->CalculateEnemyMoveBy();
				Bounds enemyBounds(enemy->GetBounds(tileMapOffset));
				Bounds newEnemyBounds(enemyBounds + enemyMoveBy + Bounds{ 1.0f, -1.0f });
				std::vector<Bounds> enemyTilesBounds(tileMap->GetTilesBounds(newEnemyBounds));

				if (!enemyTilesBounds.empty())
				{
					enemyMoveBy = 0;
				}

				//if (distancePlayerEnemy < TILE_SIZE.x) //if they are colliding - circle collision
				if (playerBounds.BoundsCollide(enemyBounds))
				{
					//vec2 enemyNewPos = { Rand(static_cast<float>(screen->GetWidth())), Rand(static_cast<float>(screen->GetHeight())) };
					enemyMoveBy = 0;
				}
				enemy->Move(enemyMoveBy);

				if (newEnemyBounds.MinX() > enemyBounds.MinX()) enemy->SetFrame(1);
				else if (newEnemyBounds.MinX() < enemyBounds.MinX()) enemy->SetFrame(0);
			}
			screen->Line(player->GetPosition().x, player->GetPosition().y, enemy->GetPosition(tileMapOffset).x, enemy->GetPosition(tileMapOffset).y, 0xffff0000);

			enemy->Draw(*screen, tileMapOffset);
			//screen->Bar(enemy->GetBounds(tileMapOffset).MinX(), enemy->GetBounds(tileMapOffset).MinY(), enemy->GetBounds(tileMapOffset).MaxX(), enemy->GetBounds(tileMapOffset).MaxY(), enemyBarColor);
			
			for (auto iter = playerBullets.begin(); iter != playerBullets.end();)
			{
				Bounds bulletBounds((*iter)->GetBounds(tileMapOffset));
				if (bulletBounds.BoundsCollide(enemy->GetBounds(tileMapOffset)))
				{
					delete* iter;
					iter = playerBullets.erase(iter);

					enemy->HitTaken();
					if (enemy->GetHitsTaken() >= enemyHitsToDie)
						enemy->SetNotAlive();
				}
				else iter++;
			}
			}
		}

		/*if (!tilesBounds.empty())
			for (auto& collidingTile : tilesBounds)
				screen->Bar(collidingTile.MinX(), collidingTile.MinY(), collidingTile.MaxX(), collidingTile.MaxY(), 0xffff0000);*/

		player->Draw(*screen);
		//screen->Bar(playerBounds.MinX(), playerBounds.MinY(), playerBounds.MaxX(), playerBounds.MaxY(), playerBarColor);
		//screen->Box(playerBounds.MinX(), playerBounds.MinY(), playerBounds.MaxX(), playerBounds.MaxY(), 0);
		//screen->Box(newPlayerBounds.MinX(), newPlayerBounds.MinY(), newPlayerBounds.MaxX(), newPlayerBounds.MaxY(), 0);
		//screen->Box(enemyBounds.MinX(), enemyBounds.MinY(), enemyBounds.MaxX(), enemyBounds.MaxY(), 0);

		int tileMapCenterX = static_cast<int>(tileMap->GetSizeInPixels().x / 2 + tileMapOffset.x);
		int tileMapCenterY = static_cast<int>(tileMap->GetSizeInPixels().y / 2 + tileMapOffset.y);
		screen->Bar(tileMapCenterX - 5, tileMapCenterY - 5, tileMapCenterX + 5, tileMapCenterY + 5, 0xffff0000);

		bulletSpawnTime += deltaTime;
		if (input.mouse)
		{
			if (bulletSpawnTime * bulletRate > 1)
			{
				bulletSpawnTime = 0;
				vec2 bulletDir = (input.mousePos - player->GetPosition()).normalized();

				playerBullets.push_back(new Bullet(BulletTexture, 1, bulletSpeed, player->GetPosition() - tileMapOffset, bulletDir));
			}
		}

		for (auto bullet : playerBullets)
		{
			bullet->Move();
			bullet->Draw(*screen, tileMapOffset);
		}

		for (auto iter = playerBullets.begin(); iter != playerBullets.end();)
		{
			vec2 bulletPos = (*iter)->GetPosition(tileMapOffset);

			if (bulletPos.x < 0 || bulletPos.y < 0 || bulletPos.x > ScreenWidth || bulletPos.y > ScreenHeight || tileMap->Collides(bulletPos))
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