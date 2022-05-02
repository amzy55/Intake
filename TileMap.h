#pragma once

#include "Bounds.h"
#include "surface.h"
//#include "Settings.h"
#include <cassert>
#include <string>
#include <vector>
#include <array>

static const int arraySize = 60 * 60; //x * y

struct Tile
{
	// Is this a blocking tile?
	bool isBlocking = false;
	// The x pixels in the tilemap.
	int x;
	// The y pixels in the tilemap.
	int y;
	// The width and height of the tile in pixels.
	int tileSize;
};

class TileMap
{
public:
	/// <summary>
	/// Create a tile map with initial width and height.
	/// </summary>
	/// <param name="file">The file path of the tile map.</param>
	TileMap(const char* file);

	const Tile* GetTile(int x, unsigned int y) const;
	void SetTile(int x, int y, Tile* tile);

	/// <summary>
	/// Set the tiles of the tile map.
	/// </summary>
	/// <param name="tiles">The tilemap.</param>
	/// <param name="width">The number of tiles in the width of the tilemap.</param>
	void SetTiles(const Tile* tiles[], int width);

	/// <summary>
	/// Check to see if a point collides with a blocking tile.
	/// </summary>
	/// <param name="p">The point to check.</param>
	/// <returns>true if the point collides with a blocking tile, false otherwise.</returns>
	bool Collides(const Tmpl8::vec2& point) const;

	/// <summary>
	/// Check to see if the bounding box collides with a blocking tile.
	/// </summary>
	/// <param name="bounds">The bounding box to check for collision with.</param>
	/// <returns></returns>
	bool Collides(const Bounds& bounds) const;

	/// <summary>
	/// Returns a vector of bounds of all the colliding tiles.
	/// </summary>
	/// <param name="bounds">The bounding box to check for collision with.</param>
	/// <returns></returns>
	std::vector<Bounds> GetTilesBounds(Bounds& bounds);

	bool NewCollides(Bounds& bounds);

	std::vector<Bounds> NewGetTilesBounds(Bounds& bounds);

	Tmpl8::vec2 GetSizeInPixels() const
	{
		int height = static_cast<int>(arraySize) / m_width;
		return { static_cast<float>(m_width * m_tiles[0]->tileSize), static_cast<float>(height * m_tiles[0]->tileSize) };
	}

	void SetOffset(const Tmpl8::vec2& offset)
	{
		m_offset = offset;
	}

	void Move(const Tmpl8::vec2& translation)
	{
		m_offset += translation;
	}

	const Tmpl8::vec2& GetOffset() const
	{
		return m_offset;
	}

	const Tile* operator()(int x, int y) const
	{
		return GetTile(x, y);
	}

	void Draw(Tmpl8::Surface& screen);

	std::vector<Tmpl8::vec2> GetNonCollidingPos();


private:
	void DrawTile(Tmpl8::Surface& screen, const Tile* tile, int tileX, int tileY);

	Tmpl8::Surface m_tileSurface;
	const Tile* m_tiles[arraySize] = {};

	// Number of tiles in the width of the map.
	int m_width = 0;
	Tmpl8::vec2 m_offset{ 0, 0 };
};

