#include "Sprite.h"
#include "GfxInternal.h"
#include "GfxInternal_Dx9.h"
#include "InputMouse.h"

std::vector<Sprite*> Sprite::SpritesList;
int Sprite::OnMouseEnter = -1;
int Sprite::OnMouseExit = -1;
int Sprite::OnMouseOver = -1;

Sprite::Sprite() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
{
 MESSAGE_CLASS_CREATED(Sprite);

 m_Texture = nullptr;
 field_8C = 1;
 m_SpriteSize_X = 64.0f;
 m_SpriteFlags = m_SpriteFlags & 0xFEFEFC0F | 0x400;
 field_94 = 0;
 m_SpriteSize_Y = 64.0f;
 m_SpriteFlags = m_SpriteFlags ^ (unsigned short)(m_SpriteFlags ^ *(short*)0xA3E0CC << 11) & 0xF800;
 m_Opacity = m_Opacity | 0xFF;
 m_ConstSizeNear = 10;
 m_ConstSizeFar = 10;
 m_ColorRGB_1 = m_ColorRGB_2 = m_ColorRGB_3 = m_ColorRGB_4 = -1;
 m_SpriteFlags = m_SpriteFlags & 0xFF81FFF0 | 0x800000;
 m_Angle = 0.0f;
 m_ConstSizeFarShrink = 25;
 m_SpriteState.MouseEventsEnabled = true;
 m_SpriteState.MouseOver = false;
 m_SpriteState.c = m_SpriteState.d = false;
 m_v1u = m_v1v = 0.0f;
 m_v2u = 0.0f; m_v2v = 1.0f;
 m_v3u = m_v3v = 1.0f;
 m_v4u = 1.0f; m_v4v = 0.0f;

 m_QuadTree->field_1C = m_QuadTree->field_1C & 0xFFFFFF | m_QuadTree->field_1C & 0xFF000000 | 0x80000000;

 SpritesList.push_back(this);
}

void Sprite::TriggerMouseCallbacks()
{
 if (OnMouseEnter == -1)
  OnMouseEnter = GetCommandByName("OnMouseEnter");
 if (OnMouseExit == -1)
  OnMouseExit = GetCommandByName("OnMouseExit");
 if (OnMouseOver == -1)
  OnMouseOver = GetCommandByName("OnMouseOver");

 tagPOINT mousepos = g_GfxInternal_Dx9->m_Windowed ? g_InputMouse->m_WindowedMousePosition : g_InputMouse->m_FullscreenMousePosition;
 ScreenResolution screenres;
 g_GfxInternal->GetScreenResolution(screenres);
 screenres =
 {
  (unsigned int)((float)mousepos.x / ((float)screenres.x * 0.00125f)),
  (unsigned int)((float)mousepos.y / ((float)screenres.y * 0.0016666667f))
 };

 if (!SpritesList.size())
  return;

 for (std::vector<Sprite*>::const_iterator it = SpritesList.cbegin(); it != SpritesList.cend(); ++it)
 {
  if ((*it)->m_SpriteState.MouseEventsEnabled &&
   ((*it)->m_SpriteFlags & 0x10) == 0 && (((*it)->m_Flags.m_FlagBits.Disable | (*it)->m_Flags.m_FlagBits.Invisible) & 1) == 0)
  {
   Vector4f spritepos;
   (*it)->GetPos(spritepos);

   if (((*it)->m_SpriteFlags & 0x20) != 0)
    mousepos = { (LONG)screenres.x, (LONG)screenres.y };

   if (mousepos.x < spritepos.x || mousepos.y < mousepos.y ||
    (mousepos.x + (*it)->m_SpriteSize_X) < mousepos.x || (mousepos.y + (*it)->m_SpriteSize_Y) < mousepos.y)
   {
    if ((*it)->m_SpriteState.MouseEventsEnabled && (*it)->m_SpriteState.MouseOver)
    {
     (*it)->TriggerGlobalScript(OnMouseExit, 0);
     (*it)->m_SpriteState.MouseOver = false;
    }
   }
   else
   {
    if ((*it)->m_SpriteState.MouseOver == false)
    {
     (*it)->TriggerGlobalScript(OnMouseEnter, 0);
     (*it)->m_SpriteState.MouseOver = true;
    }

    (*it)->TriggerGlobalScript(OnMouseOver, 0);
   }
  }
 }
}