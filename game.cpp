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

	static const Pixel BarColor[2] = { 0xbf42f5, 0x36c75c };


	static Tile* I = new Tile{ false, 0, 0, TILE_SIZE_INT }; //snow
	static Tile* O = new Tile{ true, 1, 0, TILE_SIZE_INT }; //rock
	static Tile* RED_TILE = new Tile{ true, 2, 0, TILE_SIZE_INT };

	const Tile* map []= {
		#include "snowMap.txt"
	};

	UIText Instructions("Defeat all enemies to win!", { 0.0f, 260.0f }, 0xff000000, 3);
	UIText ScoreText("Score:", 20.0f, 0x000000, 3, 0);
	UIText TimeText("Time: ", {20.0f, 512.0f - 40.0f}, 0x000000, 3, 0, 0, ":");
	UIText EnemiesDefeatedText("Enemies:", { 800.0f - 250.0f, 20.0f }, 0x000000, 3, 0, 0, "/");
	UIText GameOver("Game over!", { 0.0f, 90.0f }, 0x000000, 8);
	UIText YouWin("You win!", { 0.0f, 90.0f }, 0x000000, 9);

	Game::Game()
		: screen(nullptr)
	{
		assert(theGame == nullptr);

		theGame = this;

		tileMap = new TileMap("assets/TilesTexture78.png");
		tileMap->SetTiles(map, 60); //second param = number of tiles in width
		vec2 tileMapSize = tileMap->GetSizeInPixels();
		tileMap->SetOffset({-TILE_SIZE_INT * 4, -(tileMapSize.y - ScreenHeight - TILE_SIZE_INT * 2) }); //Start position

		playerTexture = new Surface("assets/playerIdea4frames.png");
		player = new Player(playerTexture, 4, { ScreenWidth / 2, ScreenHeight / 2 });

		for (int i = 0; i < setEnemies; i++)
			enemies.push_back(new Enemy(playerTexture, 4, enemySpeed, setEnemyPos[i]));

		std::vector<vec2> enemyPositions = tileMap->GetNonCollidingPos();
		std::vector<vec2> gemPositions = tileMap->GetNonCollidingPos();
		std::random_shuffle(enemyPositions.begin(), enemyPositions.end());
		std::random_shuffle(gemPositions.begin(), gemPositions.end());

		for (int i = 0; i < randomEnemies; i++)
			enemies.push_back(new Enemy(playerTexture, 4, enemySpeed, enemyPositions[i]));
		
		bulletTexture = new Surface("assets/snowballBullet.png");

		heartsTexture = new Surface("assets/hearts.png");
		for (int i = 0; i < playerHitsToDie; ++i)
			hearts.push_back(new Entity(heartsTexture, 2));

		startButtonTexture = new Surface("assets/startButtonUgly.png");
		startButton = new Button(startButtonTexture, 2, { ScreenWidth / 2, ScreenHeight / 2 - 90.0f});
		
		gemTexture = new Surface("assets/greenGem.png");
		for (int i = 0; i < gemCount; ++i)
			gems.push_back(new StaticObject(gemTexture, 1, gemPositions[i]));

		hiSound.PlaySound();
	}

	Game::~Game()
	{
		if (tileMap != nullptr)
			delete tileMap;

		delete I;
		delete O;

		delete playerTexture;
		delete player;
		for (Enemy* enemy : enemies)
			delete enemy;
		delete bulletTexture;
		delete heartsTexture;
		for (Entity* heart : hearts)
			delete heart;
		for (Bullet* bullet : playerBullets)
			delete bullet;

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
		if (GameState == START)
		{
			screen->Clear(0xffe7f4f5);
			Instructions.DrawTextCentre(*screen);
			startButton->Animate(input.mousePos);
			startButton->Draw(*screen);
			if (startButton->Pressed(input.mouseClick, input.mousePos))
			{
				GameState = GAMEPLAY;
				startSound.PlaySound();
			}
		}
		else if (GameState == GAMEPLAY)
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

			Pixel playerBarColor = BarColor[1];

			Bounds playerBounds(player->GetBounds() + Bounds{ 4.0f, -4.0f });
			Bounds newPlayerBounds(playerBounds - moveTileMap);
			std::vector<Bounds> tilesBounds(tileMap->NewGetTilesBounds(newPlayerBounds));

			if (!tilesBounds.empty())
			{
				playerBarColor = BarColor[0];

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
					if (playerBounds.NewBoundsCollide(gem->GetBounds(tileMapOffset)))
					{
						gem->SetNotAlive();
						score += 50;
					}
					gem->Draw(*screen, tileMapOffset);
				}
			}

			player->AnimatePlayerDirection(moveTileMap);

			Pixel enemyBarColor = BarColor[1];
			for (auto& enemy : enemies)
			{
				if (enemy->CheckIfAlive() && enemy->CheckIfOnScreen(*screen, tileMapOffset))
				{
					Bounds enemyBounds(enemy->GetBounds(tileMapOffset));

					//if (tileMap->NewCollides(enemyBounds))
					//	enemyBarColor = BarColor[0];

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

								//screen->Box(collidingTile.MinX(), collidingTile.MinY(), collidingTile.MaxX(), collidingTile.MaxY(), 0xffff0000);
							}
						}
						screen->Box(enemyBounds.MinX(), enemyBounds.MinY(), enemyBounds.MaxX(), enemyBounds.MaxY(), 0xff0000ff);

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
									GameState = END;
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
								linePos = { ScreenWidth / 2.0f + 1.0f, ScreenHeight / 2.0f + 1.0f };
								if (enemiesDefeated >= totalEnemies)
								{
									GameState = END;
									youWinSound.PlaySound();
								}
							}
						}
						else iter++;
					}
				} 
				//if (enemy->CheckIfAlive())
				//	screen->Line(player->GetPosition().x, player->GetPosition().y, enemy->GetPosition(tileMapOffset).x, enemy->GetPosition(tileMapOffset).y, 0xffff0000);
				//screen->Box(enemy->GetBounds(tileMapOffset).MinX(), enemy->GetBounds(tileMapOffset).MinY(), enemy->GetBounds(tileMapOffset).MaxX(), enemy->GetBounds(tileMapOffset).MaxY(), 0xff0000ff);
				//screen->Box(enemy->NewEnemyBounds(tileMapOffset).MinX(), enemy->NewEnemyBounds(tileMapOffset).MinY(), enemy->NewEnemyBounds(tileMapOffset).MaxX(), enemy->NewEnemyBounds(tileMapOffset).MaxY(), 0xffff0000);
				i = (i + 1) % totalEnemies;
				enemyPos[i] = enemy->GetPosition(tileMapOffset);
				enemyDist[i] = enemy->GetDistancePlayerEnemy(player, tileMapOffset);
				float* minDist = std::min_element(enemyDist, enemyDist + totalEnemies);
				for (int j = 0; j < totalEnemies; ++j)
				{
					if (enemies[j]->CheckIfAlive())
						if (enemyDist[j] == *minDist)
							linePos = enemyPos[j];
				}
				screen->Line(player->GetPosition().x, player->GetPosition().y, linePos.x, linePos.y, 0xffa5bebb);
			}

			//if (!tilesBounds.empty())
			//	for (auto& collidingTile : tilesBounds)
			//		screen->Box(collidingTile.MinX(), collidingTile.MinY(), collidingTile.MaxX(), collidingTile.MaxY(), 0xffff0000);


			if (player->CheckIfAlive())
				player->Draw(*screen);

			//screen->Bar(playerBounds.MinX(), playerBounds.MinY(), playerBounds.MaxX(), playerBounds.MaxY(), playerBarColor);
			screen->Box(playerBounds.MinX(), playerBounds.MinY(), playerBounds.MaxX(), playerBounds.MaxY(), 0);
			//screen->Box(newPlayerBounds.MinX(), newPlayerBounds.MinY(), newPlayerBounds.MaxX(), newPlayerBounds.MaxY(), 0);
			//screen->Box(enemyBounds.MinX(), enemyBounds.MinY(), enemyBounds.MaxX(), enemyBounds.MaxY(), 0);

			int tileMapCenterX = static_cast<int>(tileMap->GetSizeInPixels().x / 2 + tileMapOffset.x);
			int tileMapCenterY = static_cast<int>(tileMap->GetSizeInPixels().y / 2 + tileMapOffset.y);
			screen->Bar(tileMapCenterX - 5, tileMapCenterY - 5, tileMapCenterX + 5, tileMapCenterY + 5, 0xffff0000);

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

				if (bulletPos.x < 0 || bulletPos.y < 0 || bulletPos.x > ScreenWidth || bulletPos.y > ScreenHeight || tileMap->Collides(bulletPos))
				{
					delete* iter;
					iter = playerBullets.erase(iter);
				}
				else iter++;
			}

			//for (int x = 0; x < 6; x++)
			//	for (int y = 0; y < 6; y++)
			//	{
			//		Tmpl8::vec2 min = { static_cast<float>(x * TILE_SIZE_INT), static_cast<float>(y * TILE_SIZE_INT) };
			//		Tmpl8::vec2 max = { TILE_SIZE_FLOAT + min.x, TILE_SIZE_FLOAT + min.y };
			//		Bounds tileBounds(Bounds(min, max) + tileMapOffset);
			//		if (tileMap->GetTile(x, y)->isBlocking)
			//			screen->Box(tileBounds.MinX(), tileBounds.MinY(), tileBounds.MaxX(), tileBounds.MaxY(), 0xffff0000);
			//	}

			ScoreText.Draw(*screen, score);
			TimeText.Draw(*screen, static_cast<int>(Timer::Get().TotalTimeSeconds()) / 60, static_cast<int>(Timer::Get().TotalTimeSeconds()) % 60);
			EnemiesDefeatedText.Draw(*screen, enemiesDefeated, totalEnemies);

			for (int i = 0; i < playerHitsToDie; ++i)
			{
				hearts[i]->SetPosition({ static_cast<float>(350 + i * (40/*hearts[i]->GetWidth()*/ + 10)), 30.0f });
				hearts[i]->Draw(*screen);
			}
		}
		else if (GameState == END)
		{
		screen->Clear(0xffe7f4f5);

		if (enemiesDefeated < totalEnemies)
			GameOver.DrawTextCentre(*screen);
		else YouWin.DrawTextCentre(*screen);

		ScoreText.ChangePosition({210.0f, 256.0f - 60.0f + 30.0f });
		EnemiesDefeatedText.ChangePosition({ 210.0f, 256.0f + 30.0f});
		TimeText.ChangePosition({ 210.0f, 256.0f + 60.0f + 30.0f });

		ScoreText.ChangeSize(5);
		EnemiesDefeatedText.ChangeSize(5);
		TimeText.ChangeSize(5);

		ScoreText.Draw(*screen, score);
		EnemiesDefeatedText.Draw(*screen, enemiesDefeated, totalEnemies);
		TimeText.Draw(*screen, static_cast<int>(Timer::Get().TotalTimeSeconds()) / 60, static_cast<int>(Timer::Get().TotalTimeSeconds()) % 60);
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