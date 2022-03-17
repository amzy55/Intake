#include "Entity.h"
#include "EntityController.h"

Entity::Entity(Tmpl8::Surface* spriteTexture, int numFrames, const Tmpl8::vec2& position, const Tmpl8::vec2& anchor)
	: m_sprite(spriteTexture, numFrames)
	, m_position(position)
	, m_anchor(anchor)
{}

void Entity::Draw(Tmpl8::Surface& screen, float xoffset, float yoffset)
{
	float x = (m_position.x) - (m_anchor.x) * m_sprite.GetWidth();
	float y = (m_position.y) - (m_anchor.y) * m_sprite.GetHeight();


	m_sprite.Draw(&screen, x + xoffset, y + yoffset);
}
