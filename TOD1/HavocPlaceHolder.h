#pragma once

#include "Placeholder.h"

#define HAVOC_PLACEHOLDER_CLASS_SIZE 288

class HavocPlaceHolder : public PlaceHolder
{
protected:
 unsigned int m_Flags_12;
 float m_VIPTimer;
 int m_Flags_11;
 int m_Flags_10;
 int m_Flags_9;

public:
 HavocPlaceHolder() : PlaceHolder() // NOTE: no constructor.
 {
  MESSAGE_CLASS_CREATED(HavocPlaceHolder);

  m_VIPTimer = 0.0f;

  m_Flags_9 = m_Flags_9 & 0xFE000000;
  m_Flags_10 = m_Flags_10 & 0xFF000000;
  m_Flags_11 = m_Flags_11 & 0xFFFF0000;
  m_Flags_12 = m_Flags_12 & 0xFFFFFC02 | 2;
 }
};

static_assert(sizeof(HavocPlaceHolder) == HAVOC_PLACEHOLDER_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(HavocPlaceHolder));