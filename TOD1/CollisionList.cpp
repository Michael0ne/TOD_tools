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

    m_SafePosition = { BuiltinType::ZeroVector.x, BuiltinType::ZeroVector.y, BuiltinType::ZeroVector.z };
    m_Position_3 = { BuiltinType::ZeroVector.x, BuiltinType::ZeroVector.y, BuiltinType::ZeroVector.z };
    m_SafeOrientation = BuiltinType::Orient;
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

void CollisionList::CommitCollision()
{
    DirectX::XMMATRIX mat;
    ((Node*)m_Owner)->GetWorldMatrix(mat);

    m_SafePosition = *(Vector3f*)&mat.r[3];
    DirectX::XMVECTOR rotvec = DirectX::XMQuaternionRotationMatrix(mat);
    m_SafeOrientation = *(Orientation*)&rotvec;

    Scene::SceneInstance->_894810((field_78 * 2) >> 17, 0);
}