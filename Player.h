#pragma once

#include "Entity.h"

/// <summary>
/// The player sprite must have 4 frames.
/// </summary>
class Player : public Entity
{
public:
	Player(Tmpl8::Surface* spriteTexture, int numFrames, const Tmpl8::vec2& position = (0.0f));

	Bounds NewPlayerBounds(Tmpl8::vec2 moveTileMap);

	/// <summary>
	/// Make the player appear he is moving left or right.
	/// </summary>
	/// <param name="moveTileMap"></param>
	void AnimatePlayerDirection(Tmpl8::vec2 moveTileMap);

	void AnimateDamage();

	void AddToTimer(float deltaTime);

protected:

private:
	float m_timerHurt = 2.0f;
	float m_damageDelay = 2.0f;

};

