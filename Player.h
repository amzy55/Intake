#pragma once

#include "Entity.h"

class Player : public Entity
{
public:
	Player(Tmpl8::Surface* spriteTexture, int numFrames, const Tmpl8::vec2& position = (0.0f))
		: Entity(spriteTexture, numFrames, position)
	{};

	Bounds NewPlayerBounds(Tmpl8::vec2 moveTileMap)
	{
		CalculateBounds();
		return Bounds(GetBounds() - moveTileMap);
	}

	/// <summary>
	/// Make the player appear he is moving left or right.
	/// </summary>
	/// <param name="moveTileMap"></param>
	void AnimatePlayerDirection(Tmpl8::vec2 moveTileMap)
	{
		//if (maxframes > 1)
		if (NewPlayerBounds(moveTileMap).MinX() > GetBounds().MinX()) SetFrame(1);
		else if (NewPlayerBounds(moveTileMap).MinX() < GetBounds().MinX()) SetFrame(0);
	}

	void AnimateDamage()
	{
		if (m_frame == 0) SetFrame(2);
		else if (m_frame == 1) SetFrame(3);
	}

protected:

private:

};

