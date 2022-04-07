#pragma once
#include "Node.h"

class CharacterPlaceHolder : public Node
{
protected:
    unsigned int    m_Flags_1;
    float           m_VIPTimer;
    unsigned int    m_Flags_2;
    float           m_StoredDamage;
    float           m_OrgHealth;
    unsigned int    m_Flags_3;
    char           *m_DynamicNodeName;
    unsigned int    m_Flags_4;
    float           m_Wait_until_Aggressive;
    int             m_DesiredRange;
    float           m_CustomDesiredRange;
    unsigned int    m_Flags_5;
    float           m_GotopointDamage_mod;
    unsigned int    m_Flags_6;
    float           m_Health_mod;
    float           m_Damage_mod;
    float           m_Marco_sqr_dist;
    int             m_Flags_7;
    unsigned int    m_Flags_8;
    unsigned int    m_Flags_9;

public:
    #pragma message(TODO_IMPLEMENTATION)
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
    }

    virtual ~CharacterPlaceHolder(); // @8CC860
    virtual Vector4f* GetBounds(Vector4f& outbounds) override; // @8CC800

    static void                     Register(); //  @8CB680

private:
    static CharacterPlaceHolder*    Create(AllocatorIndex); //  @8CC8A0
};

extern EntityType* tCharacterPlaceHolder;

ASSERT_CLASS_SIZE(CharacterPlaceHolder, 160);