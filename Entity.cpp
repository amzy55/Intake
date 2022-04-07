#include "Entity.h"

Entity::Entity(Tmpl8::Surface* spriteTexture, int numFrames, const Tmpl8::vec2& position, const Tmpl8::vec2& anchor)
	: m_sprite(spriteTexture, numFrames)
	, m_position(position)
	, m_anchor(anchor)
{
	CalculateBounds();
}

void Entity::Draw(Tmpl8::Surface& screen, Tmpl8::vec2 offset)
{
	float x = m_position.x - m_anchor.x * m_sprite.GetWidth();
	float y = m_position.y - m_anchor.y * m_sprite.GetHeight();


	m_sprite.Draw(&screen, static_cast<int>(x + offset.x), static_cast<int>(y + offset.y));
}

float Entity::GetDistancePlayerEnemy(Entity* other, Tmpl8::vec2 TileMapOffset)
{
	//float distancePlayerEnemy = sqrtf(powf(other->GetPosition().x - (enemy->GetPosition().x - TileMapOffset.x), 2) + powf(other->GetPosition().y - (enemy->GetPosition().y - TileMapOffset.y), 2));

	Tmpl8::vec2 otherPos = other->GetPosition();
	Tmpl8::vec2 position = m_position + TileMapOffset;
	float distancePlayerEnemy = (otherPos - position).length();

	return distancePlayerEnemy;
}

Tmpl8::vec2 Entity::GetDirectionPlayerEnemy(Entity* other, Tmpl8::vec2 TileMapOffset)
{
	Tmpl8::vec2 otherPos = other->GetPosition();
	Tmpl8::vec2 position = m_position + TileMapOffset;

	Tmpl8::vec2 direction = otherPos - position;

	return direction.normalized();
}

void Entity::CalculateBounds()
{
	float xmin = m_position.x - m_anchor.x * m_sprite.GetWidth();
	float ymin = m_position.y - m_anchor.y * m_sprite.GetHeight();
	Tmpl8::vec2 min = { xmin, ymin };

	float xmax = m_position.x + m_anchor.x * m_sprite.GetWidth();
	float ymax = m_position.y + m_anchor.y * m_sprite.GetHeight();
	Tmpl8::vec2 max = { xmax, ymax };

	m_bounds = { min, max };
}
