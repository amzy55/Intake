#pragma once

#include "Entity.h"

class Button : public Entity
{
public:
	Button(Tmpl8::Surface* spriteTexture, int maxFrames, const Tmpl8::vec2& position = (0.0f))
		: Entity(spriteTexture, maxFrames, position)
	{
		m_maxFrames = maxFrames;
	}

	bool MouseHover(Tmpl8::vec2 mousePos)
	{
		return GetBounds().BoundsPointCollide(mousePos);
	}

	bool Pressed(bool mouseClick, Tmpl8::vec2 mousePos)
	{
		if (MouseHover(mousePos))
			return mouseClick;
	}

	void Animate(Tmpl8::vec2 mousePos)
	{
		if (m_maxFrames > 1)
		{
			if (MouseHover(mousePos))
				SetFrame(1);
			else SetFrame(0);
		}
	}

protected:

private:
	int m_maxFrames = 1;

};

