#include "Wind.h"
#include "NumberType.h"
#include "Timer.h"

Wind* Wind::Instance;
EntityType* tWind;
int Wind::CreationTime;
int Wind::LastUpdateTime;

Wind::Wind() : Node(NODE_MASK_POSITION)
{
    MESSAGE_CLASS_CREATED(Wind);

    for (unsigned int i = 0; i < ARRAYSIZE(m_Pos); ++i)
        m_Pos[i] = { 0, 0, 0, 0 };

    Instance = this;

    field_194 = 0;
    field_190 = 0;
    field_19C = 1;
    field_198 = 1;
    m_WindSpeed = 1;
    m_WindFlux = 0.5;
    m_WindFrenzy = 0.5;
    m_FrenzyAngle = 180.0;
    m_VariationSpeed = 0.5;
}

Wind::~Wind()
{
    MESSAGE_CLASS_DESTROYED(Wind);

    if (Instance == this)
        Instance = nullptr;
}

void Wind::Instantiate()
{
    Node::Instantiate();

    for (unsigned int i = 0; i < ARRAYSIZE(m_Pos); ++i)
        m_Pos[i] = { 0, 0, 0, 0 };
}

void Wind::Register()
{
    tWind = new EntityType("Wind");
    tWind->InheritFrom(tNode);
    tWind->SetCreator((CREATOR)Create);
    
    tWind->RegisterProperty(tNUMBER, "WindSpeed", (EntityGetterFunction)&GetWindSpeed, NULL, NULL, NULL, (EntitySetterFunction)&SetWindSpeed, NULL, NULL, NULL, "control=slider|min=0|max=10", NULL, NULL, -1);
    tWind->RegisterProperty(tNUMBER, "WindFlux", (EntityGetterFunction)&GetWindFlux, NULL, NULL, NULL, (EntitySetterFunction)&SetWindFlux, NULL, NULL, NULL, "control=slider|min=0|max=1", NULL, NULL, -1);
    tWind->RegisterProperty(tNUMBER, "WindFrenzy", (EntityGetterFunction)&GetWindFrenzy, NULL, NULL, NULL, (EntitySetterFunction)&SetWindFrenzy, NULL, NULL, NULL, "control=slider|min=0|max=1", NULL, NULL, -1);
    tWind->RegisterProperty(tNUMBER, "FrenzyAngle", (EntityGetterFunction)&GetFrenzyAngle, NULL, NULL, NULL, (EntitySetterFunction)&SetFrenzyAngle, NULL, NULL, NULL, "control=slider|min=0|max=180", NULL, NULL, -1);
    tWind->RegisterProperty(tNUMBER, "VariationSpeed", (EntityGetterFunction)&GetVariationSpeed, NULL, NULL, NULL, (EntitySetterFunction)&SetVariationSpeed, NULL, NULL, NULL, "control=slider|min=0|max=1", NULL, NULL, -1);

    tWind->PropagateProperties();

    CreationTime = Timer::GetMilliseconds();
    LastUpdateTime = CreationTime;
}

Wind* Wind::Create(AllocatorIndex)
{
    return new Wind;
}

const float Wind::GetWindSpeed() const
{
    return m_WindSpeed;
}

void Wind::SetWindSpeed(const float speed)
{
    m_WindSpeed = speed;
}

const float Wind::GetWindFlux() const
{
    return m_WindFlux;
}

void Wind::SetWindFlux(const float flux)
{
    m_WindFlux = flux;
}

const float Wind::GetWindFrenzy() const
{
    return m_WindFrenzy;
}

void Wind::SetWindFrenzy(const float frenzy)
{
    m_WindFrenzy = frenzy;
}

const float Wind::GetFrenzyAngle() const
{
    return m_FrenzyAngle;
}

void Wind::SetFrenzyAngle(const float angle)
{
    m_FrenzyAngle = angle;
}

const float Wind::GetVariationSpeed() const
{
    return m_VariationSpeed;
}

void Wind::SetVariationSpeed(const float speed)
{
    m_VariationSpeed = speed;
}