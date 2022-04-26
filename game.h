#pragma once

#include <SDL_scancode.h>
#include "TileMap.h"
#include "Entity.h"
#include <vector>
#include "Bullet.h"
#include "Enemy.h"

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
		void MouseUp(int button) { input.mouse = false; };
		void MouseDown(int button) { input.mouse = true; };
		void MouseMove(vec2 mousePos) { input.mousePos = mousePos; };
		void KeyUp(SDL_Scancode key);
		void KeyDown(SDL_Scancode key);

	private:
		Surface* screen = nullptr;
		TileMap* tileMap = nullptr;
		Surface* playerTexture = nullptr;
		Entity* player = nullptr;
		Enemy* enemy = nullptr;
		std::vector<Enemy*> enemies;
		Surface* BulletTexture = nullptr;
		std::vector<Bullet*> playerBullets;

		//EntityController* entityController = nullptr;

		static Game* theGame;

		//Key input booleans.
		struct Input
		{
			bool left = false;
			bool right = false;
			bool up = false;
			bool down = false;
			bool sprint = false;

			bool mouse = false;
			vec2 mousePos = 0.0f;
		}input;

		//Choose according to how the game feels.
		float playerTileMapSpeed = 240.0f; //60 * 4
		float enemySpeed = 180.0f; //60 * 3
		float bulletSpeed = 800.0f;
		int bulletRate = 3; //bullets per second
		int tilesAway = 3;

		float bulletSpawnTime = 0;
	};

}; // namespace Tmpl8
