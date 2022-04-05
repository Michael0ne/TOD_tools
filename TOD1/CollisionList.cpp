#include "CollisionList.h"
#include "Entity.h"
#include "Scene.h"
#include "BuiltinType.h"
#include "CollisionProbe.h"

CollisionInfo::CollisionInfo(Entity* owner)
{
    MESSAGE_CLASS_CREATED(CollisionInfo);

    m_Owner = owner;
    field_78 = 10'000;
    m_GlobalIndex = -1;
    
    Scene::SceneInstance->AddCollisionList(this);

    m_SafePosition = { BuiltinType::ZeroVector.x, BuiltinType::ZeroVector.y, BuiltinType::ZeroVector.z };
    m_Position_3 = { BuiltinType::ZeroVector.x, BuiltinType::ZeroVector.y, BuiltinType::ZeroVector.z };
    m_SafeOrientation = BuiltinType::Orient;
    m_Unknown_1 = (Vector4f)BuiltinType::Orient;
}

CollisionInfo::~CollisionInfo()
{
    MESSAGE_CLASS_DESTROYED(CollisionInfo);

    Scene::SceneInstance->RemoveCollisionList(this);
}

void CollisionInfo::SetListGlobalIndex(const int index)
{
    m_GlobalIndex = index;
}

void CollisionInfo::CommitCollision()
{
    DirectX::XMMATRIX mat;
    ((Node*)m_Owner)->GetWorldMatrix(mat);

    m_SafePosition = *(Vector3f*)&mat.r[3];
    DirectX::XMVECTOR rotvec = DirectX::XMQuaternionRotationMatrix(mat);
    m_SafeOrientation = *(Orientation*)&rotvec;

    Scene::SceneInstance->_894810((field_78 * 2) >> 17, 0);
}

void CollisionInfo::RemoveNode(Entity* node)
{
    std::vector<CollisionProbe*>::iterator it = std::find(m_CollisionProbesList.begin(), m_CollisionProbesList.end(), node);
#ifdef INCLUDE_FIXES
    if (it == m_CollisionProbesList.end())
    {
        debug("RemoveNode failed! Node %x not found!\n", node);
        return;
    }
#endif
    m_CollisionProbesList.erase(it);
}

void CollisionInfo::AddNode(Entity* node)
{
    if (!node)
        return;

    m_CollisionProbesList.push_back((CollisionProbe*)node);
}