#pragma once
#include "Node.h"

class CharacterPlaceHolder : public Node
{
protected:
    unsigned int    m_Flags_8;
    float           m_VIPTimer;
    unsigned int    m_Flags_7;
    float           m_StoredDamage;
    float           m_OrgHealth;
    unsigned int    m_Flags_6;
    char           *m_DynamicNodeName;
    unsigned int    m_Flags_5;
    float           m_Wait_until_Aggressive;
    int             m_DesiredRange;
    float           m_CustomDesiredRange;
    unsigned int    m_Flags_4;
    float           m_GotopointDamage_mod;
    unsigned int    m_Flags_3;
    float           m_Health_mod;
    float           m_Damage_mod;
    float           m_Marco_sqr_dist;
    int             m_Flags_2;
    unsigned int    m_Flags_1;
    unsigned int    m_Flags;

public:
    inline CharacterPlaceHolder() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
    {
        MESSAGE_CLASS_CREATED(CharacterPlaceHolder);

        m_CustomDesiredRange = 25.0f;
        m_GotopointDamage_mod = 100.0f;
        m_VIPTimer = 0.0f;
        m_StoredDamage = 0.0f;
        m_OrgHealth = 0.0f;
        m_DynamicNodeName = "";
        m_Wait_until_Aggressive = 0.0f;
        m_Health_mod = 1.0f;
        m_Damage_mod = 1.0f;
        m_Marco_sqr_dist = 0.0f;

        m_Flags = m_Flags & 0xFFFFFFF0;
        m_Flags_1 = m_Flags_1 & 0xC07EBAE0 | 0x7EBAE0;
        m_Flags_2 = m_Flags_2 & 0x8301FFFF | 0x3010000;
        m_Flags_3 = m_Flags_3 & 0xFFFFF821 | 0x21;
        m_Flags_4 = m_Flags_4 & 0xFFF9E000 | 0x1E000;
        m_Flags_5 = m_Flags_5 & 0xFFFFFEA6 | 0xA6;
        m_Flags_6 = m_Flags_6 & 0xFFFFFFF0;
        m_Flags_7 = m_Flags_7 & 0xFFFFE0B8 | 0xB8;
        m_Flags_8 = m_Flags_8 & 0xFFFFFC01 | 1;
    }

    virtual ~CharacterPlaceHolder();	//	@8CC860
    virtual Vector4f*	GetBounds(Vector4f& outbounds) const override;	//	@8CC800

    static CharacterPlaceHolder*    Create(AllocatorIndex); //  @8CC8A0
    static void                     Register(); //  @8CB680
};

extern EntityType* tCharacterPlaceHolder;

ASSERT_CLASS_SIZE(CharacterPlaceHolder, 160);