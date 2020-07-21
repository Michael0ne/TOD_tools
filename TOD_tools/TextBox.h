#pragma once

#include "Sprite.h"

#define TEXTBOX_CLASS_SIZE 220

enum E_TEXTBOX_HORIZONTAL_ALIGNMENT
{
	ALIGN_LEFT = 0,
	ALIGN_RIGHT = 1,
	ALIGN_CENTER = 2,
	ALIGN_JUSTIFY = 3
};

class TextBox : public Sprite
{
protected:
	int* m_Font;
	int field_A8;
	String* m_Text;
	int field_B0;
	class TextSlot* m_TextSlot;
	int field_B8;
	int m_TextColor;
	int m_TextColor2;
	float m_ScaleX;
	float m_ScaleY;
	float m_HorizontalSpacing;
	float m_VerticalSpacing;
	float m_VerticalScroll;
	unsigned int m_Flags;

public:
	TextBox();	//	@8FDAA0
};

static_assert(sizeof(TextBox) == TEXTBOX_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(TextBox));