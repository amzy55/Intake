#pragma once

#include "Entity.h"

class Enemy : public Entity
{
public:
	Enemy(Tmpl8::Surface* spriteTexture, int numFrames, const Tmpl8::vec2& position = (0.0f), const Tmpl8::vec2& anchor = (0.5f))
		: Entity(spriteTexture, numFrames, position, anchor)
	{}

protected:

private:

};

