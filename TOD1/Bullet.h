#pragma once

#include "Model.h"
#include "CollisionProbe.h"
#include "Light.h"
#include "BuiltinType.h"

class Bullet : public Model
{
public:
    struct BulletShell
    {
        friend class Bullet;
    protected:
        Vector4f    m_RightVec;
        Vector4f    m_UpVec;
        Vector4f    m_InVec;
        Vector4f    m_OrientVec;
        Vector4f    m_Vec_5;
        float       field_50;
        float       field_54;
        EntityType *m_IgnoredEntity;
        char        field_5C;
        char        field_5D;
        char        field_5E;
        char        field_5F;
        int         field_60;
        int         field_64;

    public:
        BulletShell(); // NOTE: no actual constructor.
        BulletShell(const BulletShell& rhs) = default; // @8CAFA0
        ~BulletShell(); // NOTE: no actual destructor.
    };

protected:
    float           m_Speed;
    float           m_Range;
    int             m_DamageType;
    float           m_ImpulseMultiplier;
    unsigned int   m_BulletShellsTotal;
    int            *m_SceneBufferPtr;
    std::vector<BulletShell> m_BulletShellsList;
    std::vector<int>m_CollisionProbesList;
    int             m_CurrentBulletShellsNumber;
    std::vector<int>m_List_3;
    std::vector<int>m_List_4;
    Light_Properties* m_Lighting;
    int             field_160;
    int             field_164;

    void            Shoot_Impl(
        const Vector4f& inVec1,
        const Vector4f& inVec2,
        EntityType* ignoredNode1,
        bool a4,
        bool a5,
        const float a6,
        const int iterations,
        const int a8,
        const int a9,
        bool a10,
        const float speedMultiplier,
        EntityType* ignoredNode2);  //  @8CA120

public:
    virtual ~Bullet(); // @8C9290
    Bullet(); // @8C9ED0

    void            ClearShells();  //  @8CB130
    const float     GetSpeed() const;   //  @8C91F0
    void            SetSpeed(const float speed);    //  @4A66C0
    const float     GetRange() const;   //  @8C9200
    void            SetRange(const float range);    //  @698CE0
    const int       GetDamageType() const;  //  @8C9210
    void            SetDamageType(const int damagetype);    //  @4A66E0
    const float     GetImpulseMultiplier() const;   //  @8C9220
    void            SetImpulseMultiplier(const float multiplier);   //  @8C9230

    void            Shoot(int* args);    //  @8CB1D0

    static Bullet*  Create(AllocatorIndex); // @8CB280
    static void     Register(); // @8CADB0
    static void     ClearBullets(); //  @8C99F0

    static int      TakeAHitCommand;   //  @A3DFB8
    static int      BulletImpactCommand;    //  @A3DFBC
};

extern EntityType* tBullet; // @A3DFB4

ASSERT_CLASS_SIZE(Bullet, 360);
ASSERT_CLASS_SIZE(Bullet::BulletShell, 104);