#include "CollisionProbe.h"
#include "EntityType.h"

EntityType* tCollisionProbe;
std::vector<CollisionProbe*> CollisionProbe::ProbesList;
unsigned int CollisionProbe::CachedProbes;

CollisionProbe::CollisionProbe(int, float) : Node(NODE_MASK_POSITION)
{
    MESSAGE_CLASS_CREATED(CollisionProbe);

    field_AC = 0x2007C00;
    m_CachedProbes = nullptr;

    Reset();

    field_FC = true;

    ProbesList.push_back(this);
    m_CachedProbes = nullptr;

    Reset();
}

#pragma message(TODO_IMPLEMENTATION)
void CollisionProbe::Reset()
{
    m_UserMask = -1;
    m_CollisionMask = -1;
    m_DynamicMask = NULL;
    m_Angle = -1.f;
    m_LineMode = NULL;
    m_LineThickness = 0.f;
    m_LineWidth = NULL;
    m_LineHeight = NULL;
    m_ClosestNode = nullptr;
    m_ClosestCollisionVolume = NULL;
    m_RealNode = nullptr;
    m_IgnoredNodes.clear();
    m_SurfaceID = -1;
    m_MaterialID = -1;
    m_HintNode = nullptr;
    m_OptimisticMode = NULL;
    field_AC = NULL;

    // _A3DE18->8B3590();
    // _A3DE18->8B4570(ScriptType_Builtin::ZeroVector);

    ClearCache();
}

void CollisionProbe::ClearCache()
{
    if (m_CachedProbes)
    {
        do
        {
            CachedProbe* cachedprobeptr = m_CachedProbes->m_Next;

            --CachedProbes;
            delete m_CachedProbes;

            m_CachedProbes = cachedprobeptr->m_Next;
        } while (m_CachedProbes);
    }

    m_Nodes.clear();
    m_TouchingNodes.clear();
}

#pragma message(TODO_IMPLEMENTATION)
void CollisionProbe::Register()
{
    tCollisionProbe = new EntityType("CollisionProbe");
    tCollisionProbe->InheritFrom(tNode);
    tCollisionProbe->SetCreator((CREATOR)Create);
}

CollisionProbe* CollisionProbe::Create()
{
    return new CollisionProbe(0, -1.0f);
}