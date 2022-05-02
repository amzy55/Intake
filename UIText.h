#pragma once

#include "Surface.h"
#include "Template.h"
#include <string>

class UIText
{
public:
	/// <summary>
	/// Create an UI text element.
	/// </summary>
	/// <param name="text"></param>
	/// <param name="number">Specify if the element contains numbers (such as score).</param>
	/// <param name="position">Top left of the text.</param>
	/// <param name="color"></param>
	/// <param name="size"></param>
	UIText(char* text, int number = 0, Tmpl8::vec2 position = 0, Tmpl8::Pixel color = 0, int size = 1)
		: m_text(text)
		, m_position(position)
		, m_color(color)
		, m_size(size)
	{
		SetNumber(number);
	};

	/// <summary>
	/// Convert int number to char* number.
	/// </summary>
	/// <param name="number"></param>
	void SetNumber(int number)
	{
		char buffer[256];
		snprintf(buffer, 256, "%i", number);
		m_number = buffer;
	}

	/// <summary>
	/// Width of text in pixels.
	/// </summary>
	/// <returns></returns>
	int GetTextSize()
	{
		return (int)strlen(m_text) * m_size * 6;
	}

	void DrawText(Tmpl8::Surface& screen)
	{
		screen.Print(m_text, m_position.x, m_position.y, m_color, m_size);
	}

	void DrawNumber(Tmpl8::Surface& screen, int number)
	{
		SetNumber(number);
		screen.Print(m_number, m_position.x + GetTextSize(), m_position.y, m_color, m_size);
	}

	/// <summary>
	/// Draw the text and the number at the same time.
	/// Separation was needed as two Print() calls were not allowed in the same function.
	/// </summary>
	/// <param name="screen"></param>
	/// <param name="number"></param>
	void Draw(Tmpl8::Surface& screen, int number)
	{
		DrawText(screen);
		DrawNumber(screen, number);
	}

protected:

private:
	char* m_text;
	char* m_number;
	Tmpl8::vec2 m_position = 0;
	Tmpl8::Pixel m_color = 0;
	int m_size = 1;
};

