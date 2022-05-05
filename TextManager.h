#pragma once

#include "UIText.h"

class TextManager
{
public:
	TextManager()
	{};

	void Start(Tmpl8::Surface& screen);

	void GamePlay(Tmpl8::Surface& screen, int score, int enemiesDefeated, int totalEnemies);

	void End(Tmpl8::Surface& screen, int score, int enemiesDefeated, int totalEnemies);

protected:

private:
	UIText m_instructions = UIText("Defeat all enemies to win!", { 0.0f, 260.0f }, 0xff000000, 3);
	UIText m_scoreText = UIText("Score:", 20.0f, 0x000000, 3, 0);
	UIText m_timeText = UIText("Time: ", { 20.0f, 512.0f - 40.0f }, 0x000000, 3, 0, 0, ":");
	UIText m_enemiesDefeatedText = UIText("Enemies:", { 800.0f - 250.0f, 20.0f }, 0x000000, 3, 0, 0, "/");
	UIText m_gameOver = UIText("Game over!", { 0.0f, 120.0f }, 0x000000, 8);
	UIText m_youWin = UIText("You win!", { 0.0f, 120.0f }, 0x000000, 9);

};
