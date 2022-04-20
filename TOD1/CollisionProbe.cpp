#include "CollisionProbe.h"
#include "EntityType.h"
#include "BuiltinType.h"
#include "NumberType.h"
#include "IntegerType.h"
#include "VectorType.h"
#include "TruthType.h"

EntityType* tCollisionProbe;
std::vector<CollisionProbe*> CollisionProbe::ProbesList;
unsigned int CollisionProbe::CachedProbes;

CollisionProbe::CollisionProbe(int, float) : Node(NODE_MASK_POSITION)
{
    MESSAGE_CLASS_CREATED(CollisionProbe);

    field_AC = 0x2007C00;
    m_CachedProbes = nullptr;

    Reset_Impl();

    field_FC = true;

    ProbesList.push_back(this);
    m_CachedProbes = nullptr;

    Reset_Impl();
}

const float CollisionProbe::GetRadius() const
{
    return m_Radius;
}

void CollisionProbe::SetRadius(const float radius)
{
    m_Radius = radius;
}

const float CollisionProbe::GetAngle() const
{
    return m_Angle;
}

void CollisionProbe::SetAngle(const float angle)
{
    m_Angle = angle;
}

const int CollisionProbe::GetDynamicMask() const
{
    return m_DynamicMask;
}

void CollisionProbe::SetDynamicMask(const int mask)
{
    m_DynamicMask = mask;
}

const int CollisionProbe::GetUserMask() const
{
    return m_UserMask;
}

void CollisionProbe::SetUserMask(const int mask)
{
    m_UserMask = mask;
}

std::vector<Node*>& CollisionProbe::GetNodes()
{
    return m_Nodes;
}

std::vector<Node*>& CollisionProbe::GetTouchingNodes()
{
    return m_TouchingNodes;
}

Node* CollisionProbe::GetClosestNode()
{
    return m_ClosestNode;
}

const int CollisionProbe::GetClosestCollisionVolume() const
{
    return m_ClosestCollisionVolume;
}

Node* CollisionProbe::GetRealNode()
{
    return m_RealNode;
}

void CollisionProbe::GetClosestNormal(Vector4f& outNormal) const
{
    outNormal = m_ClosestNormal;
}

void CollisionProbe::GetResolvedPos(Vector4f& outPos) const
{
    outPos = m_ResolvedPos;
}

void CollisionProbe::GetContactPos(Vector4f& outPos) const
{
    outPos = m_ContactPos;
}

const float CollisionProbe::GetMinDistance() const
{
    return m_MinDistance;
}

const int CollisionProbe::GetSurfaceID() const
{
    return m_SurfaceID;
}

const int CollisionProbe::GetMaterialID() const
{
    return m_MaterialID;
}

#pragma message(TODO_IMPLEMENTATION)
const int CollisionProbe::GetCollisionFlags() const
{
    //  TODO: type of 'ClosestCollisionVolume' is unknown for now.
    return -1;
}

Node* CollisionProbe::GetHintNode()
{
    return m_HintNode;
}

void CollisionProbe::SetHintNode(Node* node)
{
    m_HintNode = node;
}

const int CollisionProbe::GetHintCollisionVolume() const
{
    return m_HintCollisionVolume;
}

void CollisionProbe::SetHintCollisionVolume(const int hintcoll)
{
    m_HintCollisionVolume = hintcoll;
}

const bool CollisionProbe::IsOptimisticMode() const
{
    return m_OptimisticMode;
}

void CollisionProbe::SetOptimisticMode(const bool mode)
{
    m_OptimisticMode = mode;
}

void CollisionProbe::Update(int* args)
{
    Update_Impl(false, BuiltinType::ZeroVector, BuiltinType::ZeroVector);
}

#pragma message(TODO_IMPLEMENTATION)
void CollisionProbe::Update_Impl(const bool updateForLine, const Vector4f& vec1, const Vector4f& vec2)
{
}

void CollisionProbe::UpdateForLine(int* args)
{
    Update_Impl(true, BuiltinType::ZeroVector, BuiltinType::ZeroVector);
}

void CollisionProbe::IsLineColliding(int* args)
{
    const int dummy = args[1];
    const Vector4f vec1((float)args[2], (float)args[3], (float)args[4], 0);
    const Vector4f vec2((float)args[5], (float)args[6], (float)args[7], 0);

    args[0] = IsLineColliding_Impl(dummy, vec1, vec2);
}

#pragma message(TODO_IMPLEMENTATION)
bool CollisionProbe::IsLineColliding_Impl(const int, const Vector4f& vec1, const Vector4f& vec2)
{
    return false;
}

