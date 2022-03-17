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
		enemy = new Entity(playerTexture, 1, {0,0});

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

		if (move.left) moveTileMap.x += speed * deltaTime;
		if (move.right) moveTileMap.x -= speed * deltaTime;
		if (move.up) moveTileMap.y += speed * deltaTime;
		if (move.down) moveTileMap.y -= speed * deltaTime;

		tileMap->Translate(moveTileMap);
		tileMap->Draw(*screen);
		vec2 TileMapOffset = tileMap->GetOffset();

		player->Draw(*screen);
		enemy->Draw(*screen, TileMapOffset.x, TileMapOffset.y);
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

		default:
			break;
		}
	}
};

