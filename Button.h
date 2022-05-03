#pragma once

#include "Entity.h"

class Button : public Entity
{
public:
	Button(Tmpl8::Surface* spriteTexture, int numFrames, const Tmpl8::vec2& position = (0.0f))
		: Entity(spriteTexture, numFrames, position)
	{}

	bool MouseHover(Tmpl8::vec2 mousePosInput)
	{
		return GetBounds().BoundsPointCollide(mousePosInput);
	}

	bool Pressed(bool mouseInput)
	{
		return mouseInput;
	}

	void Animate(Tmpl8::vec2 mousePosInput)
	{
		if (MouseHover(mousePosInput))
			SetFrame(0);
		else SetFrame(1);
	}

protected:

private:

};

