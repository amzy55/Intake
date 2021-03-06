#include "Bounds.h"
#include "game.h"
#include "surface.h"
#include "template.h"
#include "Timer.h"
#include "Settings.h"
#include "UIText.h"
#include <SDL.h>
#include <algorithm>

#include <cstddef>
#include <cassert>

namespace Tmpl8
{
	Game* Game::theGame = nullptr;

	static Tile* I = new Tile{ false, true, 0, 0, TILE_SIZE_INT }; //snow
	static Tile* L = new Tile{ false, false, 0, 0, TILE_SIZE_INT }; //snow but enemies can't spawn
	static Tile* O = new Tile{ true, true, 1, 0, TILE_SIZE_INT }; //rock

	const Tile* map []= {
		#include "snowMap.txt"
	};

	Game::Game()
		: screen(nullptr)
	{
		assert(theGame == nullptr);

		theGame = this;

		tileMap = new TileMap("assets/TilesTexture78.png");
		tileMap->SetTiles(map, 60); //second param = number of tiles in width
		vec2 tileMapSize = tileMap->GetSizeInPixels();
		tileMap->SetOffset({-TILE_SIZE_INT * 4, -(tileMapSize.y - ScreenHeight - TILE_SIZE_INT * 2) }); //Start position

		playerTexture = new Surface("assets/player.png");
		player = new Player(playerTexture, 4, { ScreenWidth / 2, ScreenHeight / 2 });

		enemy1Texture = new Surface("assets/enemy1.png");
		enemy2Texture = new Surface("assets/enemy2.png");
		for (int i = 0; i < setEnemies; i++)
			enemies.push_back(new Enemy(enemy1Texture, 2, enemySpeed, setEnemyPos[i]));

		for (int i = 4; i < 17; ++i)
			for (int j = 51; j < 57; ++j)
				aroundPlayer.push_back({ i * TILE_SIZE_FLOAT + TILE_SIZE_FLOAT / 2.0f, j * TILE_SIZE_FLOAT + TILE_SIZE_FLOAT / 2.0f });

		std::vector<vec2> enemyPositions = tileMap->GetNonCollidingPosEnemies();
		std::vector<vec2> gemPositions = tileMap->GetNonCollidingPos();
		std::random_shuffle(enemyPositions.begin(), enemyPositions.end());
		std::random_shuffle(gemPositions.begin(), gemPositions.end());

		for (int i = 0; i < randomEnemies; i++)
			enemies.push_back(new Enemy(enemy2Texture, 2, enemySpeed, enemyPositions[i]));
		
		bulletTexture = new Surface("assets/snowballBullet.png");

		heartsTexture = new Surface("assets/hearts.png");
		for (int i = 0; i < playerHitsToDie; ++i)
			hearts.push_back(new Entity(heartsTexture, 2));

		startButtonTexture = new Surface("assets/startButton.png");
		startButton = new Button(startButtonTexture, 2, { ScreenWidth / 2, ScreenHeight / 2 - 90.0f});
		
		gemTexture = new Surface("assets/gem.png");
		for (int i = 0; i < gemCount; ++i)
			gems.push_back(new Collectable(gemTexture, 1, gemPositions[i]));

		hiSound.PlaySound();

		startScreenTexture = new Surface("assets/startBG.png");
		startScreenBG = new Sprite(startScreenTexture, 1);
		gunTexture = new Surface("assets/gun.png");
		gun = new Sprite(gunTexture, 1);

		endScreenTexture = new Surface("assets/endBG.png");
		endScreenBG = new Sprite(endScreenTexture, 1);
	}

