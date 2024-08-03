#include "PhysSystem.h"
#include "Wind.h"

PhysSystem::PhysSystem(const unsigned int listsize, Cloth* clothptr, const unsigned char iterations)
{
    MESSAGE_CLASS_CREATED(PhysSystem);

    m_List_1.resize(listsize);
    m_VerticesList.resize(listsize);
    m_RestLength.resize(listsize);
    m_List_4.resize(listsize);

    m_Cloth = clothptr;
    m_GravityVec = { 0, -9.8199997, 0, 0 };

    Wind::CreationTime = 345987156;
    Wind::LastUpdateTime = 345987156;   //  NOTE: no idea what these are.

    m_NumIterations = 1;
    m_Damping = 0.0099999998;
    m_Weight = 1;
    m_WindInfluence = 10;
    field_60 = 0.039999999;

    m_NumIterations = iterations;
}

PhysSystem::~PhysSystem()
{
    MESSAGE_CLASS_DESTROYED(PhysSystem);
}

void PhysSystem::SetVertexPos(const unsigned int vertexindex, const Vector4f& pos)
{
    m_List_1[vertexindex] = pos;
    m_VerticesList[vertexindex].m_Position = pos;
    m_VerticesList[vertexindex].m_Normal = {};
    m_VerticesList[vertexindex].m_Orientation = { 1, 0, 0, 0 };

    m_VerticesList[vertexindex].field_30 = -1;
    m_VerticesList[vertexindex].field_32 = -1;
}

void PhysSystem::SetNormalPos(const unsigned int vertexindex, const Vector4f& norm)
{
    m_VerticesList[vertexindex].m_Normal = norm;
}

bool PhysSystem::Update(const float_t timePassed)
{
    if (field_60 < 0.0299f)
    {
        field_60 += timePassed;
        return false;
    }
    else
    {
        AccountWindAndGravity();
        AccountForDamping(field_60);

        _930D30();
        _930FA0();

        field_60 = timePassed;

        return true;
    }
}

#pragma message(TODO_IMPLEMENTATION)
void PhysSystem::AccountWindAndGravity()
{
}

#pragma message(TODO_IMPLEMENTATION)
void PhysSystem::AccountForDamping(const float_t timePassed)
{
}

#pragma message(TODO_IMPLEMENTATION)
void PhysSystem::_930D30()
{
}

#pragma message(TODO_IMPLEMENTATION)
void PhysSystem::_930FA0()
{
}
