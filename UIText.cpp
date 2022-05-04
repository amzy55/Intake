#include "UIText.h"

UIText::UIText(char* text, Tmpl8::vec2 position, Tmpl8::Pixel color, int size, int number1, int number2, char* separator)
	: m_text(text)
	, m_position(position)
	, m_color(color)
	, m_size(size)
	, m_separator(separator)
{
	SetNumber1(number1);
	SetNumber2(number2);
};

void UIText::SetNumber1(int number)
{
	char buffer[256];
	snprintf(buffer, 256, "%i", number);
	m_number1 = buffer;
}

void UIText::SetNumber2(int number)
{
	char buffer[256];
	snprintf(buffer, 256, "%i", number);
	m_number2 = buffer;
}

int UIText::GetTextSize()
{
	return (int)strlen(m_text) * m_size * 6;
}

int UIText::GetNumber1Size()
{
	return (int)strlen(m_number1) * m_size * 6;
}

void UIText::DrawText(Tmpl8::Surface& screen)
{
	screen.Print(m_text, static_cast<int>(m_position.x), static_cast<int>(m_position.y), m_color, m_size);
}

void UIText::DrawTextCentre(Tmpl8::Surface& screen)
{
	screen.Centre(m_text, static_cast<int>(m_position.y), m_color, m_size);
}

void UIText::DrawSeparator(Tmpl8::Surface& screen)
{

	screen.Print(m_separator, static_cast<int>(m_position.x) + GetTextSize() + GetNumber1Size(), static_cast<int>(m_position.y), m_color, m_size);
}

void UIText::DrawNumbers(Tmpl8::Surface& screen, int number1, int number2)
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

void UIText::Draw(Tmpl8::Surface& screen, int number1, int number2)
{
	DrawText(screen);
	if (number1 != -1)
		DrawNumbers(screen, number1, number2);
}
