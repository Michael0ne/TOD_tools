#pragma once
#include "Node.h"

class Wind : public Node
{
protected:
    Vector4f    m_Pos[20];
    float       field_190;
    float       field_194;
    float       field_198;
    float       field_19C;
    float       m_WindSpeed;
    float       m_WindFlux;
    float       m_WindFrenzy;
    float       m_FrenzyAngle;
    float       m_VariationSpeed;

public:
    Wind();	//	@8D2200
    virtual ~Wind() override;    //  @8D29B0 for scalar, 8D22E0 for unused, but same.
    virtual void    Instantiate() override; //  @8D2310

    static Wind*    Instance;   //  @A3DFF0
    static int      LastUpdateTime; //  @A0ADC4

    static void		Register();	//	@8D2370

private:
    static Wind*    Create(AllocatorIndex); //  @8D2970
    
    static int      CreationTime;   //  @A0ADC0

    const float     GetWindSpeed() const;   //  @833340
    void            SetWindSpeed(const float speed);    //  @659DE0

    const float     GetWindFlux() const;    //  @8D2180
    void            SetWindFlux(const float flux);  //  @659E00

    const float     GetWindFrenzy() const;  //  @8D2190
    void            SetWindFrenzy(const float frenzy);  //  @907E80

    const float     GetFrenzyAngle() const; //  @8D21C0
    void            SetFrenzyAngle(const float angle);  //  @8D21A0

    const float     GetVariationSpeed() const;  //  @8D21F0
    void            SetVariationSpeed(const float speed);   //  @8D21D0
};

extern EntityType* tWind;   //  @A3DFF4

ASSERT_CLASS_SIZE(Wind, 436);