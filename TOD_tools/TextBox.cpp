#include "TextBox.h"

TextBox::TextBox() : Sprite()
{
	MESSAGE_CLASS_CREATED(TextBox);

	m_Font = nullptr;
	field_A8 = 1;
	m_TextColor = m_TextColor2 = -1;
	m_ScaleX = 1.0f;
	m_ScaleY = 1.0f;
	m_Flags = m_Flags & 0xFFFFFF00;
	m_HorizontalSpacing = m_VerticalSpacing = 0.0f;
	field_94 = 0;
	m_Flags = m_Flags & 0xFFFFFEFF;
	m_Text = new String();
	m_Flags = m_Flags & 0xFFFFA5FF | 0x7FF2000;
	m_TextSlot = nullptr;
	field_B8 = field_B0 = 0;
	m_VerticalScroll = 0.0f;
	m_Id = m_Id | 8;

	m_QuadTree->m_nUserType = m_QuadTree->m_nUserType & 0xFFFFFF | m_QuadTree->m_nUserType & 0xFF000000 | 0x8000000;
}