void CollisionProbe::sGetClosestNode(int* args)
{
    const int dummy = args[1];
    const Vector4f vec1((float)args[2], (float)args[3], (float)args[4], 0);
    const Vector4f vec2((float)args[5], (float)args[6], (float)args[7], 0);

    args[0] = GetClosestNode_Impl(dummy, vec1, vec2);
}

#pragma message(TODO_IMPLEMENTATION)
bool CollisionProbe::GetClosestNode_Impl(const int, const Vector4f& vec1, const Vector4f& vec2)
{
    return false;
}

void CollisionProbe::GetFieldOnFace(int* args)
{
    args[0] = GetFieldOnFace_Impl(args[1]);
}

const int CollisionProbe::GetFieldOnFace_Impl(const int face) const
{
    switch (face)
    {
    case 0:
        return field_AC & 63;
    case 1:
        return (field_AC >> 6) & 15;
    case 2:
        return (field_AC >> 10)  & 1;
    case 3:
        return (field_AC >> 11) & 1;
    case 4:
        return (field_AC >> 12) & 1;
    case 5:
        return (field_AC >> 13) & 1;
    case 6:
        return (field_AC >> 14) & 1;
    case 7:
        return (field_AC >> 15) & 1;
    case 8:
        return (field_AC >> 16) & 1;
    case 9:
        return (field_AC >> 17) & 7;
    case 10:
        return (field_AC >> 20) & 7;
    case 11:
        return (field_AC >> 23) & 1;
    case 12:
        return (field_AC >> 24) & 1;
    case 13:
        return (field_AC >> 25) & 1;
    case 14:
        return (field_AC >> 26) & 1;
    case 15:
        return field_AC > 27;
    default:
        return 0;
    }
}

void CollisionProbe::SetLineMode(int* args)
{
    m_LineMode = args[0];
}

void CollisionProbe::SetLineThickness(int* args)
{
    m_LineThickness = args[0];
}

void CollisionProbe::SetLineWidthHeight(float* args)
{
    m_LineWidth = args[0];
    m_LineHeight = args[1];
}

void CollisionProbe::SetCollisionMask(int* args)
{
    m_CollisionMask = args[0];
}

void CollisionProbe::IgnoreNode(int* args)
{
    IgnoreNode_Impl((Node*)args[0]);
}

void CollisionProbe::IgnoreNode_Impl(Node* node)
{
    if (m_IgnoredNodes.size() <= 0)
    {
        m_IgnoredNodes.push_back(node);
    }
    else
    {
        auto existingNode = std::find(m_IgnoredNodes.begin(), m_IgnoredNodes.end(), node);
        if (existingNode != m_IgnoredNodes.end())
            m_IgnoredNodes.push_back(node);
    }
}

void CollisionProbe::RemoveIgnoreNode(int* args)
{
    RemoveIgnoreNode_Impl((Node*)args[0]);
}

void CollisionProbe::RemoveIgnoreNode_Impl(Node* node)
{
    auto foundNode = std::find(m_IgnoredNodes.begin(), m_IgnoredNodes.end(), node);
    if (*foundNode)
        m_IgnoredNodes.erase(foundNode);
}

void CollisionProbe::ResetIgnoreList()
{
    m_IgnoredNodes.clear();
}

void CollisionProbe::Reset(int* args)
{
    Reset_Impl();
}

#pragma message(TODO_IMPLEMENTATION)
void CollisionProbe::Reset_Impl()
{
}

void CollisionProbe::ClearCache(int* args)
{
    ClearCache_Impl();
}

void CollisionProbe::ClearCache_Impl()
{
    CachedProbe* nextProbe = nullptr;
    CachedProbe* currentProbe = nullptr;
    if (m_CachedProbes)
    {
        do
        {
            currentProbe = m_CachedProbes;
            nextProbe = currentProbe->m_Next;
            CachedProbes--;

            if (currentProbe)
                delete currentProbe;

            m_CachedProbes = nextProbe;
        } while (nextProbe);
    }

    m_Nodes.clear();
    m_TouchingNodes.clear();
}

void CollisionProbe::ClearCacheLine(int* args)
{
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
    tCollisionProbe->RegisterProperty(tTRUTH, "OptimisticMode", (EntityGetterFunction)&IsOptimisticMode, (EntitySetterFunction)&SetOptimisticMode, nullptr);

    tCollisionProbe->RegisterScript("Update", (EntityFunctionMember)&Update);
    tCollisionProbe->RegisterScript("UpdateForLine", (EntityFunctionMember)&UpdateForLine);
    tCollisionProbe->RegisterScript("IsLineColliding(integer,vector,vector):truth", (EntityFunctionMember)&IsLineColliding);
    tCollisionProbe->RegisterScript("GetClosestNode(integer,vector,vector):truth", (EntityFunctionMember)&sGetClosestNode);
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