#include "Entity.h"

Entity::Entity(Tmpl8::Surface* spriteTexture, int numFrames, const Tmpl8::vec2& position, const Tmpl8::vec2& anchor)
	: m_sprite(spriteTexture, numFrames)
	, m_position(position)
	, m_anchor(anchor)
{
	CalculateBounds();
}

void Entity::Draw(Tmpl8::Surface& screen)
{
	float posx = m_position.x - m_anchor.x * m_sprite.GetWidth();
	float posy = m_position.y - m_anchor.y * m_sprite.GetHeight();

	m_sprite.Draw(&screen, posx, posy);
}

void Entity::CalculateBounds()
{
	float xmin = m_position.x - m_anchor.x * (m_sprite.GetWidth() / m_frame);
	float ymin = m_position.y - m_anchor.y * (m_sprite.GetHeight() / m_frame);
	Tmpl8::vec2 min = { xmin, ymin };

	float xmax = m_position.x + m_anchor.x * m_sprite.GetWidth();
	float ymax = m_position.y + m_anchor.y * m_sprite.GetHeight();
	Tmpl8::vec2 max = { xmax, ymax };

	m_bounds = { min, max };
}
