#pragma once

#include "Bounds.h"
#include "surface.h"
#include <cassert>
#include <string>
#include <vector>

struct Tile
{
	// Is this a blocking tile?
	bool isBlocking = false;
	// The x pixels in the tilemap.
	int x;
	// The y pixels in the tilemap.
	int y;
	// The with of the tile in pixels.
	int width;
	// The height of the tile in pixels.
	int height;
};

class TileMap
{
public:
	/// <summary>
	/// Create a tile map with initial width and height.
	/// </summary>
	/// <param name="file">The file path of the tile map.</param>
	TileMap(const char* file);

	const Tile* GetTile(int x, int y) const;
	void SetTile(int x, int y, const Tile& tile);

	/// <summary>
	/// Set the tiles of the tile map.
	/// </summary>
	/// <param name="tiles">The tilemap.</param>
	/// <param name="width">The number of tiles in the width of the tilemap.</param>
	void SetTiles(const std::vector<Tile>& tiles, int width);

	/// <summary>
	/// Check to see if a point collides with a blocking tile.
	/// </summary>
	/// <param name="p">The point to check.</param>
	/// <returns>true if the point collides with a blocking tile, false otherwise.</returns>
	bool Collides(const Tmpl8::vec2& p) const;

	/// <summary>
	/// Check to see if the bounding box collides with a blocking tile.
	/// </summary>
	/// <param name="bounds">The bounding box to check for collision with.</param>
	/// <returns></returns>
	bool Collides(const Bounds& bounds) const;

	Tmpl8::vec2 GetSizeInPixels() const
	{
		int height = static_cast<int>(m_tiles.size()) / m_width;
		return { static_cast<float>(m_width * m_tiles[0].width), static_cast<float>(height * m_tiles[0].height) };
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

	const Bounds GetTileBounds(Bounds& bounds);

private:
	void DrawTile(Tmpl8::Surface& screen, const Tile& tile, int tileX, int tileY);

	Tmpl8::Surface m_tileSurface;
	std::vector<Tile> m_tiles;

	// Number of tiles in the width of the map.
	int m_width = 0;
	Tmpl8::vec2 m_offset{ 0, 0 };
};

