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
		enemy = new Entity(playerTexture, 1, { 0.0f, 0.0f });

	}

	Game::~Game()
	{
		if (tileMap != nullptr)
			delete tileMap;

		delete playerTexture;
		delete player;
		delete enemy;

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

		tileMap->Draw(*screen);

		vec2 moveTileMap = 0;
		vec2 oldTileMap = moveTileMap;

		if (move.left) moveTileMap.x += playerTileMapSpeed * deltaTime;
		if (move.right) moveTileMap.x -= playerTileMapSpeed * deltaTime;
		if (move.up) moveTileMap.y += playerTileMapSpeed * deltaTime;
		if (move.down) moveTileMap.y -= playerTileMapSpeed * deltaTime;

		if (move.sprint) playerTileMapSpeed = 2 * 240.0f;
		else playerTileMapSpeed = 240.0f;

		vec2 TileMapOffset = tileMap->GetOffset();

		Pixel enemyBarColor = BarColor[1];
		Bounds enemyBounds(enemy->GetBounds().At(TileMapOffset));
		if (tileMap->Collides(enemyBounds))
			enemyBarColor = BarColor[0];

		Pixel playerBarColor = BarColor[1];
		Bounds playerBounds(player->GetBounds());
		Bounds NewPlayerBounds(playerBounds);
		Bounds tileBounds(tileMap->GetTileBounds(playerBounds));
		if (tileMap->Collides(playerBounds))
		{
			playerBarColor = BarColor[0];

			if (playerBounds.min.y < tileBounds.min.y)
			{
				/*float newPos = tileBounds.min.y - TILE_SIZE.y;
				player->SetPosition({ playerPos.x, newPos });*/
				//move.down = false;
				moveTileMap = 0;
			}
		}

		tileMap->Move(moveTileMap);
		
		vec2 playerPos = player->GetPosition();
		vec2 enemyPos = enemy->GetPosition(TileMapOffset);

		float distancePlayerEnemy = enemy->GetDistancePlayerEnemy(player, TileMapOffset);
		vec2 enemyMoveBy = 0.0f;

		vec2 enemyDir = enemy->GetDirection(player, TileMapOffset);

		if (distancePlayerEnemy < TILE_SIZE.x * 5)
		{
			if (distancePlayerEnemy < TILE_SIZE.x)
			{
				vec2 enemyNewPos = { Rand(static_cast<float>(screen->GetWidth())), Rand(static_cast<float>(screen->GetHeight())) };
				enemy->SetPosition(enemyNewPos);
			}
			enemyMoveBy += (enemyDir * enemySpeed) * deltaTime;
			enemy->Move(enemyMoveBy);
		}

		

		screen->Bar(tileBounds.MinX(), tileBounds.MinY(), tileBounds.MaxX(), tileBounds.MaxY(), 0xffff0000);

		enemy->Draw(*screen, TileMapOffset.x, TileMapOffset.y);
		screen->Bar(enemyBounds.MinX(), enemyBounds.MinY(), enemyBounds.MaxX(), enemyBounds.MaxY(), enemyBarColor);
		
		player->Draw(*screen);
		screen->Bar(playerBounds.MinX(), playerBounds.MinY(), playerBounds.MaxX(), playerBounds.MaxY(), playerBarColor);

		int tileMapCenterX = static_cast<int>(tileMap->GetSizeInPixels().x / 2 + TileMapOffset.x);
		int tileMapCenterY = static_cast<int>(tileMap->GetSizeInPixels().y / 2 + TileMapOffset.y);
		screen->Bar(tileMapCenterX - 5, tileMapCenterY - 5, tileMapCenterX + 5, tileMapCenterY + 5, 0xffff0000);

		screen->Line(playerPos.x, playerPos.y, enemyPos.x, enemyPos.y, 0xffff0000);

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

