#pragma once

#pragma once

#include "Entity.h"

class Button : public Entity
{
public:
	Button(Tmpl8::Surface* spriteTexture, int maxFrames, const Tmpl8::vec2& position = (0.0f));

	bool MouseHover(Tmpl8::vec2 mousePos);

	bool Pressed(bool mouseClick, Tmpl8::vec2 mousePos);

	void Animate(Tmpl8::vec2 mousePos);

protected:

private:
	int m_maxFrames = 1;

};

