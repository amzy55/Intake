#pragma once

#include "template.h"

#include <cfloat> // for FLT_MIN and FLT_MAX	

struct Bounds
{
	Tmpl8::vec2 min;
	Tmpl8::vec2 max;

	Bounds()
		: min(FLT_MAX, FLT_MAX)
		, max(-FLT_MAX, -FLT_MAX)
	{}

	Bounds(const Tmpl8::vec2& min, const Tmpl8::vec2& max)
		: min(min)
		, max(max)
	{}

	Bounds(Tmpl8::vec2& vector)
		: min(vector)
		, max(vector)
	{}

	float MinX() const
	{
		return min.x;
	}

	float MaxX() const
	{
		return max.x;
	}

	float MinY() const
	{
		return min.y;
	}

	float MaxY() const
	{
		return max.y;
	}

	float Width() const
	{
		return max.x - min.x;
	}

	float Height() const
	{
		return max.y - min.y;
	}

	Bounds Add(const Tmpl8::vec2& p) const
	{
		return Bounds(min + p, max + p);
	}

	bool Collides(const Bounds& other) const
	{
		return Collides(*this, other);
	}

	static bool Collides(const Bounds& a, const Bounds& b)
	{
		return a.min.x < b.max.x && a.max.x > b.min.x &&
			a.min.y < b.max.y&& a.max.y > b.min.y;
	}

	Bounds operator + (const Bounds& operand)
	{
		Tmpl8::vec2 newMin = (*this).min + operand.min;
		Tmpl8::vec2 newMax = (*this).max + operand.max;
		return Bounds(newMin, newMax);
	}

	Bounds operator - (const Bounds& operand)
	{
		Tmpl8::vec2 newMin = (*this).min - operand.min;
		Tmpl8::vec2 newMax = (*this).max - operand.max;
		return Bounds(newMin, newMax);
	}
};