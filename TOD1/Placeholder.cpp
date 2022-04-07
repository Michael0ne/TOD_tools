#include "Placeholder.h"
#include "NumberType.h"
#include "TruthType.h"
#include "IntegerType.h"

EntityType* tPlaceholder;
int PlaceHolder::GetPlaceholderModelCommand;

PlaceHolder::PlaceHolder() : Model()
{
    MESSAGE_CLASS_CREATED(PlaceHolder);

    m_Flags.ModelBound = false;
    m_Flags.CreateCollisionNode = false;
    m_Flags.CollisionPivot = -1;

    m_BoundRadius = 1.0f;
    m_PlaceholderCollisionPivot = nullptr;
}

const float PlaceHolder::GetBoundRadius() const
{
    return m_BoundRadius;
}

void PlaceHolder::SetBoundRadius(const float radius)
{
    StoreProperty(15, &m_BoundRadius, tNUMBER);
    m_BoundRadius = radius;

    if (m_QuadTree)
        m_QuadTree->Refresh();
}

const bool PlaceHolder::IsUsingModelBound() const
{
    return m_Flags.ModelBound;
}

void PlaceHolder::UseModelBound(const bool use)
{
    const bool oldValue = m_Flags.ModelBound;
    StoreProperty(14, &oldValue, tTRUTH);

    m_Flags.ModelBound = use;

    if (m_QuadTree)
        m_QuadTree->Refresh();
}

const bool PlaceHolder::ShouldCreateCollisionNode() const
{
    return m_Flags.CreateCollisionNode;
}

void PlaceHolder::CreateCollisionNode(const bool create)
{
    m_Flags.CreateCollisionNode = create;
    TryInstantiate();
}

const int PlaceHolder::GetCollisionPivot() const
{
    if (!m_ModelRes.m_AssetPtr || m_Flags.CollisionPivot < ((ModelAsset*)m_ModelRes.m_AssetPtr)->m_MeshList.size())
        return m_Flags.CollisionPivot;

    LogDump::LogA("Placeholder ID %d, '%s':\nCollisionPivot is out of bounds. The modelres has probably changed.\n", m_Id.Id, m_Name);
    return -1;
}

void PlaceHolder::SetCollisionPivot(const short pivotid)
{
    const short oldpivotValue = m_Flags.CollisionPivot;
    StoreProperty(13, &oldpivotValue, tINTEGER);

    m_Flags.CollisionPivot = pivotid;
    SetCollisionPivotFromModelRes();
}

void PlaceHolder::Register()
{
    tPlaceholder = new EntityType("PlaceHolder");
    tPlaceholder->InheritFrom(tModel);
    tPlaceholder->SetCreator((CREATOR)Create);

    tPlaceholder->RegisterProperty(tNUMBER, "boundradius", (EntityGetterFunction)&GetBoundRadius, (EntitySetterFunction)&SetBoundRadius, "control=slider|min=0|max=50|step=0.5", 15);
    tPlaceholder->RegisterProperty(tTRUTH, "usemodelbound", (EntityGetterFunction)&IsUsingModelBound, (EntitySetterFunction)&UseModelBound, nullptr, 14);
    tPlaceholder->RegisterProperty(tTRUTH, "createcollisionnode", (EntityGetterFunction)&ShouldCreateCollisionNode, (EntitySetterFunction)&CreateCollisionNode, "control=truth|name=");
    tPlaceholder->RegisterProperty(tINTEGER, "collisionpivot", (EntityGetterFunction)&GetCollisionPivot, (EntitySetterFunction)&SetCollisionPivot, "control=slider|min=0|max=60|step=1", 13);

    GetPlaceholderModelCommand = RegisterGlobalCommand("get_placeholder_model:entity", true);

    tPlaceholder->PropagateProperties();
}

void PlaceHolder::RemoveSiblings()
{
    if (!m_FirstChild)
        return;

    Node* child = m_FirstChild;
    Node* sibling = nullptr;
    do
    {
        Node* sibling = child->m_NextSibling;
        if (child != m_PlaceholderCollisionPivot && child->m_ScriptEntity == tNode)
        {
            child->SetParent(nullptr);
            child->Destroy();
        }

        child = sibling;
    } while (sibling);
}

#pragma message(TODO_IMPLEMENTATION)
void PlaceHolder::SetCollisionPivotFromModelRes()
{
    if (m_Flags.CollisionPivot < 0 || !m_ModelRes.m_AssetPtr || m_Flags.CollisionPivot > ((ModelAsset*)m_ModelRes.m_AssetPtr)->m_MeshList.size() || !m_PlaceholderCollisionPivot)
        return;
}

PlaceHolder* PlaceHolder::Create(AllocatorIndex)
{
    return new PlaceHolder;
}