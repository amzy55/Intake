#include "TextManager.h"
#include "Timer.h"

void TextManager::Start(Tmpl8::Surface& screen)
{
	m_instructions.DrawTextCentre(screen);
}

void TextManager::GamePlay(Tmpl8::Surface& screen, int score, int enemiesDefeated, int totalEnemies)
{
	m_scoreText.Draw(screen, score);
	m_timeText.Draw(screen, static_cast<int>(Timer::Get().TotalTimeSeconds()) / 60, static_cast<int>(Timer::Get().TotalTimeSeconds()) % 60);
	m_enemiesDefeatedText.Draw(screen, enemiesDefeated, totalEnemies);

}

void TextManager::End(Tmpl8::Surface& screen, int score, int enemiesDefeated, int totalEnemies)
{
	if (enemiesDefeated < totalEnemies)
		m_gameOver.DrawTextCentre(screen);
	else m_youWin.DrawTextCentre(screen);

	m_scoreText.ChangePosition({ 210.0f, 256.0f - 60.0f + 30.0f });
	m_enemiesDefeatedText.ChangePosition({ 210.0f, 256.0f + 30.0f });
	m_timeText.ChangePosition({ 210.0f, 256.0f + 60.0f + 30.0f });

	m_scoreText.ChangeSize(5);
	m_enemiesDefeatedText.ChangeSize(5);
	m_timeText.ChangeSize(5);

	m_scoreText.Draw(screen, score);
	m_enemiesDefeatedText.Draw(screen, enemiesDefeated, totalEnemies);
	m_timeText.Draw(screen, static_cast<int>(Timer::Get().TotalTimeSeconds()) / 60, static_cast<int>(Timer::Get().TotalTimeSeconds()) % 60);
}
