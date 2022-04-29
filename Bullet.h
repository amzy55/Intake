#pragma once

#include "Enemy.h"
#include "Timer.h"


/// <summary>
/// Bullets have mostly the same properties as an enemy entity but with extra details.
/// </summary>
class Bullet : public Enemy
{
public:
	Bullet(Tmpl8::Surface* spriteTexture, int numFrames, float speed, const Tmpl8::vec2& position = (0.0f), const Tmpl8::vec2& direction = (0.0f))
		: Enemy(spriteTexture, numFrames, speed, position, direction)
	{}

	void Move()
	{
		Tmpl8::vec2 bulletMoveBy = 0.0f;
		bulletMoveBy = (m_direction * m_speed) * Timer::Get().ElapsedSeconds();
		m_position += bulletMoveBy;
	}

protected:

private:
};

