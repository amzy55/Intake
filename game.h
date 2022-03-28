#pragma once

#include <SDL_scancode.h>
#include "TileMap.h"
#include "Entity.h"

namespace Tmpl8 {

	class Surface;
	class Game
	{
	public:
		Game();
		~Game();

		Game(const Game& copy) = delete;
		Game& operator=(const Game& copy) = delete;

		Game(Game&& copy) = default;
		Game& operator=(Game&& copy) = default;

		static Game& Get();

		void SetTarget(Surface* surface) { screen = surface; }
		void Init();
		void Shutdown();
		void Tick(float deltaTime);
		void MouseUp(int button) {};
		void MouseDown(int button) {};
		void MouseMove(int x, int y) {};
		void KeyUp(SDL_Scancode key);
		void KeyDown(SDL_Scancode key);

	private:
		Surface* screen = nullptr;
		TileMap* tileMap = nullptr;
		Surface* playerTexture = nullptr;
		Entity* player = nullptr;
		Entity* enemy = nullptr;

		//EntityController* entityController = nullptr;

		static Game* theGame;

		//Key input booleans.
		struct Move
		{
			bool left = false;
			bool right = false;
			bool up = false;
			bool down = false;
			bool sprint = false;
		}move;

		//Choose according to how the game feels.
		float playerTileMapSpeed = 240.0f; //60 * 4
		float enemySpeed = 180.0f; //60 * 3
	};

}; // namespace Tmpl8