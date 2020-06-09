#pragma once

#include "stdafx.h"

#include "StringsPool.h"
#include "Entity.h"
#include "TextSlot.h"

#define TEXTBOX_CLASS_SIZE 220

class TextBox : Entity
{
	int field_78;
	int field_7C;
	float m_f80;
	int field_84;
	int field_88;
	int field_8C;
	int field_90;
	int field_94;
	int field_98;
	int field_9C;
	int field_A0;
	int* m_pFont;
	int field_A8;
	String* m_sText;
	int* m_pTextSlotParams;
	TextSlot* m_pTextSlot;
	int field_B8;
	ColorRGB* m_vColor1;
	ColorRGB* m_vColor2;
	float m_fScaleX;
	float m_fScaleY;
	float m_fHorizontalSpacing;
	float m_fVerticalSpacing;
	float m_fVerticalScroll;
	unsigned int m_nFlags;
};

extern TextBox* g_TextBox;

static_assert(sizeof(TextBox) == TEXTBOX_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(TextBox));