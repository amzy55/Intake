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

	bool BoundsCollide(const Bounds& other) const
	{
		return BoundsCollide(*this, other);
	}

	static bool BoundsCollide(const Bounds& a, const Bounds& b)
	{
		return a.min.x < b.max.x && a.max.x > b.min.x &&
			a.min.y < b.max.y && a.max.y > b.min.y;
	}

	bool NewBoundsCollide(const Bounds& other)
	{
		return ((MinX() >= other.MinX() && MinX() <= other.MaxX() && MinY() >= other.MinY() && MinY() <= other.MaxY()) ||
			(MaxX() >= other.MinX() && MaxX() <= other.MaxX() && MinY() >= other.MinY() && MinY() <= other.MaxY()) ||
			(MaxX() >= other.MinX() && MaxX() <= other.MaxX() && MaxY() >= other.MinY() && MaxY() <= other.MaxY()) ||
			(MinX() >= other.MinX() && MinX() <= other.MaxX() && MaxY() >= other.MinY() && MaxY() <= other.MaxY()));
	}

	bool BoundsPointCollide(Tmpl8::vec2 p)
	{
		return p.x > MinX() && p.x < MaxX() && p.y > MinY() && p.y < MaxY();
	}

	bool AboveOrBelow(const Bounds& other)
	{
		return MinX() > other.MinX() && MinX() < other.MaxX() ||
			MaxX() > other.MinX() && MaxX() < other.MaxX();
	}

	bool Below(const Bounds& other)
	{
		return MinX() > other.MinX() && MinX() < other.MaxX();
	}


	bool Above(const Bounds& other)
	{
		return MaxX() > other.MinX() && MaxX() < other.MaxX(); 
	}

	bool LeftOrRight(const Bounds& other)
	{
		return MinY() > other.MinY() && MinY() < other.MaxY() ||
			MaxY() > other.MinY() && MaxY() < other.MaxY();
	}

	bool Left(const Bounds& other)
	{
		return MinY() > other.MinY() && MinY() < other.MaxY();
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

	//bool BoundsEdgeCollision(Tmpl8::vec2& p)
	//{
	//	int px = static_cast<int>(p.x);
	//	int py = static_cast<int>(p.y);

	//	int minx = MinX();
	//	int maxx = MaxX();
	//	int miny = MinY();
	//	int maxy = MaxY();

	//	return (((px == minx || px == maxx) && (py >= miny && py <= maxy)) ||
	//		((py == miny || py == maxy) && (px >= minx && px <= maxx)));
	//}

	//bool CompareFloats(float x, float y, float margin = 0.01f)
	//{
	//	if (fabs(x - y) < margin)
	//		return true;
	//	return false;
	//}

	//bool BoundsEdgeCollision(Tmpl8::vec2& p)
	//{
	//	return (((CompareFloats(p.x, MinX()) || CompareFloats(p.x, MaxX())) && (p.y >= MinY() && p.y <= MaxY())) ||
	//		((CompareFloats(p.y, MinY()) || CompareFloats(p.y, MaxY())) && (p.x >= MinX() && p.x <= MaxX())));
	//}
};