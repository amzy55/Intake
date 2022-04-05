#pragma once

#include "Entity.h"
#include "Timer.h"


/// <summary>
/// Bullets have mostly the same properties as an enemy entity but with extra details.
/// </summary>
class Bullet : public Entity
{
public:
	Bullet(Tmpl8::Surface* spriteTexture, int numFrames, float speed, const Tmpl8::vec2& position = (0.0f), const Tmpl8::vec2& direction = (0.0f), const Tmpl8::vec2& anchor = (0.5f))
		: Entity(spriteTexture, numFrames, position, anchor)
		, m_direction(direction)
		, m_speed(speed)
	{}

	void Move()
	{
		Tmpl8::vec2 bulletMoveBy = 0;
		bulletMoveBy = (m_direction * m_speed) * Timer::Get().ElapsedSeconds();
		m_position += bulletMoveBy;
	}

protected:

private:
	Tmpl8::vec2 m_direction = 0;
	float m_speed = 0;
};

