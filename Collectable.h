#pragma once

#include "Entity.h"

class Collectable :  public Entity
{
public:
	Collectable(Tmpl8::Surface* spriteTexture, int numFrames, const Tmpl8::vec2& position = (0.0f))
		: Entity(spriteTexture, numFrames, position)
	{};

	void Draw(Tmpl8::Surface screen, Tmpl8::vec2& tileMapOffset)
	{
		float x = m_position.x - m_anchor.x * m_sprite.GetWidth();
		float y = m_position.y - m_anchor.y * m_sprite.GetHeight();

		m_sprite.SetFrame(m_frame);

		m_sprite.Draw(&screen, static_cast<int>(x + tileMapOffset.x), static_cast<int>(y + tileMapOffset.y));
	}

	Bounds GetBounds(Tmpl8::vec2 tileMapOffset)
	{
		CalculateBounds();
		return m_bounds + tileMapOffset;
	}
protected:

private:

};

