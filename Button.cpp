#include "Button.h"

Button::Button(Tmpl8::Surface* spriteTexture, int maxFrames, const Tmpl8::vec2& position)
	: Entity(spriteTexture, maxFrames, position)
{
	m_maxFrames = maxFrames;
}

bool Button::MouseHover(Tmpl8::vec2 mousePos)
{
	return GetBounds().BoundsPointCollide(mousePos);
}

bool Button::Pressed(bool mouseClick, Tmpl8::vec2 mousePos)
{
	if (MouseHover(mousePos))
		return mouseClick;
}

void Button::Animate(Tmpl8::vec2 mousePos)
{
	if (m_maxFrames > 1)
	{
		if (MouseHover(mousePos))
			SetFrame(1);
		else SetFrame(0);
	}
}
