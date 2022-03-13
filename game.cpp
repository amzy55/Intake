#include "Bounds.h"
#include "game.h"
#include "surface.h"
#include "template.h"
#include "Timer.h"

#include <SDL_scancode.h>
#include <cstddef>
#include <cassert>


namespace Tmpl8
{
	Game* Game::theGame = nullptr;

	static const Tile WATER_TILE = { true, 10, 2, 32, 32 };
	static const Tile PATH_TILE = { false, 5, 1, 32, 32 };
	static const Tile WATER_BORDER = { true, 11, 2, 32, 32 };

	std::vector<Tile> map = {
		#include "map.txt"
	};

	Game::Game()
		: screen(nullptr)
	{
		assert(theGame == nullptr);

		theGame = this;

		tileMap = new TileMap("assets/nc2tiles.png");
		tileMap->SetTiles(map, 26);
		vec2 tileMapSize = tileMap->GetSizeInPixels();
		tileMap->SetOffset({ (ScreenWidth - tileMapSize.x) / 2.0f, (ScreenHeight - tileMapSize.y) / 2.0f });
	}

	Game::~Game()
	{
		//delete playerController;
		//delete playerEntity;
		//delete playerTexture;
		if (tileMap != nullptr)
			delete tileMap;

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

		screen->Clear(0);

		tileMap->Draw(*screen);
	}

};