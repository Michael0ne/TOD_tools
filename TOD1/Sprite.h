#pragma once
#include "Node.h"

class Sprite : public Node
{
protected:
 float               m_v1u;
 float               m_v1v;
 float               m_v2u;
 float               m_v2v;
 float               m_v3u;
 float               m_v3v;
 float               m_v4u;
 float               m_v4v;
 int                 m_ColorRGB_1;
 int                 m_ColorRGB_2;
 int                 m_ColorRGB_3;
 int                 m_ColorRGB_4;
 float               m_SpriteSize_X;
 float               m_SpriteSize_Y;
 int*                m_Texture;
 int                 field_8C;
 float               m_Angle;
 int                 field_94;
 char                m_Opacity;
 char                m_ConstSizeNear;
 char                m_ConstSizeFar;
 char                m_ConstSizeFarShrink;
 unsigned int        m_SpriteFlags;
 union
 {
  unsigned char   MouseEventsEnabled;
  unsigned char   MouseOver;
  unsigned char   c;
  unsigned char   d;
 }                   m_SpriteState;

public:
 Sprite(); // @8F6AA0

 static void         TriggerMouseCallbacks(); // @8F4A00

 static std::vector<Sprite*> SpritesList; // @A3E0D0
 static int          OnMouseEnter; // @A12E90
 static int          OnMouseExit; // @A12E94
 static int          OnMouseOver; // @A12E98
};

ASSERT_CLASS_SIZE(Sprite, 164);