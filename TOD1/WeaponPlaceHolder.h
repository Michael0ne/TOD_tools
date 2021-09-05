#pragma once

#include "Placeholder.h"

#define WEAPON_PLACEHOLDER_CLASS_SIZE 284

class WeaponPlaceHolder : public PlaceHolder
{
protected:
 unsigned int m_Flags_15;
 float m_VIPTimer;
 unsigned int m_Flags_16;
 float m_RespawnTime;

public:
 WeaponPlaceHolder() : PlaceHolder() // NOTE: no constructor.
 {
  MESSAGE_CLASS_CREATED(WeaponPlaceHolder);

  m_Flags_15 = m_Flags_15 & 0xFFFFFC03 | 3;
  m_Flags_16 = m_Flags_16 & 0xFFFF8000;

  m_VIPTimer = 0.0f;
  m_RespawnTime = 0.0f;
 }
};

static_assert(sizeof(WeaponPlaceHolder) == WEAPON_PLACEHOLDER_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(WeaponPlaceHolder));