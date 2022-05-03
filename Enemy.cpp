#include "Enemy.h"
#include "Timer.h"

void Enemy::Draw(Tmpl8::Surface& screen, Tmpl8::vec2& tileMapOffset)
{
	float x = m_position.x - m_anchor.x * m_sprite.GetWidth();
	float y = m_position.y - m_anchor.y * m_sprite.GetHeight();

	m_sprite.SetFrame(m_frame);

	m_sprite.Draw(&screen, static_cast<int>(x + tileMapOffset.x), static_cast<int>(y + tileMapOffset.y));
}

float Enemy::GetDistancePlayerEnemy(Entity* player, Tmpl8::vec2& tileMapOffset)
{
	Tmpl8::vec2 playerPos = player->GetPosition();
	Tmpl8::vec2 enemyPosition = m_position + tileMapOffset;
	float distancePlayerEnemy = (playerPos - enemyPosition).length();

	return distancePlayerEnemy;
}

void Enemy::SetDirectionPlayerEnemy(Entity* player, Tmpl8::vec2& tileMapOffset)
{
	Tmpl8::vec2 playerPos = player->GetPosition();
	Tmpl8::vec2 enemyPosition = m_position + tileMapOffset;

	Tmpl8::vec2 direction = playerPos - enemyPosition;

	m_direction =  direction.normalized();
}

Tmpl8::vec2 Enemy::GetDirectionPlayerEnemy(Entity* player, Tmpl8::vec2& tileMapOffset)
{
	Tmpl8::vec2 playerPos = player->GetPosition();
	Tmpl8::vec2 enemyPosition = m_position + tileMapOffset;

	Tmpl8::vec2 direction = playerPos - enemyPosition;

	return direction.normalized();
}

Tmpl8::vec2 Enemy::CalculateEnemyMoveBy()
{
	return (m_direction * m_speed) * Timer::Get().ElapsedSeconds();
}

bool Enemy::CheckIfOnScreen(Tmpl8::Surface& screen, Tmpl8::vec2& tileMapOffset)
{
	Bounds screenBounds({ 0, 0 }, { static_cast<float>(screen.GetWidth()), static_cast<float>(screen.GetHeight()) });

	if (GetBounds(tileMapOffset).NewBoundsCollide(screenBounds))
		return true;

	return false;
}

void Enemy::AnimateEnemyDirection(Tmpl8::vec2 enemyMoveBy, Tmpl8::vec2 tileMapOffset)
{
	if (NewEnemyBounds(tileMapOffset).MinX() > GetBounds(tileMapOffset).MinX()) SetFrame(1);
	else if (NewEnemyBounds(tileMapOffset).MinX() < GetBounds(tileMapOffset).MinX()) SetFrame(0);
}
