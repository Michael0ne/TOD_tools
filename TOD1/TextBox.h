#pragma once

#include "Sprite.h"
#include "ResourcesTypes.h"

#define TEXTBOX_CLASS_SIZE 220

enum TextBoxHorizontalAlignment
{
	ALIGN_LEFT = 0,
	ALIGN_RIGHT = 1,
	ALIGN_CENTER = 2,
	ALIGN_JUSTIFY = 3
};

class TextBox : public Sprite
{
protected:
	ResType::Font* m_FontRes;
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
	virtual ~TextBox();	//	@8FDBE0
	virtual Vector4f* GetBounds(Vector4f& outBounds) const;	//	@8FF220

	TextBox();	//	@8FDAA0

	void* operator new (size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
	}

	Vector4f*	GetActualBoxSize(Vector4f& outSize) const;	//	@8FDCD0
	void		SetFont(const char* fontName);	//	@8FF2F0
};

static_assert(sizeof(TextBox) == TEXTBOX_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(TextBox));