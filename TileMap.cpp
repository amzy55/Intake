#include "TileMap.h"

using namespace Tmpl8;

TileMap::TileMap(const char* file)
	: m_tileSurface(file)
{}

const Tile* TileMap::GetTile(int x, unsigned int y) const
{
	if (x < 0 || x >= m_width) return nullptr;
	if (y < 0 || y >= arraySize / m_width) return nullptr;

	int i = x + y * m_width;
	return m_tiles[i];
}

void TileMap::SetTile(int x, int y, Tile* tile)
{
	int i = x + y * m_width;
	m_tiles[i] = tile;
}

void TileMap::SetTiles(const Tile* tiles[], int width)
{
	m_width = width;
	for (int i = 0; i < arraySize; ++i)
	{
		m_tiles[i] = tiles[i];
	}
}

bool TileMap::Collides(const vec2& point) const
{
	if (m_tiles[0] == nullptr) return false;

	vec2 localPoint = point - m_offset;

	int tileX = static_cast<int>(localPoint.x / m_tiles[0]->tileSize);
	int tileY = static_cast<int>(localPoint.y / m_tiles[0]->tileSize);

	const Tile* tile = GetTile(tileX, tileY);
	if (tile)      
	{
		return tile->isBlocking;
	}

	return false;
}

bool TileMap::Collides(const Bounds& bounds) const
{
	if (m_tiles[0] == nullptr) return false;

	int tileWidth = m_tiles[0]->tileSize;
	int tileHeight = m_tiles[0]->tileSize;

	vec2 min = bounds.min;
	vec2 max = bounds.max;

	vec2 minPos = min;
	while (minPos.y < max.y + tileWidth)
	{
		minPos.x = min.x;
		while (minPos.x < max.x + tileHeight)
		{
			if (Collides(vec2{ std::min(minPos.x, max.x), std::min(minPos.y, max.y) })) return true;
			minPos.x += tileWidth;
		}
		minPos.y += tileHeight;
	}

	return false;
}

std::vector<Bounds> TileMap::GetTilesBounds(Bounds& bounds)
{
	std::vector<Bounds> tilesBounds = {};

	float tileSize = static_cast<float>(m_tiles[0]->tileSize);

	vec2 min = bounds.min;
	vec2 max = bounds.max;

	vec2 minPos = min;
	while (minPos.y < max.y + tileSize)
	{
		minPos.x = min.x;
		while (minPos.x < max.x + tileSize)
		{
			if (Collides(vec2{ std::min(minPos.x, max.x), std::min(minPos.y, max.y) }))
			{
				vec2 p = vec2{ std::min(minPos.x, max.x), std::min(minPos.y, max.y) };

				vec2 localPoint = p - m_offset;

				int tileX = static_cast<int>(localPoint.x / tileSize);
				int tileY = static_cast<int>(localPoint.y / tileSize);

				float minPlace = tileX * tileSize;
				float maxPlace = tileY * tileSize;

				vec2 minBounds = { minPlace, maxPlace };
				minBounds += m_offset;
				vec2 maxBounds = minBounds + tileSize;

				tilesBounds.push_back({ minBounds, maxBounds });
			}
			minPos.x += tileSize;
		}
		minPos.y += tileSize;
	}
	return tilesBounds;
}

bool TileMap::NewCollides(Bounds& bounds)
{
	if (m_tiles[0] == nullptr) return false;

	float tileSize = static_cast<float>(m_tiles[0]->tileSize);

	for (int x = 0; x < m_width; x++)
		for (int y = 0; y < arraySize / m_width; y++)
		{
			Tmpl8::vec2 min = { static_cast<float>(x) * tileSize, static_cast<float>(y) * tileSize };
			Tmpl8::vec2 max = min + tileSize;
			Bounds tileBounds(Bounds(min, max) + m_offset);
			if (GetTile(x, y)->isBlocking)
				if (bounds.NewBoundsCollide(tileBounds))
					return true;
		}
	return false;
}

std::vector<Bounds> TileMap::NewGetTilesBounds(Bounds& bounds)
{
	float tileSize = static_cast<float>(m_tiles[0]->tileSize);
	std::vector<Bounds> tilesBounds = {};

	for (int x = 0; x < m_width; x++)
		for (int y = 0; y < arraySize / m_width; y++)
		{
			Tmpl8::vec2 min = { static_cast<float>(x) * tileSize, static_cast<float>(y) * tileSize };
			Tmpl8::vec2 max = min + tileSize; 
			Bounds tileBounds(Bounds(min, max) + m_offset);
			if (GetTile(x, y)->isBlocking)
				if (bounds.NewBoundsCollide(tileBounds))
					tilesBounds.push_back(tileBounds);
		}
	return tilesBounds;
}

void TileMap::DrawTile(Tmpl8::Surface& screen, const Tile* tile, int tileX, int tileY)
{
	int dstW = tile->tileSize;
	int dstH = tile->tileSize;
	int dstX = static_cast<int>(m_offset.x + (tileX * tile->tileSize));
	int dstY = static_cast<int>(m_offset.y + (tileY * tile->tileSize));

	// Check if the entire tile is clipped.
	if (dstX + dstW < 0 || dstX >= screen.GetWidth()) return;
	if (dstY + dstH < 0 || dstY >= screen.GetHeight()) return;

	// Compute the amount to clip.
	int clipLeft = std::min(0, dstX);
	int clipTop = std::min(0, dstY);
	int clipRight = std::max(0, dstX + dstW - screen.GetWidth());
	int clipBottom = std::max(0, dstY + dstH - screen.GetHeight());

	// Adjust the destination rectangles according to the clipping.
	dstX -= clipLeft;
	dstY -= clipTop;
	dstW -= clipRight - clipLeft;
	dstH -= clipBottom - clipTop;

	int srcX = tile->x * (tile->tileSize + 1) + 1;
	int srcY = tile->y * (tile->tileSize + 1) + 1;
	// Account for clipping.
	srcX -= clipLeft;
	srcY -= clipTop;

	// Draw the unclipped part of the tile.
	Pixel* dst = screen.GetBuffer() + dstX + dstY * screen.GetPitch();
	Pixel* src = m_tileSurface.GetBuffer() + srcX + srcY * m_tileSurface.GetPitch();

	for (int y = 0; y < dstH; ++y)
	{
		memcpy(dst, src, sizeof(Pixel) * dstW);
		dst += screen.GetPitch();
		src += m_tileSurface.GetPitch();
	}
}

void TileMap::Draw(Tmpl8::Surface& screen)
{
	int tileX = 0;
	int tileY = 0;
	for (auto tile : m_tiles)
	{
		DrawTile(screen, tile, tileX, tileY);
		++tileX;
		if (tileX >= m_width)
		{
			tileX = 0;
			++tileY;
		}
	}
}

std::vector<Tmpl8::vec2> TileMap::GetNonCollidingPos()
{
	float tileSize = static_cast<float>(m_tiles[0]->tileSize);
	std::vector<Tmpl8::vec2> availablePos = {};

	for (int x = 0; x < m_width; x++)
		for (int y = 0; y < arraySize / m_width; y++)
		{
			Tmpl8::vec2 min = { static_cast<float>(x) * tileSize, static_cast<float>(y) * tileSize };
			Tmpl8::vec2 max = min + tileSize; 
			Bounds tileBounds(Bounds(min, max) + m_offset);
			if (!GetTile(x, y)->isBlocking)
				availablePos.push_back({ min.x + tileSize / 2, max.y - tileSize / 2 });
		}

	return availablePos;
}
