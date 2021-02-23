#include "Sprite.h"

List<Sprite>& Sprite::SpritesList = *(List<Sprite>*)0xA3E0D0;	//	@A3E0D0

Sprite::Sprite() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
{
	MESSAGE_CLASS_CREATED(Sprite);

	m_Texture = nullptr;
	field_8C = 1;
	m_SpriteSize_X = 64.0f;
	m_Flags = m_Flags & 0xFEFEFC0F | 0x400;
	field_94 = 0;
	m_SpriteSize_Y = 64.0f;
	m_Flags = m_Flags ^ (unsigned short)(m_Flags ^ *(short*)0xA3E0CC << 11) & 0xF800;
	m_Opacity = m_Opacity | 0xFF;
	m_ConstSizeNear = 10;
	m_ConstSizeFar = 10;
	m_ColorRGB_1 = m_ColorRGB_2 = m_ColorRGB_3 = m_ColorRGB_4 = -1;
	m_Flags = m_Flags & 0xFF81FFF0 | 0x800000;
	m_Angle = 0.0f;
	m_ConstSizeFarShrink = 25;
	m_EnableMouseInput.a = m_EnableMouseInput.b = m_EnableMouseInput.c = 0xFF;	//	TODO: not sure if this is correct.
	m_EnableMouseInput.State = NULL;
	m_v1u = m_v1v = 0.0f;
	m_v2u = 0.0f; m_v2v = 1.0f;
	m_v3u = m_v3v = 1.0f;
	m_v4u = 1.0f; m_v4v = 0.0f;

	m_QuadTree->m_UserType = m_QuadTree->m_UserType & 0xFFFFFF | m_QuadTree->m_UserType & 0xFF000000 | 0x80000000;

	SpritesList.AddElement(this);
}

#pragma message(TODO_IMPLEMENTATION)
void Sprite::TriggerMouseCallbacks()
{
}