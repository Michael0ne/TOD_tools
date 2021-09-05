#pragma once

#include "Node.h"

#define SOUNDEMITTER_CLASS_SIZE 148

class SoundEmitter : public Node
{
protected:
 Vector4f m_FollowCamOffset;
 class SoundSlot* m_SoundSlot;
 int field_64;
 int field_68;
 float m_MinRange;
 float m_MaxRange;
 unsigned int m_Flags;
 int m_Volume;
 __int16 m_Pitch;
 float m_DopplerFactor;
 int field_84;
 int m_StopMessage;
 __int16 m_Randompitch_Length;
 __int16 m_Randomvolume_Length;
 __int16 field_90;
 __int16 field_92;

public:
 SoundEmitter() : Node(NODE_MASK_POSITION) // NOTE: no constructor.
 {
  MESSAGE_CLASS_CREATED(SoundEmitter);

  field_84 = field_68 = -1;
  field_64 = 0;
  m_StopMessage = -1;
  m_Randompitch_Length = 0;
  m_Randomvolume_Length = 0;
  m_SoundSlot = nullptr;
  field_90 = 10000;
  m_MaxRange = 100.0f;
  m_Pitch = 10000;
  field_92 = 10000;
  m_MinRange = 1.0f;
  m_Volume = m_Volume & 0xFFFE4E21 | 0x4E20;
  m_Flags = m_Flags & 0xC1802710 | 0x40002710;
  m_FollowCamOffset = Vector4f();
  m_Volume = m_Volume | 1;
  m_FollowCamOffset.y = 0.8f;
  m_Flags = m_Flags & 0x7E7FFFFF;
  m_DopplerFactor = 1.0f;
 }
};

static_assert(sizeof(SoundEmitter) == SOUNDEMITTER_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(SoundEmitter));