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
	UIText(char* text, Tmpl8::vec2 position = 0, Tmpl8::Pixel color = 0, int size = 1, int number1 = -1, int number2 = -1, char* separator = {});

	/// <summary>
	/// Convert int number to char* number.
	/// </summary>
	/// <param name="number"></param>
	void SetNumber1(int number);

	/// <summary>
	/// Convert int number to char* number.
	/// </summary>
	/// <param name="number"></param>
	void SetNumber2(int number);

	/// <summary>
	/// Width of text in pixels.
	/// </summary>
	/// <returns></returns>
	int GetTextSize();

	/// <summary>
	/// Width of the first number in pixels.
	/// </summary>
	/// <returns></returns>
	int GetNumber1Size();

	void DrawText(Tmpl8::Surface& screen);

	void DrawTextCentre(Tmpl8::Surface& screen);

	void DrawSeparator(Tmpl8::Surface& screen);

	void DrawNumbers(Tmpl8::Surface& screen, int number1, int number2 = -1);

	/// <summary>
	/// Draw the text and the number at the same time.
	/// Separation was needed as two Print() calls were not allowed in the same function.
	/// </summary>
	/// <param name="screen"></param>
	/// <param name="number"></param>
	void Draw(Tmpl8::Surface& screen, int number1 = -1, int number2 = -1);

	/// <summary>
	/// Chenge the position of the text for the end screen.
	/// </summary>
	/// <param name="screen"></param>
	/// <param name="postion"></param>
	void ChangePosition(Tmpl8::vec2 postion)
	{
		m_position = postion;
	}

	void ChangeColor(Tmpl8::Pixel color)
	{
		m_color = color;
	}

	void ChangeSize(int size)
	{
		m_size = size;
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

