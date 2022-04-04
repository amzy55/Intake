#pragma once

#include "Entity.h"
#include "Timer.h"


/// <summary>
/// Bullets have mostly the same properties as an enemy entity but with extra details.
/// </summary>
class Bullet : public Entity
{
public:
	Bullet(Tmpl8::Surface* spriteTexture, int numFrames, float speed, Tmpl8::vec2* TileMapOffset, Tmpl8::vec2 lastPos, const Tmpl8::vec2& position = (0.0f), const Tmpl8::vec2& direction = (0.0f), const Tmpl8::vec2& anchor = (0.5f))
		: Entity(spriteTexture, numFrames, position, anchor)
		, m_direction(direction)
		, m_speed(speed)
		, m_tileMapOffset(TileMapOffset)
		, m_lastPos(lastPos)
	{}

	void Move()
	{
		Tmpl8::vec2 bulletMoveBy = 0;
		m_direction = ((m_lastPos - m_position ) - *m_tileMapOffset).normalized();
		bulletMoveBy = (m_direction * m_speed) * Timer::Get().ElapsedSeconds();
		m_position += bulletMoveBy;
	}

protected:

private:
	Tmpl8::vec2 m_direction = 0;
	Tmpl8::vec2* m_tileMapOffset = nullptr;
	Tmpl8::vec2 m_lastPos = 0;
	float m_speed = 0;
};

