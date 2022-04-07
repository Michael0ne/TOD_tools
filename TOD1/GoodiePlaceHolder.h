#pragma once
#include "Placeholder.h"

class GoodiePlaceHolder : public PlaceHolder
{
protected:
    unsigned int m_Flags_13;
    float m_VIPTimer;
    unsigned int m_Flags_14;
    float m_RespawnTime;

public:
    GoodiePlaceHolder() : PlaceHolder()
    {
        MESSAGE_CLASS_CREATED(GoodiePlaceHolder);

        m_Flags_13 = m_Flags_13 & 0xFFFFFC05 | 5;
        m_Flags_14 = m_Flags_14 & 0xFFFFFE00;

        m_VIPTimer = 0.f;
        m_RespawnTime = 0.f;
    }

    static void     Register(); //  @8CD2E0

private:
    static GoodiePlaceHolder*   Create(AllocatorIndex); //  @8CD6A0
};

extern EntityType* tGoodiePlaceholder;  //  @A3DFC8

ASSERT_CLASS_SIZE(GoodiePlaceHolder, 284);