#pragma once

#include "Bounds.h"
#include "template.h"
#include "surface.h"

/// <summary>
/// An Entity is an object that can appear in the game (like the player entity or an enemy character).
/// </summary>
class Entity
{
public:
	/// <summary>
	/// Create a new entity.
	/// </summary>
	/// <param name="spriteTexture">The surface that contains the sprite texture for this entity.</param>
	/// <param name="numFrames">The number of frames in the sprite texture.</param>
	/// <param name="position">The position of the entity in screen space.</param>
	/// <param name="anchor">An anchor to set the position to be the middle of the sprite for 0.5f, instead of top left corner.</param>
	Entity(Tmpl8::Surface* spriteTexture, int numFrames, const Tmpl8::vec2& position = (0.0f), const Tmpl8::vec2& anchor = (0.5f));

	/// <summary>
	/// Draw this entity to the screen.
	/// </summary>
	/// <param name="screen">- The surface to draw this entity to.</param>
	virtual void Draw(Tmpl8::Surface& screen);

	/// <summary>
	/// Set the screen-space position of the entity.
	/// </summary>
	/// <param name="position">- The screen-space position to set this entity to.</param>
	void SetPosition(const Tmpl8::vec2 position)
	{
		m_position = position;
	}

	/// <summary>
	/// Get the screen-space position of the entity.
	/// </summary>
	/// <returns>The screen-space position of the entity.</returns>
	const Tmpl8::vec2 GetPosition()
	{
		return m_position;
	}

	/// <summary>
	/// The (normalized) anchor-point of the sprite determines the
	/// position of the entity relative to the position of the sprite.
	/// An anchor-point of (0, 0) makes the entity's position the top-left
	/// corner of the sprite. An anchor-point of (0.5, 0.5) makes the entity's
	/// position the center of the sprite, and an anchor point of (1, 1) makes
	/// the entity's position the bottom-right corner of the sprite.
	/// </summary>
	/// <param name="anchor">- The normalized anchor-point of the sprite.</param>
	void SetAnchor(const Tmpl8::vec2& anchor)
	{
		m_anchor = anchor;
	}

	/// <summary>
	/// Get the anchor-point of the sprite.
	/// </summary>
	/// <returns>The anchor.</returns>
	const Tmpl8::vec2 GetAnchor() const
	{
		return m_anchor;
	}

	/// <summary>
	/// Set the frame of the animation in the sprite.
	/// </summary>
	/// <param name="frame">- The frame to set it with.</param>
	void SetFrame(int frame)
	{
		m_frame = frame;
	}

	/// <summary>
	/// Get the current frame of the animation in the sprite.
	/// </summary>
	/// <returns>The current frame of the sprite animation.</returns>
	int GetFrame() const
	{
		return m_frame;
	}

	/// <summary>
	/// Calculate the bounds to use in constructor and to update after moving the entity.
	/// </summary>
	/// <returns></returns>
	void CalculateBounds();

	/// <summary>
	/// Checks whether or not the entity is destroyed.
	/// </summary>
	/// <returns></returns>
	bool CheckIfAlive()
	{
		return m_isAlive;
	}

	/// <summary>
	/// Get the bounds to use for collision.
	/// </summary>
	/// <returns></returns>
	Bounds GetBounds()
	{
		return m_bounds;
	}

	/// <summary>
	/// Use to update the bounds.
	/// </summary>
	/// <param name="bounds"></param>
	void SetBounds(Bounds bounds)
	{
		m_bounds = bounds;
	}

protected:
	Tmpl8::vec2 m_position;
	Tmpl8::Sprite m_sprite;
	Tmpl8::vec2 m_anchor;
	Bounds m_bounds;

private:
	int m_frame = 1;
	bool m_isAlive = true;
};