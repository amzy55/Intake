#pragma once

#include "Entity.h"

/// <summary>
/// Bullets have mostly the same properties as an enemy entity but with extra details.
/// </summary>
class Bullet : public Entity
{
public:
	Bullet(Tmpl8::Surface* spriteTexture, int numFrames, const Tmpl8::vec2& direction = (0.0f), const Tmpl8::vec2& position = (0.0f), const Tmpl8::vec2& anchor = (0.5f))
		: Entity(spriteTexture, numFrames, position, anchor)
		, m_direction(direction)
	{}

protected:

private:
	Tmpl8::vec2 m_direction;
};

