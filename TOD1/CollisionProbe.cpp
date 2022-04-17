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

void CollisionProbe::Register()
{
    tCollisionProbe = new EntityType("CollisionProbe");
    tCollisionProbe->InheritFrom(tNode);
    tCollisionProbe->SetCreator((CREATOR)Create);

    tCollisionProbe->RegisterProperty(tNUMBER, "radius", (EntityGetterFunction)&GetRadius, (EntitySetterFunction)&SetRadius, nullptr);
    tCollisionProbe->RegisterProperty(tNUMBER, "angle", (EntityGetterFunction)&GetAngle, (EntitySetterFunction)&SetAngle, nullptr);
    tCollisionProbe->RegisterProperty(tINTEGER, "dynamicmask", (EntityGetterFunction)&GetDynamicMask, (EntitySetterFunction)&SetDynamicMask, nullptr);
    tCollisionProbe->RegisterProperty(tINTEGER, "usermask", (EntityGetterFunction)&GetUserMask, (EntitySetterFunction)&SetUserMask, nullptr);
    DataType* listEntity = DataType::LoadScript("list(entity)");
    tCollisionProbe->RegisterProperty(listEntity, "nodes", (EntityGetterFunction)&GetNodes, nullptr, nullptr);
    tCollisionProbe->RegisterProperty(listEntity, "touchingnodes", (EntityGetterFunction)&GetTouchingNodes, nullptr, nullptr);
    tCollisionProbe->RegisterProperty(tEntity, "ClosestNode", (EntityGetterFunction)&GetClosestNode, nullptr, nullptr);
    tCollisionProbe->RegisterProperty(tINTEGER, "ClosestCollisionVolume", (EntityGetterFunction)&GetClosestCollisionVolume, nullptr, nullptr);
    tCollisionProbe->RegisterProperty(tEntity, "RealNode", (EntityGetterFunction)&GetRealNode, nullptr, nullptr);
    tCollisionProbe->RegisterProperty(tVECTOR, "ClosestNormal", (EntityGetterFunction)&GetClosestNormal, nullptr, nullptr);
    tCollisionProbe->RegisterProperty(tVECTOR, "ResolvedPos", (EntityGetterFunction)&GetResolvedPos, nullptr, nullptr);
    tCollisionProbe->RegisterProperty(tVECTOR, "ContactPos", (EntityGetterFunction)&GetContactPos, nullptr, nullptr);
    tCollisionProbe->RegisterProperty(tNUMBER, "MinDistance", (EntityGetterFunction)&GetMinDistance, nullptr, nullptr);
    tCollisionProbe->RegisterProperty(tINTEGER, "SurfaceID", (EntityGetterFunction)&GetSurfaceID, nullptr, nullptr);
    tCollisionProbe->RegisterProperty(tINTEGER, "MaterialID", (EntityGetterFunction)&GetMaterialID, nullptr, nullptr);
    tCollisionProbe->RegisterProperty(tINTEGER, "CollisionFlags", (EntityGetterFunction)&GetCollisionFlags, nullptr, nullptr);
    tCollisionProbe->RegisterProperty(tEntity, "HintNode", (EntityGetterFunction)&GetHintNode, (EntitySetterFunction)&SetHintNode, nullptr);
    tCollisionProbe->RegisterProperty(tINTEGER, "HintCollisionVolume", (EntityGetterFunction)&GetHintCollisionVolume, (EntitySetterFunction)&SetHintCollisionVolume, nullptr);
    tCollisionProbe->RegisterProperty(tTRUTH, "OptimisticMode", (EntityGetterFunction)&IsOptimisticMode, (EntitySetterFunction)&SetIsOptimisticMode, nullptr);

    tCollisionProbe->RegisterScript("Update", (EntityFunctionMember)&Update);
    tCollisionProbe->RegisterScript("UpdateForLine", (EntityFunctionMember)&UpdateForLine);
    tCollisionProbe->RegisterScript("IsLineColliding(integer,vector,vector):truth", (EntityFunctionMember)&IsLineColliding);
    tCollisionProbe->RegisterScript("GetClosestNode(integer,vector,vector):truth", (EntityFunctionMember)&GetClosestNode);
    tCollisionProbe->RegisterScript("getfieldonface(integer):integer", (EntityFunctionMember)&GetFieldOnFace);
    tCollisionProbe->RegisterScript("setlinemode(integer)", (EntityFunctionMember)&SetLineMode, 0, 0, 0, "SetLineModeMSG");
    tCollisionProbe->RegisterScript("setlinethickness(number)", (EntityFunctionMember)&SetLineThickness, 0, 0, 0, "SetLineThicknessMSG");
    tCollisionProbe->RegisterScript("setlinewidthheight(number,number)", (EntityFunctionMember)&SetLineWidthHeight, 0, 0, 0, "SetLineWidthHeightMSG");
    tCollisionProbe->RegisterScript("setcollisionmask(integer)", (EntityFunctionMember)&SetCollisionMask, 0, 0, 0, "SetCollisionMaskMSG");
    tCollisionProbe->RegisterScript("ignorenode(entity)", (EntityFunctionMember)&IgnoreNode);
    tCollisionProbe->RegisterScript("removeignorenode", (EntityFunctionMember)&RemoveIgnoreNode);
    tCollisionProbe->RegisterScript("resetignorelist", (EntityFunctionMember)&ResetIgnoreList);
    tCollisionProbe->RegisterScript("reset", (EntityFunctionMember)&Reset);
    tCollisionProbe->RegisterScript("clearcache", (EntityFunctionMember)&ClearCache);
    tCollisionProbe->RegisterScript("clearcacheline(integer)", (EntityFunctionMember)&ClearCacheLine);

    tCollisionProbe->PropagateProperties();
}

CollisionProbe* CollisionProbe::Create()
{
    return new CollisionProbe(0, -1.0f);
}