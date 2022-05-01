#pragma once

#include "Entity.h"

class Enemy : public Entity
{
public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="spriteTexture">The surface that contains the sprite texture for this entity.</param>
	/// <param name="numFrames">The number of frames in the sprite texture.</param>
	/// <param name="speed">enemySpeed</param>
	/// <param name="position">The position of the entity in screen space.</param>
	/// <param name="direction">Direction that will get calculated.</param>
	/// <param name="anchor">An anchor to set the position to be the middle of the sprite for 0.5f, instead of top left corner.</param>
	Enemy(Tmpl8::Surface* spriteTexture, int numFrames, float speed, const Tmpl8::vec2& position = (0.0f), const Tmpl8::vec2& direction = (0.0f))
		: Entity(spriteTexture, numFrames, position)
		, m_direction(direction)
		, m_speed(speed)
	{}

	/// <summary>
	/// Draw this entity to the screen.
	/// </summary>
	/// <param name="screen">- The surface to draw this entity to.</param>
	/// <param name="tileMapOffset"></param>
	virtual void Draw(Tmpl8::Surface& screen, Tmpl8::vec2& tileMapOffset);

	/// <summary>
	/// Get the screen-space position of the entity.
	/// </summary>
	/// <returns>The screen-space position of the entity.</returns>
	const Tmpl8::vec2 GetPosition(Tmpl8::vec2 tileMapOffset)
	{
		return m_position + tileMapOffset;
	}

	/// <summary>
	/// Get the bounds to use for collision.
	/// </summary>
	/// <returns></returns>
	Bounds GetBounds(Tmpl8::vec2 tileMapOffset)
	{
		CalculateBounds();
		return m_bounds.Add(tileMapOffset);
	}

	/// <summary>
	/// Distance between the player and an entity.
	/// </summary>
	/// <param name="enemy">- The player.</param>
	/// <param name="TileMapOffset">- Tilemap offset to be taken into consideration for the entities to be in the same space (screen space).
	/// <returns>The distance between them.</returns>
	float GetDistancePlayerEnemy(Entity* player, Tmpl8::vec2& tileMapOffset);
	
	/// <summary>
	/// Get the normalized direction needed to move an entity.
	/// </summary>
	/// <param name="player"></param>
	/// <param name="TileMapOffset"></param>
	/// <returns></returns>
	void SetDirectionPlayerEnemy(Entity* player, Tmpl8::vec2& tileMapOffset);

	/// <summary>
	/// Used for the implementation with one enemy.
	/// </summary>
	/// <param name="player"></param>
	/// <param name="tileMapOffset"></param>
	/// <returns></returns>
	Tmpl8::vec2 GetDirectionPlayerEnemy(Entity* player, Tmpl8::vec2& tileMapOffset);

	/// <summary>
	/// Move an entity.
	/// </summary>
	/// <param name="moveBy">- How much to move the entity by.</param>
	void Move(Tmpl8::vec2& moveBy)
	{
		m_position += moveBy;
		CalculateBounds();
	}

	/// <summary>
	/// Calculates how much the enemy moves.
	/// </summary>
	/// <returns></returns>
	Tmpl8::vec2 CalculateEnemyMoveBy();

	/// <summary>
	/// Checks to see if an entity is on screen.
	/// </summary>
	/// <param name="screen"></param>
	/// <returns></returns>
	bool CheckIfOnScreen(Tmpl8::Surface& screen, Tmpl8::vec2& tileMapOffset)
	{
		Bounds screenBounds({ 0, 0 }, { static_cast<float>(screen.GetWidth()), static_cast<float>(screen.GetHeight()) });

		if (GetBounds(tileMapOffset).NewBoundsCollide(screenBounds))
			return true;

		return false;
	}

protected:
	Tmpl8::vec2 m_direction = 0;
	float m_speed = 0;

private:
};

