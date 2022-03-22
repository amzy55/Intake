#pragma once

#include "Bounds.h"

class Entity;

/// <summary>
/// A controller is an interface type attached to an Entity (like a player or Enemy entity).
/// </summary>
class EntityController
{
public:

	// The minimum requirement of any interface is a virtual destructor.
	virtual ~EntityController() = default;

	/// <summary>
	/// Update the entity controller.
	/// </summary>
	virtual void Update(Entity& entity) = 0;

	/*void SetBounds(Bounds& bounds)
	{
		m_bounds = bounds;
	}

	const Bounds GetBounds()
	{
		return m_bounds;
	}*/

	/*float Distance(Entity* player, Entity* enemy, Tmpl8::vec2 TileMapOffset)
	{
		Tmpl8::vec2 playerPos = player->GetPosition();
		Tmpl8::vec2 enemyPos = enemy->GetPosition() + TileMapOffset;
		float distancePlayerEnemy = (playerPos - enemyPos).length();

		return distancePlayerEnemy;
	}*/

protected:


private:

	//Bounds m_bounds;
};