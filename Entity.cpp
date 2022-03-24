#include "Entity.h"

Entity::Entity(Tmpl8::Surface* spriteTexture, int numFrames, const Tmpl8::vec2& position, const Tmpl8::vec2& anchor)
	: m_sprite(spriteTexture, numFrames)
	, m_position(position)
	, m_anchor(anchor)
{}

void Entity::Draw(Tmpl8::Surface& screen, float xoffset, float yoffset)
{
	float x = (m_position.x) - (m_anchor.x) * m_sprite.GetWidth();
	float y = (m_position.y) - (m_anchor.y) * m_sprite.GetHeight();


	m_sprite.Draw(&screen, static_cast<int>(x + xoffset), static_cast<int>(y + yoffset));
}

//float Entity::DistancePlayerEnemy(Entity* enemy, Tmpl8::vec2 TileMapOffset)
//{
//	//float distancePlayerEnemy = sqrtf(powf(player->GetPosition().x - (enemy->GetPosition().x - TileMapOffset.x), 2) + powf(player->GetPosition().y - (enemy->GetPosition().y - TileMapOffset.y), 2));
//
//	Tmpl8::vec2 playerPos = m_position;
//	Tmpl8::vec2 enemyPos = enemy->GetPosition(TileMapOffset);
//	float distancePlayerEnemy = (playerPos - enemyPos).length();
//
//	return distancePlayerEnemy;
//}

float Entity::GetDistance(Entity* other, Tmpl8::vec2 TileMapOffset)
{
	//float distancePlayerEnemy = sqrtf(powf(other->GetPosition().x - (enemy->GetPosition().x - TileMapOffset.x), 2) + powf(other->GetPosition().y - (enemy->GetPosition().y - TileMapOffset.y), 2));

	Tmpl8::vec2 otherPos = other->GetPosition();
	Tmpl8::vec2 position = m_position + TileMapOffset;
	float distancePlayerEnemy = (otherPos - position).length();

	return distancePlayerEnemy;
}

Tmpl8::vec2 Entity::GetVelocity(Entity* other, Tmpl8::vec2 TileMapOffset)
{
	Tmpl8::vec2 otherPos = other->GetPosition();
	Tmpl8::vec2 position = m_position + TileMapOffset;

	Tmpl8::vec2 velocity = otherPos - position;

	return velocity;
}
