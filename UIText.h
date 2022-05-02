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
	UIText(char* text, Tmpl8::vec2 position = 0, Tmpl8::Pixel color = 0, int size = 1, int number1 = -1, int number2 = -1, char* separator = {})
		: m_text(text)
		, m_position(position)
		, m_color(color)
		, m_size(size)
		, m_separator(separator)
	{
		SetNumber1(number1);
		SetNumber2(number2);
	};

	/// <summary>
	/// Convert int number to char* number.
	/// </summary>
	/// <param name="number"></param>
	void SetNumber1(int number)
	{
		char buffer[256];
		snprintf(buffer, 256, "%i", number);
		m_number1 = buffer;
	}

	/// <summary>
	/// Convert int number to char* number.
	/// </summary>
	/// <param name="number"></param>
	void SetNumber2(int number)
	{
		char buffer[256];
		snprintf(buffer, 256, "%i", number);
		m_number2 = buffer;
	}

	/// <summary>
	/// Width of text in pixels.
	/// </summary>
	/// <returns></returns>
	int GetTextSize()
	{
		return (int)strlen(m_text) * m_size * 6;
	}

	/// <summary>
	/// Width of the first number in pixels.
	/// </summary>
	/// <returns></returns>
	int GetNumber1Size()
	{
		return (int)strlen(m_number1) * m_size * 6;
	}

	void DrawText(Tmpl8::Surface& screen)
	{
		screen.Print(m_text, static_cast<int>(m_position.x), static_cast<int>(m_position.y), m_color, m_size);
	}

	void DrawTextCentre(Tmpl8::Surface& screen)
	{
		screen.Centre(m_text, static_cast<int>(m_position.y), m_color, m_size);
	}

	void DrawSeparator(Tmpl8::Surface& screen)
	{

		screen.Print(m_separator, static_cast<int>(m_position.x) + GetTextSize() + GetNumber1Size(), static_cast<int>(m_position.y), m_color, m_size);
	}

	void DrawNumber(Tmpl8::Surface& screen, int number1, int number2 = -1)
	{
		SetNumber1(number1);
		screen.Print(m_number1, static_cast<int>(m_position.x) + GetTextSize(), static_cast<int>(m_position.y), m_color, m_size);
		
		if (number2 != -1)
		{
			DrawSeparator(screen);
			SetNumber2(number2);
			if (number1 < 10 && number2 >= 10 || number1 > 10 && number2 < 10)
				 screen.Print(m_number2, static_cast<int>(m_position.x) + GetTextSize() + GetNumber1Size(), static_cast<int>(m_position.y), m_color, m_size);
			else screen.Print(m_number2, static_cast<int>(m_position.x) + GetTextSize() + GetNumber1Size() + m_size * 6, static_cast<int>(m_position.y), m_color, m_size);
		}
	}

	/// <summary>
	/// Draw the text and the number at the same time.
	/// Separation was needed as two Print() calls were not allowed in the same function.
	/// </summary>
	/// <param name="screen"></param>
	/// <param name="number"></param>
	void Draw(Tmpl8::Surface& screen, int number1 = -1, int number2 = -1)
	{
		DrawText(screen);
		if (number1 != -1)
			DrawNumber(screen, number1, number2);
	}

protected:

private:
	char* m_text;
	char* m_number1;
	char* m_number2;
	char* m_separator;
	Tmpl8::vec2 m_position = 0;
	Tmpl8::Pixel m_color = 0;
	int m_size = 1;
};

