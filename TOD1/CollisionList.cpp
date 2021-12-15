#include "CollisionList.h"
#include "Entity.h"
#include "Scene.h"
#include "BuiltinType.h"

CollisionList::CollisionList(Entity* owner)
{
    MESSAGE_CLASS_CREATED(CollisionList);

    m_Owner = owner;
    field_78 = 10'000;
    m_GlobalIndex = -1;
    
    Scene::SceneInstance->AddCollisionList(this);

    m_Position_2 = { BuiltinType::ZeroVector.x, BuiltinType::ZeroVector.y, BuiltinType::ZeroVector.z };
    m_Position_3 = { BuiltinType::ZeroVector.x, BuiltinType::ZeroVector.y, BuiltinType::ZeroVector.z };
    m_SafePos = (Vector4f)BuiltinType::Orient;
    m_Unknown_1 = (Vector4f)BuiltinType::Orient;
}

CollisionList::~CollisionList()
{
    MESSAGE_CLASS_DESTROYED(CollisionList);

    Scene::SceneInstance->RemoveCollisionList(this);
}

void CollisionList::SetListGlobalIndex(const int index)
{
    m_GlobalIndex = index;
}