	Game::~Game()
	{
		if (tileMap != nullptr)
			delete tileMap;

		delete I;
		delete O;
		delete L;

		delete player;
		for (Enemy* enemy : enemies)
			delete enemy;
		for (Bullet* bullet : playerBullets)
			delete bullet;
		for (Entity* heart : hearts)
			delete heart;
		for (Collectable* gem : gems)
			delete gem;
		delete startButton;
		delete startScreenBG;
		delete endScreenBG;
	

		delete playerTexture;
		delete bulletTexture;
		delete heartsTexture;
		delete startButtonTexture;
		delete gemTexture;
		delete startScreenTexture;
		delete endScreenTexture;

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
		if (gameState == GameState::START)
		{
			startScreenBG->Draw(screen, 0, 0);
			gun->Draw(screen, ScreenWidth / 2 - 72, ScreenHeight - 200);
			textManager.Start(*screen);
			startButton->Animate(input.mousePos);
			startButton->Draw(*screen);
			if (startButton->Pressed(input.mouseClick, input.mousePos))
			{
				gameState = GameState::GAMEPLAY;
				startSound.PlaySound();
			}
		}
		else if (gameState == GameState::GAMEPLAY)
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

			if (input.sprint) playerTileMapSpeed = 2 * playerTileMapSpeedConst;
			else playerTileMapSpeed = playerTileMapSpeedConst;

			tileMapOffset = tileMap->GetOffset();

			Bounds playerBounds(player->GetBounds() + Bounds{ 4.0f, -4.0f });
			Bounds newPlayerBounds(playerBounds - moveTileMap);
			std::vector<Bounds> tilesBounds(tileMap->NewGetTilesBounds(newPlayerBounds));

			if (!tilesBounds.empty())
			{
				for (auto& collidingTile : tilesBounds)
				{
					if (playerBounds.AboveOrBelow(collidingTile))
						moveTileMap.y = 0;
					if (playerBounds.LeftOrRight(collidingTile))
						moveTileMap.x = 0;
				}
			}

			tileMap->Move(moveTileMap);

			for (auto gem : gems)
			{
				if (gem->CheckIfAlive())
				{
					if (gem->GetBounds(tileMapOffset).NewBoundsCollide(playerBounds))
					{
						gem->SetNotAlive();
						score += 50;
						gemSound.PlaySound();
					}
					gem->Draw(screen, tileMapOffset);
				}
			}

			player->AnimatePlayerDirection(moveTileMap);

			for (auto enemy : enemies)
			{
				i = (i + 1) % totalEnemies;
				enemyPos[i] = enemy->GetPosition(tileMapOffset);
				enemyDist[i] = enemy->GetDistancePlayerEnemy(player, tileMapOffset);
				float* minDist = std::min_element(enemyDist, enemyDist + totalEnemies);
				for (int j = 0; j < totalEnemies; ++j)
				{
					if (enemies[j]->CheckIfAlive())
					{
						if (fabs(static_cast<double>(enemyDist[j]) - *minDist) < 0.01)
							linePos = enemyPos[j];
					}
					else enemyDist[j] = 100000.0f;
				}
				if (enemy->CheckIfAlive() && enemy->CheckIfOnScreen(*screen, tileMapOffset))
				{
					Bounds enemyBounds(enemy->GetBounds(tileMapOffset));

					vec2 enemyMoveBy = 0.0f;
					enemy->SetDirectionPlayerEnemy(player, tileMapOffset);

					float distance = enemy->GetDistancePlayerEnemy(player, tileMapOffset);
					if (distance < TILE_SIZE_FLOAT * tilesAway) //if the player is close enough to the enemy
					{
						enemyMoveBy = enemy->CalculateEnemyMoveBy();
						Bounds newEnemyBounds(enemyBounds + enemyMoveBy);
						std::vector<Bounds> enemyTilesBounds(tileMap->NewGetTilesBounds(newEnemyBounds));

						if (!enemyTilesBounds.empty())
						{
							//enemyMoveBy = 0.0f;
							for (auto& collidingTile : enemyTilesBounds)
							{
								if (enemyBounds.AboveOrBelow(collidingTile))
									enemyMoveBy.y = 0;
								if (enemyBounds.LeftOrRight(collidingTile))
									enemyMoveBy.x = 0;
							}
						}

						if (playerBounds.NewBoundsCollide(enemyBounds))
							enemyMoveBy = 0;

						enemy->Move(enemyMoveBy);
						enemy->AnimateEnemyDirection(enemyMoveBy, tileMapOffset);

						timeSinceDamage += deltaTime;
						timeSinceDmgAnim += deltaTime;
						if (timeSinceDamage > damageDelay)
						{
							if (playerBounds.NewBoundsCollide(enemyBounds))
							{
								timeSinceDamage = 0;
								timeSinceDmgAnim = 0;

								player->HitTaken();
								if (player->GetHitsTaken() < playerHitsToDie)
								{
									int iter = playerHitsToDie - player->GetHitsTaken();
									hearts[iter]->SetFrame(1);
									damageSound.PlaySound();
								}
								else
								{
									int iter = playerHitsToDie - player->GetHitsTaken();
									hearts[iter]->SetFrame(1);
									player->SetNotAlive();
									gameState = GameState::END;
									gameOverSound.PlaySound();
								}
							}
						}
						if (timeSinceDmgAnim < damageDelay)
							player->AnimateDamage();
					}

					enemy->Draw(*screen, tileMapOffset);

					for (auto iter = playerBullets.begin(); iter != playerBullets.end();)
					{
						Bounds bulletBounds((*iter)->GetBounds(tileMapOffset));
						if (bulletBounds.NewBoundsCollide(enemy->GetBounds(tileMapOffset)))
						{
							delete* iter;
							iter = playerBullets.erase(iter);

							enemy->HitTaken();
							if (enemy->GetHitsTaken() >= enemyHitsToDie)
							{
								enemy->SetNotAlive();
								score += 100;
								enemiesDefeated++;
								if (enemiesDefeated >= totalEnemies)
								{
									gameState = GameState::END;
									youWinSound.PlaySound();
								}
							}
						}
						else iter++;
					}
				} 
				
			}
			screen->Line(player->GetPosition().x, player->GetPosition().y, linePos.x, linePos.y, 0xffa5bebb);

			if (player->CheckIfAlive())
				player->Draw(*screen);

			bulletSpawnTime += deltaTime;
			if (input.mouseClick)
			{
				if (bulletSpawnTime * bulletRate > 1)
				{
					bulletSpawnTime = 0;
					vec2 bulletDir = (input.mousePos - player->GetPosition()).normalized();

					playerBullets.push_back(new Bullet(bulletTexture, 1, bulletSpeed, player->GetPosition() - tileMapOffset, bulletDir));
					shotSound.PlaySound();
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

				if (!Bounds(0, { ScreenWidth, ScreenHeight }).BoundsPointCollide(bulletPos) || tileMap->Collides(bulletPos))
				{
					delete* iter;
					iter = playerBullets.erase(iter);
				}
				else iter++;
			}

			for (int i = 0; i < playerHitsToDie; ++i)
			{
				hearts[i]->SetPosition({ static_cast<float>(350 + i * (40/*hearts[i]->GetWidth()*/ + 10)), 30.0f });
				hearts[i]->Draw(*screen);
			}

			textManager.GamePlay(*screen, score, enemiesDefeated, totalEnemies);
		}
		else if (gameState == GameState::END)
		{
		endScreenBG->Draw(screen, 0, 0);
		textManager.End(*screen, score, enemiesDefeated, totalEnemies);
		}
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

		case SDL_SCANCODE_X:
			while (true);

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