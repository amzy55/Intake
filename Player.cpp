#include "Player.h"

Player::Player(Tmpl8::Surface* spriteTexture, int numFrames, const Tmpl8::vec2& position)
	: Entity(spriteTexture, numFrames, position)
{};

Bounds Player::NewPlayerBounds(Tmpl8::vec2 moveTileMap)
{
	CalculateBounds();
	return Bounds(GetBounds() - moveTileMap);
}

void Player::AnimatePlayerDirection(Tmpl8::vec2 moveTileMap)
{
	if (NewPlayerBounds(moveTileMap).MinX() > GetBounds().MinX()) SetFrame(1);
	else if (NewPlayerBounds(moveTileMap).MinX() < GetBounds().MinX()) SetFrame(0);
}

void Player::AnimateDamage()
{
	if (m_frame == 0) SetFrame(2);
	else if (m_frame == 1) SetFrame(3);
}