#pragma once

#include <SDL_scancode.h>
#include "TileMap.h"
#include "Entity.h"
#include <vector>
#include "Bullet.h"
#include "Enemy.h"
#include "Button.h"
#include "Player.h"
#include "AudioPlayer.h"
#include "Collectable.h"

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
		void MouseUp(int button) { input.mouseClick = false; };
		void MouseDown(int button) { input.mouseClick = true; };
		void MouseMove(vec2 mousePos) { input.mousePos = mousePos; };
		void KeyUp(SDL_Scancode key);
		void KeyDown(SDL_Scancode key);

	private:
		static Game* theGame;
		Surface* screen = nullptr;
		TileMap* tileMap = nullptr;

		Surface* playerTexture = nullptr;
		Surface* bulletTexture = nullptr;
		Surface* heartsTexture = nullptr;
		Surface* startButtonTexture = nullptr;
		Surface* gemTexture = nullptr;
		Surface* startScreenTexture = nullptr;
		Surface* endScreenTexture = nullptr;

		Player* player = nullptr;
		std::vector<Enemy*> enemies;
		std::vector<Bullet*> playerBullets;
		std::vector<Entity*> hearts;
		std::vector<Collectable*> gems;

		Button* startButton = nullptr;
		Sprite* startScreenBG = nullptr;
		Sprite* endScreenBG = nullptr;

		//Key input booleans.
		struct Input
		{
			bool left = false;
			bool right = false;
			bool up = false;
			bool down = false;
			bool sprint = false;

			bool mouseClick = false;
			vec2 mousePos = 0.0f;
		}input;

		int score = 0;

		enum class GameState
		{
			START = 1,
			GAMEPLAY = 2,
			END = 3
		};
		GameState gameState = GameState::START;

		AudioPlayer damageSound{ "assets/damage.wav" };
		AudioPlayer startSound{ "assets/start.wav" };
		AudioPlayer shotSound{ "assets/bulletShot.wav" };
		AudioPlayer gameOverSound{ "assets/gameOver.wav" };
		AudioPlayer youWinSound{ "assets/youWin.wav" };
		AudioPlayer hiSound{ "assets/hi.wav" };
	};

}; // namespace Tmpl8
