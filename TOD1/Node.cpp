#include "Node.h"
#include "Fragment.h"
#include "AssetManager.h"
#include "LogDump.h"
#include "CollisionList.h"
#include "Scene.h"
#include "TransactionBuffer.h"
#include "SceneSaveLoad.h"
#include "VectorType.h"
#include "Camera.h"
#include "BuiltinType.h"
#include "ScriptThread.h"
#include "StringType.h"
#include "IntegerType.h"
#include "NumberType.h"
#include "TruthType.h"
#include "QuaternionType.h"
#include "CollisionProbe.h"
#include "GfxInternal.h"

EntityType* tNode;
std::vector<Node::NodeInfo> Node::NodesWithUpdateOrBlockingScripts;
std::vector<Node::NodePosInfo> Node::NodesList;
String Node::IgnoredCollisionNodes;

NodeMatrix::NodeMatrix(Node* owner)
{
    MESSAGE_CLASS_CREATED(NodeMatrix);

    m_Orientation = Orientation();
    m_Position = Vector4f();
    m_Owner = owner;
}

NodeMatrix::~NodeMatrix()
{
    MESSAGE_CLASS_DESTROYED(NodeMatrix);
}

void NodeMatrix::GetMatrixForNode(DirectX::XMMATRIX& outMat)
{
    if (m_Owner->m_Id.HasQuadTree)
        ApplyMatrixFromQuadTree();
    GetMatrix(outMat);
}

#pragma message(TODO_IMPLEMENTATION)
void NodeMatrix::ApplyMatrixFromQuadTree()
{
}

void NodeMatrix::GetMatrix(DirectX::XMMATRIX& outMat) const
{
    outMat.r[0] = *(DirectX::XMVECTOR*)&m_RightVector;
    outMat.r[1] = *(DirectX::XMVECTOR*)&m_UpVector;
    outMat.r[2] = *(DirectX::XMVECTOR*)&m_ForwardVector;
    outMat.r[3] = *(DirectX::XMVECTOR*)&m_PositionVector;
}

void NodeMatrix::SetTransformationFromMatrix(const DirectX::XMMATRIX& mat)
{
    m_RightVector  = *(Vector4f*)&mat.r[0];
    m_UpVector   = *(Vector4f*)&mat.r[1];
    m_ForwardVector  = *(Vector4f*)&mat.r[2];
    m_PositionVector = *(Vector4f*)&mat.r[3];
}

#pragma message(TODO_IMPLEMENTATION)
void Node::Destroy()
{
    if (m_FirstChild)
        for (auto child = m_FirstChild->m_NextSibling; child; child = child->m_NextSibling)
            child->Destroy();
    m_FirstChild = nullptr;

    SetParent(nullptr);

    delete m_Fragment;
    
    if (m_Position && m_Position->m_Owner == this)
        delete m_Position;

    delete m_QuadTree;
    delete m_CollisionIgnoreList;
    delete[] m_Name;

    if (m_GlobalIdInBlockigList >= 0)
    {
        NodesWithUpdateOrBlockingScripts[m_GlobalIdInBlockigList].m_Flags.m_Id = NULL;
        m_GlobalIdInBlockigList = -1;
    }

    if (m_GlobalIdInSceneList >= 0)
    {
        NodesList[m_GlobalIdInSceneList].m_QuadTree = 0;
        m_GlobalIdInSceneList = -1;
    }

    (*(void(__thiscall*)(Entity*, bool))(*(int*)this))(this, true);
}

void Node::_484CC0(int)
{
    m_Id.HasQuadTree = true;
    m_Id._3 = 1;
}

#pragma message(TODO_IMPLEMENTATION)
Entity* Node::FindNode(const char* nodeName) const
{
#ifdef _EXE
    LogDump::LogA("Node::FindNode NOT IMPLEMENTED!\n");

    return nullptr;
#else
    return (*(Entity * (__thiscall*)(const Node*, const char*))0x88EED0)(this, nodeName);
#endif
}

void Node::FindNode_Impl(int* args) const
{
    *args = (int)FindNode((const char*)args[1]);
}

#pragma message(TODO_IMPLEMENTATION)
void Node::ConvertFromWorldSpace(Vector4f& outPos, const Vector4f& inPos)
{
    //  TODO: Get world matrix, transpose and return new translated position.
}

#pragma message(TODO_IMPLEMENTATION)
void Node::SetWorldOrient(const Orientation& orientation)
{
}

void Node::_86B4B0(const int size)
{
    if (m_Flags.m_FlagBits.HasFragment || m_Flags.m_FlagBits.Volatile)
        return;

    const unsigned int block = 2 * (size & 15);
    const unsigned int slot = m_Parameters[size / 16] >> block;

    if ((slot & 3) != 3)
    {
        if ((slot & 1) == 0 && Scene::_A3CEE8)
            _86A930(size, &m_Parameters[m_ScriptEntity->m_Script->m_PropertiesList[size].m_Offset], &m_Parameters[size / 16], 1 << block);
        if ((slot & 2) == 0)
            _86AA10(size, &m_Parameters[m_ScriptEntity->m_Script->m_PropertiesList[size].m_Offset], &m_Parameters[size / 16], 2 * (1 << block));
    }
}

void Node::_86A930(const int size, int* value, int* const outval, const int a4)
{
    EntityType* ent = m_ScriptEntity->m_IsBaseEntity ? m_ScriptEntity->m_Parent : m_ScriptEntity;
    *Scene::_A3CEE8++ = 
        ( (m_Id.Id << 8) | ( (*(short*)&m_Id & 0x7000) - 1 ) & 0xF000 ) |
        ( (short)size + (short)ent->m_LocalPropertiesList.size() + (short)ent->field_6C) & 0xFFF;
    Scene::_A3CEE8 += ent->m_Script->m_PropertiesList[size].m_Info->m_PropertyType->stub9((char*)value, (char*)Scene::_A3CEE8);
    if (((int)Scene::SceneInstance->m_RewindBuffer2->m_Buffer + 4 * Scene::SceneInstance->m_RewindBuffer2->m_Chunks - (int)Scene::_A3CEE8) < 0x4000)
        Scene::SceneInstance->m_RewindBuffer2->_8AA1F0(&Scene::_A3CEE8);

    *outval |= a4;
}

#pragma message(TODO_IMPLEMENTATION)
void Node::_86AA10(const int size, int* value, int* const outval, const int a4)
{
}

void Node::RotateLocalX(const float x)
{
    const Orientation orient = &m_Position->m_Orientation ? m_Position->m_Orientation : BuiltinType::Orient;
    const float halfnegx = x * -0.5f;
    const float cosneghalfx = cosf(halfnegx);
    const float sinneghalfx = sinf(halfnegx);

    const Orientation newOrient =
    {
        (orient.w * cosneghalfx) + (orient.z * sinneghalfx),
        (orient.x * cosneghalfx) - (orient.y * sinneghalfx),
        (orient.y * cosneghalfx) + (orient.x * sinneghalfx),
        (orient.z * cosneghalfx) - (orient.w * sinneghalfx)
    };

    SetOrient(newOrient);
}

void Node::RotateLocalY(const float y)
{
    const Orientation orient = &m_Position->m_Orientation ? m_Position->m_Orientation : BuiltinType::Orient;
    const float halfnegy = y * -0.5f;
    const float cosneghalfy = cosf(halfnegy);
    const float sinneghalfy = sinf(halfnegy);

    const Orientation newOrient =
    {
        (orient.w * cosneghalfy) + (orient.z * sinneghalfy),
        (orient.x * cosneghalfy) + (orient.y * sinneghalfy),
        (orient.y * cosneghalfy) - (orient.w * sinneghalfy),
        (orient.z * cosneghalfy) - (orient.x * sinneghalfy)
    };

    SetOrient(newOrient);
}

void Node::RotateLocalZ(const float z)
{
    const Orientation orient = &m_Position->m_Orientation ? m_Position->m_Orientation : BuiltinType::Orient;
    const float halfnegz = z * -0.5f;
    const float cosneghalfz = cosf(halfnegz);
    const float sinneghalfz = sinf(halfnegz);

    const Orientation newOrient =
    {
        (orient.w * cosneghalfz) + (orient.x * sinneghalfz),
        (orient.x * cosneghalfz) + (orient.w * sinneghalfz),
        (orient.y * cosneghalfz) - (orient.z * sinneghalfz),
        (orient.z * cosneghalfz) - (orient.y * sinneghalfz)
    };

    SetOrient(newOrient);
}

void Node::ConvertToWorldSpace(Vector4f& outPos, const Vector4f& inPos)
{
    if (m_Position)
    {
        if (m_Id.HasQuadTree)
            m_Position->ApplyMatrixFromQuadTree();

        DirectX::XMMATRIX mat;
        m_Position->GetMatrix(mat);

        outPos =
        {
            (((inPos.x * mat.r[0].m128_f32[0]) + (inPos.y * mat.r[1].m128_f32[0])) + (mat.r[2].m128_f32[0])) + mat.r[3].m128_f32[0],
            (((inPos.y * mat.r[1].m128_f32[1]) + (inPos.x * mat.r[0].m128_f32[1])) + (mat.r[2].m128_f32[1])) + mat.r[3].m128_f32[1],
            (((inPos.y * mat.r[1].m128_f32[2]) + (inPos.x * mat.r[0].m128_f32[2])) + (mat.r[2].m128_f32[2])) + mat.r[3].m128_f32[2],
            inPos.a
        };
    }
}

void Node::SetRotationX(const float x)
{
    const Orientation orient =
    {
        sinf(x * -0.5f),
        0,
        0,
        cosf(x * -0.5f)
    };

    SetOrient(orient);
}

void Node::SetRotationY(const float y)
{
    const Orientation orient =
    {
        0,
        sinf(y * -0.5f),
        0,
        cosf(y * -0.5f)
    };

    SetOrient(orient);
}

void Node::SetRotationZ(const float z)
{
    const Orientation orient =
    {
        0,
        0,
        sinf(z * -0.5f),
        cosf(z * -0.5f)
    };

    SetOrient(orient);
}

void Node::TouchThisPivot(const int)
{
    _484CC0(0);
}

void Node::IsSuspended(bool* suspended) const
{
    *suspended = field_20 &&
        field_20->m_ScriptThread &&
        field_20->m_ScriptThread->m_ThreadFlags.Suspended != false;
}

void Node::IsDisabled(bool* disabled) const
{
    *disabled = m_Flags.m_FlagBits._16 | m_Flags.m_FlagBits.Disable;
}

const int Node::GetUserType() const
{
    return m_QuadTree ? m_QuadTree->m_UserType : NULL;
}

const char Node::GetLod() const
{
    return m_QuadTree ? m_QuadTree->m_Lod : NULL;
}

const short Node::GetOrder() const
{
    return m_Order;
}

void Node::SetOrder(const short order)
{
    m_Order = order;

    if (m_Parent)
    {
        SetChildrenPositionToSame();
        SetParent(m_Parent);
    }
}

void Node::SetChildrenPositionToSame()
{
    if (!m_Position || m_Position->m_Owner != this)
    {
        m_Position = nullptr;
        for (Node* child = m_FirstChild; child; child = child->m_NextSibling)
            child->SetEntityPositionRecursively(nullptr);
    }

    SetAndUpdateChildrenAuxQuadTree(nullptr);

    if (m_Parent->m_FirstChild == this)
    {
        m_Parent->m_FirstChild = m_NextSibling;
        m_Parent = nullptr;
    }
    else
    {
        Node* parentsfirstchild = m_Parent->m_FirstChild;
        Node* parentssibling = parentsfirstchild->m_NextSibling;

        if (parentssibling)
        {
            while (parentssibling != this)
            {
                parentsfirstchild = parentssibling;
                parentssibling = parentssibling->m_NextSibling;
                if (!parentssibling)
                    return;
            }

            parentsfirstchild->m_NextSibling = parentssibling->m_NextSibling;
            m_Parent = nullptr;
        }
    }
}

#pragma message(TODO_IMPLEMENTATION)
void Node::SetAndUpdateChildrenAuxQuadTree(AuxQuadTree* qdtree)
{
    if (m_QuadTree)
    {
        //  TODO: methods implementation for QuadTree.
    }
    else
    {
        for (Node* child = m_FirstChild; child; child = child->m_NextSibling)
            child->SetAndUpdateChildrenAuxQuadTree(qdtree);
    }
}

void Node::SetEntityPositionRecursively(NodeMatrix* position)
{
    if (!m_Position || m_Position->m_Owner != this)
    {
        m_Position = position;
        for (Node* child = m_FirstChild; child; child = child->m_NextSibling)
            child->SetEntityPositionRecursively(position);
    }
}

const int Node::GetRenderOrderGroup() const
{
    return m_Flags.m_FlagBits.RenderOrderGroup;
}

void Node::SetRenderOrderGroup(int renderordergroup)
{
    if (renderordergroup > -1)
    {
        if (renderordergroup > 4)
            renderordergroup = 4;
    }
    else
        renderordergroup = -1;

    m_Flags.m_FlagBits.RenderOrderGroup = renderordergroup;
    m_Id._3 = true;
}

const bool Node::ShouldDisableOnCutscene() const
{
    return m_Flags.m_FlagBits.DisabledOnCutscene;
}

void Node::SetShouldDisableOnCutscene(const bool disable)
{
    m_Flags.m_FlagBits.DisabledOnCutscene = disable;
}

const int Node::GetRepresentation() const
{
    int representation = m_Position != nullptr;

    if (m_QuadTree)
        representation |= 2;

    if (m_Flags.m_FlagBits.HasFragment)
        representation |= 4;

    return representation;
}

void Node::GetOrient(Orientation& orientation) const
{
    orientation = m_Position ? m_Position->m_Orientation : BuiltinType::Orient;
}

const bool Node::ShouldUseAuxQuadTree() const
{
    return m_QuadTree ? (m_QuadTree->field_4D >> 6) & 1 : false;
}

void Node::SetShouldUseAuxQuadTree(const bool use)
{
    if (!m_QuadTree)
        return;

    const bool flag = (m_QuadTree->field_4D >> 6) & 1;
    SetParam(5, &flag, tTRUTH);
    m_QuadTree->_8A36A0(use);
}

const char* const Node::GetName() const
{
    return m_Name ? m_Name : nullptr;
}

void Node::SetName(const char* const name)
{
    if (m_Name)
        delete[] m_Name;

    if (!name)
        return;

    m_Name = new char[53];
    strcpy(m_Name, name);
}

const float Node::GetLodThreshold() const
{
    return m_QuadTree ? (m_QuadTree->field_3C * 0.0049999999f) : 0.f;
}

void Node::SetLodThreshold(float threshold)
{
    if (!m_QuadTree)
        return;

    const float thresholdcurrent = m_QuadTree->field_3C * 0.0049999999f;
    SetParam(8, &thresholdcurrent, tNUMBER);

    if (threshold > 1)
        threshold = 1;

    if (threshold < 0)
        threshold = 0;

    m_QuadTree->field_3C = (char)(threshold * 200.0f);
    _88BA60();
}

#pragma message(TODO_IMPLEMENTATION)
void Node::_88BA60()
{
}

const float Node::GetFadeThreshold() const
{
    return m_QuadTree ? (m_QuadTree->field_3D & 127) * 0.0099999998f : 0.f;
}

void Node::SetFadeThreshold(float threshold)
{
    if (!m_QuadTree)
        return;

    const float fadethreshold = (m_QuadTree->field_3D & 127) * 0.0099999998f;
    SetParam(9, &fadethreshold, tNUMBER);

    if (threshold < 0)
        threshold = 0;

    if (threshold > 1)
        threshold = 1;

    m_QuadTree->field_3D ^= (m_QuadTree->field_3D ^ (unsigned int)(threshold * 100.0f)) & 127;
    _88BAA0();
}

#pragma message(TODO_IMPLEMENTATION)
void Node::_88BAA0()
{
}

const bool Node::ShouldSlowFade() const
{
    return m_QuadTree ? (m_QuadTree->field_3D >> 7) : false;
}

void Node::SetShouldSlowFade(const bool slowfade)
{
    if (m_QuadTree)
        m_QuadTree->field_3D = (slowfade << 7) | m_QuadTree->field_3D & 127;
}

const float Node::GetTraverseDistance() const
{
    return m_QuadTree ? m_QuadTree->m_TraverseDistance : 0.f;
}

void Node::SetTraverseDistance(const float distance)
{
    if (!m_QuadTree)
        return;

    if (distance < 0)
        m_QuadTree->m_TraverseDistance = 0;

    if (distance > 65535)
        m_QuadTree->m_TraverseDistance = -1;
    else
        m_QuadTree->m_TraverseDistance = distance;
}

Entity* Node::GetScene() const
{
    return Scene::SceneInstance;
}

Entity* Node::GetFirstChild() const
{
    return m_FirstChild;
}

Entity* Node::GetNextSibling() const
{
    return m_NextSibling;
}

const float Node::GetLodDistance() const
{
    return m_QuadTree ? sqrtf((float)(unsigned int)m_QuadTree->m_LodDistance) : 0.f;
}

const float Node::GetLodFade() const
{
    return m_QuadTree ? m_QuadTree->m_LodFade * 0.0039215689f : 0.f;
}

const bool Node::GetIsTaggedForUnload() const
{
    Folder_* parentfolder = FindParentFolder();
    if (!parentfolder || parentfolder == this)
        return false;
    else
        return (parentfolder->m_BlockId >> 30) & 1;
}

Folder_* Node::FindParentFolder() const
{
    Node* node = (Node*)this;
    while (true)
    {
        if (!node)
        {
            node = node->m_Parent;
            if (!node)
                return nullptr;

            continue;
        }

        EntityType* folderscript = m_ScriptEntity;
        if (!folderscript)
        {
            node = node->m_Parent;
            if (!node)
                return nullptr;

            continue;
        }

        while (folderscript != tFolder)
        {
            folderscript = folderscript->m_Parent;
            if (!folderscript)
            {
                node = node->m_Parent;
                if (!node)
                    return nullptr;
            }
        }

        if (!node->m_Parent || ((Folder_*)node)->GetBlockId() > 0)
            return (Folder_*)node;

        node = node->m_Parent;
        if (!node)
            return nullptr;
    }
}

const int Node::GetUniqueId0() const
{
    return m_Fragment ? m_Fragment->m_UniqueId.m_Id.m_Id1 : NULL;
}

void Node::SetUniqueId0(const int id)
{
    if (m_Fragment)
        m_Fragment->m_UniqueId.m_Id.m_Id1 = id;
}

const int Node::GetUniqueId1() const
{
    return m_Fragment ? m_Fragment->m_UniqueId.m_Id.m_Id2 : NULL;
}

void Node::SetUniqueId1(const int id)
{
    if (m_Fragment)
        m_Fragment->m_UniqueId.m_Id.m_Id2 = id;
}

void Node::SetUserType(const int type)
{
    if (m_QuadTree)
    {
        SetParam(6, &m_QuadTree->m_UserType, tINTEGER);
        m_QuadTree->m_UserType ^= (type ^ m_QuadTree->m_UserType) & 0xFFFFFF;
    }
}

const char* const Node::GetIgnoreList() const
{
    if (!m_CollisionIgnoreList)
        return nullptr;

    if (!m_CollisionIgnoreList->m_CollisionProbesList.size())
        return IgnoredCollisionNodes.m_Str;

    char buffer[12] = {};
    for (int i = 0; i < m_CollisionIgnoreList->m_CollisionProbesList.size(); ++i)
    {
        if (!m_CollisionIgnoreList->m_CollisionProbesList[i])
            continue;

        sprintf(buffer, "%010d;", m_CollisionIgnoreList->m_CollisionProbesList[i]->m_Id.Id);
        IgnoredCollisionNodes.Append(buffer);

        memset(buffer, NULL, 12);
    }

    return IgnoredCollisionNodes.m_Str;
}

#pragma message(TODO_IMPLEMENTATION)
void Node::SetIgnoreList(const char* list)
{
    if (!m_CollisionIgnoreList || !list)
        return;

    m_CollisionIgnoreList->m_CollisionProbesList.clear();
    const size_t listlen = strlen(list);
    const size_t blocksize = 11;

    if (listlen < blocksize)
        return;

    for (unsigned int i = listlen / blocksize; i; --i)
    {
        //  TODO: find probe by it's id and add it to the list.
        CollisionProbe* colprobe;
        m_CollisionIgnoreList->m_CollisionProbesList.push_back(colprobe);
        list += blocksize;
    }
}

void Node::Move(const Vector4f& pos)
{
    SetNewPos(pos);
}

void Node::SetNewPos(const Vector4f& pos)
{
    Vector4f nodepos;
    GetPos(nodepos);

    SetPos({ nodepos.x + pos.x, nodepos.y + pos.y, nodepos.z + pos.z, nodepos.a });
}

void Node::MoveLocal(const Vector4f& pos)
{
    MoveLocal_Impl(pos);
}

#pragma message(TODO_IMPLEMENTATION)
void Node::MoveLocal_Impl(const Vector4f& pos)
{
}

void Node::GetBlockIdBelow(int* outBlockId) const
{
    *outBlockId = m_Id.BlockId - 1;

    if (!m_ScriptEntity)
        return;

    EntityType* folderent = (EntityType*)m_ScriptEntity;
    while (tFolder != folderent)
    {
        folderent = folderent->m_Parent;
        if (!folderent)
            return;
    }

    if (((Folder_*)this)->GetBlockId())
        *outBlockId = ((Folder_*)this)->GetBlockId();
}

void Node::GetBlockId(int* outBlockId) const
{
    *outBlockId = m_Id.BlockId;
}

void Node::SetOrientFast_Impl(const Orientation& orient)
{
    if (!m_Position)
        return;

    if (m_Position->m_Owner != this)
        return;

    const float deltaW = fabsf(orient.w - m_Position->m_Orientation.w);
    const float deltaX = fabsf(orient.x - m_Position->m_Orientation.x);
    const float deltaY = fabsf(orient.y - m_Position->m_Orientation.y);
    const float deltaZ = fabsf(orient.z - m_Position->m_Orientation.z);
    float deltaMin = 0;

    //  NOTE: maybe a better way to do this? Minimum out of four.
    if (deltaW >= 0)
        deltaMin = deltaW;

    if (deltaMin <= deltaX)
        deltaMin = deltaX;

    if (deltaMin <= deltaY)
        deltaMin = deltaY;

    if (deltaMin <= deltaZ)
        deltaMin = deltaZ;

    if (0 < deltaMin)
    {
        SetParam(4, &m_Position, tQUATERNION);
        if (m_CollisionIgnoreList)
            m_CollisionIgnoreList->m_Orientation = orient;

        m_Position->m_Orientation = orient;
    }
}

void Node::FastSetOrient(const Orientation& orient)
{
    SetOrientFast_Impl(orient);
}

void Node::SetPosFast_Impl(const Vector4f& pos)
{
    if (!m_Position)
        return;

    if (m_Position->m_Owner != this)
        return;

    if (m_Position->m_Position == pos)
        return;

    SetParam(3, &m_Position->m_Position, tVECTOR);
    if (m_CollisionIgnoreList)
    {
        const float dist =
            ( (pos.z - m_CollisionIgnoreList->m_Position_1.z) * (pos.z - m_CollisionIgnoreList->m_Position_1.z) ) +
            ( (pos.y - m_CollisionIgnoreList->m_Position_1.y) * (pos.y - m_CollisionIgnoreList->m_Position_1.y) ) +
            ( (pos.x - m_CollisionIgnoreList->m_Position_1.x) * (pos.x - m_CollisionIgnoreList->m_Position_1.x) );
        if (dist > 100)
            m_CollisionIgnoreList->m_Position_1 = { pos.x, pos.y, pos.z };
    }

    m_Position->m_Position = pos;
}

void Node::PurgeNames(const int dummy)
{
    PurgeNames_Impl(false);
}

void Node::PurgeNames_Impl(bool onlyChildren)
{
    if (m_Flags.m_FlagBits.PurgeNames)
        onlyChildren = true;

    if (onlyChildren)
        delete[] m_Name;

    for (Node* child = m_FirstChild; child; child = child->m_NextSibling)
        PurgeNames_Impl(onlyChildren);
}

void Node::ResetIgnoreList()
{
    if (!m_CollisionIgnoreList)
        return;

    const char* const ignorelist = GetIgnoreList();
    SetParam(7, &ignorelist, tSTRING);

    m_CollisionIgnoreList->m_CollisionProbesList.clear();
}

void Node::ForceLodCalculation()
{
    if (m_QuadTree)
        m_QuadTree->UpdateLodDistance();
}

//  TODO: find a better way.
void Node::GetPlatform(int* args) const
{
    static const int PlatformNumber[3] =
    {
        0, 1, 2
    };

#ifdef INCLUDE_FIXES
    #ifdef PLATFORM_PC
        *args = PlatformNumber[0];
    #endif
    #ifdef PLATFORM_PS2
        *args = PlatformNumber[1];
    #endif
    #ifdef PLATFORM_XBOX
        *args = PlatformNumber[2];
    #endif
#else
    *args = NULL;
#endif
}

void Node::GetScreenSize(Vector3f& size) const
{
    ScreenResolution resolution;
    g_GfxInternal->GetScreenResolution(resolution);

    size = { (float)resolution.x, (float)resolution.y, 0 };
}

void Node::GetSharedProbe(CollisionProbe** probe) const
{
    *probe = Scene::SceneInstance->m_SharedProbe;
}

void Node::GetCurrentCamera(Camera** camera) const
{
    *camera = Scene::SceneInstance->m_ActiveCamera;
}

void Node::SetCurrentCamera(Camera** camera) const
{
    Scene::SceneInstance->m_GameCamera = *camera;
    Scene::SceneInstance->StoreGameCamera();
}

void Node::CreateNode(int* args) const
{
    *args = (int)DataType::GetScriptEntityByName((const char*)args[1])->CreateNode();
}

void Node::CommitCollision()
{
    if (m_CollisionIgnoreList)
        m_CollisionIgnoreList->CommitCollision();
}

void Node::SetSafePos(const Vector4f& pos, const Orientation& orient)
{
    if (!m_CollisionIgnoreList)
        return;

    m_CollisionIgnoreList->m_SafePosition = { pos.x, pos.y, pos.z };
    m_CollisionIgnoreList->m_SafeOrientation = orient;
}

void Node::SetFlags(int flags)
{
    if (!m_Flags.m_FlagBits.Volatile && (flags & 0x20) == 0 && flags != (m_Flags.m_Flags & 0xFFF))
    {
        unsigned int _flags = m_Flags.m_Flags & 0xFFF;
        SetParam(2, &_flags, tINTEGER);
    }

    if (((flags ^ m_Flags.m_FlagBits.Dynamic) & 2) != 0)
    {
        if (m_QuadTree && m_QuadTree->field_38)
        {
            CollisionList* collst = _8A0810(this)->m_Owner->m_CollisionIgnoreList;
            if (collst)
                collst->field_78 |= 0x80000000;
        }
    }

    m_Flags.m_Flags ^= (flags ^ (unsigned short)m_Flags.m_Flags) & 0xFFF;
    _88E6A0(this);

    if (m_QuadTree || (m_Parent && m_Parent->GetEntityQuadTreeOrParentQuadTree()))
        m_Parent->GetEntityQuadTreeOrParentQuadTree()->m_Owner->m_Id._3 = true;
}

void Node::Instantiate()
{
    if (m_CollisionIgnoreList)
        m_CollisionIgnoreList->field_78 |= 0x80000000;

    m_Id.HasPosition = false;
    m_Id.HasQuadTree = true;
    m_Id.HasFragment = true;
    m_Id._3 = 31;   //  TODO: what is this?

    if (m_QuadTree)
        m_QuadTree->Refresh();

    m_Id._3 = 8;
}

void Node::Update()
{
    return;
}

#pragma message(TODO_IMPLEMENTATION)
void Node::_88C300()
{
    *(char*)0xA3D890 = (char)1;
}

void Node::Render()
{
    return;
}

char Node::ProcessCollision(int, int)
{
    return 0;
}

float Node::_8F8650(int, int)
{
    return -1.0f;
}

void Node::nullsub_3(int)
{
    return;
}

#pragma message(TODO_IMPLEMENTATION)
void Node::ExecuteScript()
{
    (*(void(__thiscall*)(void*, Node*))0x86CFF0)(m_ScriptEntity, this);
}

void Node::nullsub_4(int)
{
    return;
}

void Node::DestroyFrameBuffers()
{
    return;
}

void Node::nullsub_6(const std::vector<Node*>)
{
    return;
}

String* Node::GetResourceName(String* unk)
{
    *unk = String();

    return unk;
}

Vector4f* Node::GetBounds(Vector4f& unk) const
{
    return (unk = Vector4f(), &unk);
}

#pragma message(TODO_IMPLEMENTATION)
Node::Node(unsigned char allocationBitmask)
{
    MESSAGE_CLASS_CREATED(Node);

    m_Flags.m_FlagBits.HasFragment = true;

    m_GlobalIdInSceneList = NULL;
    m_QuadTree = nullptr;
    m_NextSibling = nullptr;
    m_CollisionIgnoreList = nullptr;
    m_Position = nullptr;
    m_Parent = nullptr;
    m_FirstChild = nullptr;
    m_Fragment = nullptr;
    m_Name = nullptr;

    m_NodePosition = new NodePosition();

    if (allocationBitmask & NODE_MASK_POSITION)
        m_Position = new NodeMatrix(this);

    if (allocationBitmask & NODE_MASK_QUADTREE)
        m_QuadTree = new AuxQuadTree(this);

    if (allocationBitmask & NODE_MASK_FRAGMENT)
        m_Fragment = new Fragment(this);

    m_Flags.m_FlagBits.HasFragment = m_Fragment != nullptr;

    m_GlobalIdInSceneList = m_GlobalIdInSceneList | 0xFFFFFFFF;
}

#pragma message(TODO_IMPLEMENTATION)
Node::~Node()
{
    MESSAGE_CLASS_DESTROYED(Node);
}

const char* Node::GetTypename() const
{
    return m_ScriptEntity->m_TypeName.m_Str;
}

const char* Node::GetScript() const
{
    return m_ScriptEntity->m_Script ? m_ScriptEntity->m_Script->m_Name.m_Str : nullptr;
}

unsigned int Node::GetFlags() const
{
    return m_Flags.m_Flags & 0xFFF;
}

void Node::SetParam(const int index, const void* param, DataType* type)
{
    if (!m_ScriptEntity)
        return;

    if (m_Flags.m_FlagBits.HasFragment && m_Flags.m_FlagBits.Volatile)
        return;

    unsigned char paramInd = 1 << (index & 7);
    char* paramsMap = (char*)this + index / 8;

    if ((paramInd & paramsMap[8]) == NULL)
        _869EC0(index, param, *type);
    if ((paramInd & paramsMap[13]) == NULL)
        _869F80(index, param, *type);
}

#pragma message(TODO_IMPLEMENTATION)
void Node::SetOrient(const Orientation& orient)
{
    if (!m_Position)
        return;

    if (m_Position->m_Owner != this)
        return;
}

Vector4f* Node::GetPos(Vector4f& outVec)
{
    if (m_Position)
        return (outVec = Vector4f(m_Position->m_Position.x, m_Position->m_Position.y, m_Position->m_Position.z, m_Position->m_Position.a), &outVec);
    else
        return (outVec = Vector4f(), &outVec);
}

void Node::GetWorldMatrix(DirectX::XMMATRIX& outMat) const
{
    if (m_Position)
        m_Position->GetMatrixForNode(outMat);
    else
        outMat = DirectX::XMMatrixIdentity();
}

Node* Node::GetParent() const
{
    return m_Parent;
}

#pragma message(TODO_IMPLEMENTATION)
void Node::SetParent(const Node* parent)
{
    if (m_Parent == parent)
        return;

    SetParam(1, &m_Parent, tEntity);

    if (parent)
    {
        Node* _pa = (Node*)parent;
        while (true)
        {
            if (_pa->m_Parent == this)
                break;
            _pa = _pa->m_Parent;
            if (!_pa)
                break;
        }

        if (_pa)
            _pa->SetParent(nullptr);
    }

    //if (!m_QuadTree && m_Parent)
        //SetChildrenPositionToSame(); // @88D1E0
}

void Node::DestroyAddon()
{
    if (m_Name && strstr(m_Name, "ADD_"))
    {
        SetParent(nullptr);
        Destroy();
        LogDump::LogA("addon destroyed\n");
    }
    else
        for (Node* node_ = m_FirstChild; node_; node_ = node_->m_NextSibling)
            node_->DestroyAddon();
}

#pragma message(TODO_IMPLEMENTATION)
void Node::SetPos(const Vector4f& newpos)
{
    if (!m_Position)
        return;

    if (m_Position->m_Owner != this)
        return;

    if (m_Position->m_Position.x == newpos.x ||
        m_Position->m_Position.y == newpos.y ||
        m_Position->m_Position.z == newpos.z ||
        Script::_A11C90 == 1.f)
        return;

    SetParam(3, &m_Position, tVECTOR);

    if (m_CollisionIgnoreList)
    {
        if (Script::_A11C90 != 1.f)
        {
            float vecdiffx = newpos.x - m_CollisionIgnoreList->m_Position_1.x;
            float vecdiffy = newpos.y - m_CollisionIgnoreList->m_Position_1.y;
            float vecdiffz = newpos.z - m_CollisionIgnoreList->m_Position_1.z;
            if ( ((vecdiffz * vecdiffz) + (vecdiffy * vecdiffy) + (vecdiffx * vecdiffx)) <= 100.f)
            {
                float negflt = 1.f - Script::_A11C90;
                m_Position->m_Position =
                {
                    (m_CollisionIgnoreList->m_Position_1.x * negflt) + (newpos.x * Script::_A11C90),
                    (m_CollisionIgnoreList->m_Position_1.y * negflt) + (newpos.y * Script::_A11C90),
                    (m_CollisionIgnoreList->m_Position_1.z * negflt) + (newpos.z * Script::_A11C90),
                    0.f
                };

                _484CC0(0);

                //if (m_QuadTree)
                    //m_QuadTree->_8A3320();

                for (Node* sibling = m_FirstChild; sibling; sibling = sibling->m_NextSibling)
                {
                    sibling->_484CC0(0);
                    //if (sibling->m_QuadTree)
                        //sibling->m_QuadTree->_8A3320();

                    for (Node* siblingA = sibling->m_FirstChild; siblingA; siblingA = siblingA->m_NextSibling)
                    {
                        siblingA->_484CC0(0);
                        //siblingA->_88D230(0);
                    }
                }

                if (m_QuadTree ||
                    (m_Parent && (m_QuadTree = m_Parent->GetEntityQuadTreeOrParentQuadTree())))
                    m_QuadTree->m_Owner->_484CC0(0);

                return;
            }
        }

        m_CollisionIgnoreList->m_Position_1 = { newpos.x, newpos.y, newpos.z };
    }
    else
    {
        //if (Camera::PlayerPos && m_ScriptEntity == tBone)
        //{
            // TODO ...
        //}
    }

    m_Position->m_Position = newpos;
    _484CC0(0);

    //if (m_QuadTree)
        //m_QuadTree->_8A3320();

    for (Node* sibling = m_FirstChild; sibling; sibling = sibling->m_NextSibling)
    {
        sibling->_484CC0(0);
        //if (sibling->m_QuadTree)
            //sibling->m_QuadTree->_8A3320();

        for (Node* siblingA = sibling->m_FirstChild; siblingA; siblingA = siblingA->m_NextSibling)
        {
            siblingA->_484CC0(0);
            //siblingA->_88D230(0);
        }
    }

    if (m_QuadTree ||
        (m_Parent && (m_QuadTree = m_Parent->GetEntityQuadTreeOrParentQuadTree())))
        m_QuadTree->m_Owner->_484CC0(0);
}

const char* Node::GetFragment() const
{
    if (!m_Fragment)
        return nullptr;

    if (!m_Flags.m_FlagBits.HasFragment)
        return m_Fragment->m_Name;

    if (m_Fragment->m_FragmentRes)
        return m_Fragment->m_FragmentRes->AddResToOpenListAndReturnName();

    return nullptr;
}

void Node::ForceLodCalculation(unsigned int a1)
{
    if (m_QuadTree)
        m_QuadTree->CalculateLodForAllChildren();
    else
        for (Node* chld = m_FirstChild; chld; chld = chld->m_NextSibling)
            chld->ForceLodCalculation(a1);
}

#pragma message(TODO_IMPLEMENTATION)
void Node::_88E6A0(Node* node)
{
}

AuxQuadTree* Node::GetEntityQuadTreeOrParentQuadTree() const
{
    if (m_QuadTree)
        return m_QuadTree;

    Node* parentnode = m_Parent;
    while (true)
    {
        if (!parentnode)
            break;
        if (parentnode->m_QuadTree)
            return parentnode->m_QuadTree;

        parentnode = parentnode->m_Parent;
    }

    return nullptr;
}

void Node::_869EC0(const unsigned int paramind, const void* paramptr, DataType& paramtype)
{
    if (!Scene::_A3CEE4)
        return;

    *Scene::_A3CEE4++ = paramind & 0xFFF | (m_Id.Id << 8) | (m_Id.BlockId - 1) & 0xF000;
    Scene::_A3CEE4 += paramtype.stub9((char*)paramptr, (char*)Scene::_A3CEE4);

    TransactionBuffer* tb = Scene::SceneInstance->m_RewindBuffer1;
    if (tb->m_Buffer[tb->m_Chunks - (int)Scene::_A3CEE4] < 1024)
        tb->_8AA1F0(&Scene::_A3CEE4);

    field_8[paramind / 8] |= 1 << (paramind & 7);
}

void Node::_869F80(const unsigned int paramind, const void* paramptr, DataType& paramtype)
{
    int* v1 = g_SceneSaveLoad->_873BA0(m_Id.Id);
    if (v1)
    {
        *v1 = paramind & 0xFFF | (m_Id.Id << 8) | (m_Id.BlockId - 1) & 0xF000;
        v1++;
        g_SceneSaveLoad->_873C00(m_Id.Id, &v1[paramtype.stub9((char*)paramptr, (char*)v1)]);
    }

    field_8[paramind / 8 + 5] |= 1 << (paramind & 7);
}

#pragma message(TODO_IMPLEMENTATION)
void Node::TriggerGlobalScript(int scriptId, void* args)
{
    if (!m_ScriptEntity->m_Script || !m_ScriptEntity->m_Script->_48A7E0(this, scriptId, args))
    {
        EntityType::ScriptInfo* scriptinfo = m_ScriptEntity->m_IsBaseEntity ? &m_ScriptEntity->m_Parent->m_ScriptsList[scriptId] : &m_ScriptEntity->m_ScriptsList[scriptId];
        if (scriptinfo)
#ifdef INCLUDE_FIXES
            //  TODO: THIS IS VERY UGLY! I don't think original code had something like this!
            ((Entity*)(this + (int)scriptinfo->field_4)->*(scriptinfo->m_ScriptPtr))(args);
#else
            //  NOTE: this code is probably bugged so don't use it.
            if (scriptinfo->field_C)
                scriptinfo->m_ScriptPtr((void*)(this + (int)scriptinfo->field_4 + (int)scriptinfo->field_8), args);    //  NOTE: never used, since field_C is usually 0, same as field_4.
            else
                scriptinfo->m_ScriptPtr((void*)(this + (int)scriptinfo->field_4), args);
#endif
    }
}

#pragma message(TODO_IMPLEMENTATION)
void Node::_88C310(CameraMatrix* cammat)
{
}

void Node::InstantiateAllChildren()
{
    Instantiate();

    for (Node* n = m_FirstChild; n; n = n->m_NextSibling)
        n->InstantiateAllChildren();
}

void Node::DestroyChildren()
{
    for (Node* child = m_FirstChild; child; child = child->m_NextSibling)
    {
        child->DestroyFrameBuffers();
        child->DestroyChildren();
    }
}

void Node::ClearFromBlockingList()
{
    if (m_GlobalIdInBlockigList >= 0)
    {
        NodesWithUpdateOrBlockingScripts[m_GlobalIdInBlockigList].m_Node = nullptr;
        m_GlobalIdInBlockigList = -1;
    }
}

void Node::ClearFromSceneList()
{
    if (m_GlobalIdInSceneList >= 0)
    {
        NodesList[m_GlobalIdInSceneList].m_QuadTree = 0;
        m_GlobalIdInSceneList = -1;
    }
}

void Node::_86A190()
{
    field_8[5] = NULL;
    field_8[6] = NULL;
    field_8[7] = NULL;
    field_8[8] = NULL;
    field_8[9] = NULL;
}

void Node::SetFragment(const char* const fragmentpath)
{
    Node* child = m_FirstChild;
    if (child)
    {
        Node* sibling;
        do 
        {
            sibling = child->m_NextSibling;
            child->Destroy();
            child = sibling;
        } while (sibling);
    }

    delete m_Fragment;

    if (!fragmentpath)
        return;

    if (!m_Fragment)
        m_Fragment = new Fragment(this);

    String respath;
    String fullfragmpath;
    g_AssetManager->GetResourcePath(respath, fragmentpath);
    _891E70(respath, fullfragmpath);

    const size_t tstscrpathlen = strlen("/data/scripts/testing/fragments");
    if (strncmp(respath.m_Str, "/data/scripts/testing/fragments", tstscrpathlen))
    {
        fullfragmpath = "/data/scripts/fragments";
        fullfragmpath.Append(&respath.m_Str[tstscrpathlen]);
    }

    if (strcmp(fullfragmpath.m_Str, fragmentpath))
        LogDump::LogA("unmapped %s -> %s\n", fragmentpath, fullfragmpath.m_Str);

    if (m_Flags.m_FlagBits.HasFragment)
    {
        m_Fragment->LoadResourceFile(fullfragmpath.m_Str);
        m_Fragment->ApplyFragment();
    }
    else
        m_Fragment->SetFragmentName(fullfragmpath.m_Str);
}

void Node::TryInstantiate()
{
    if (m_GlobalIdInBlockigList < 0)
        m_Id.HasPosition = true;
    else
        Instantiate();
}

AuxQuadTree* Node::_8A0810(Node* node)
{
    if (node->m_QuadTree)
    {
        AuxQuadTree* qdtr = node->m_QuadTree;
        while (!qdtr->m_Owner->m_CollisionIgnoreList)
        {
            qdtr = qdtr->field_4;
            if (!qdtr)
                return node->m_QuadTree;
        }

        return qdtr;
    }
    else
    {
        Node* nd = node->m_Parent;
        if (nd)
        {
            while (!nd->m_QuadTree)
            {
                nd = nd->m_Parent;
                if (!nd)
                    return nullptr;
            }

            AuxQuadTree* qdtr = nd->m_QuadTree;
            if (qdtr)
            {
                while (!qdtr->m_Owner->m_CollisionIgnoreList)
                {
                    qdtr = qdtr->field_4;
                    if (!qdtr)
                        return nd->m_QuadTree;
                }

                return nd->m_QuadTree;
            }
        }
        else
            return nullptr;
    }

    return nullptr;
}

void Node::_891E70(const String& s, String& outs)
{
    if (File::DirectoryMappingsList.size() <= 0)
    {
        outs = s;
        return;
    }

    for (unsigned int i = 0; i < File::DirectoryMappingsList.size(); ++i)
    {
        if (strncmp(s.m_Str, File::DirectoryMappingsList[i].m_String_1.m_Str, File::DirectoryMappingsList[i].m_String_1.m_Length) == NULL)
        {
            outs = File::DirectoryMappingsList[i].m_String_2.m_Str;
            outs.Append(&s.m_Str[File::DirectoryMappingsList[i].m_String_1.m_Length]);
        }
    }

    outs = s;
}

#pragma message(TODO_IMPLEMENTATION)
void Node::Register()
{
    tNode = new EntityType("Node");
    tNode->InheritFrom(tEntity);
    tNode->SetCreator((CREATOR)Create);

    tNode->RegisterProperty(tSTRING, "typename", (EntityGetterFunction)&GetTypename, NULL, NULL, NULL, nullptr, NULL, NULL, -1, "control=static", NULL, NULL, -1);
    tNode->RegisterProperty(tSTRING, "script", (EntityGetterFunction)&GetScript, NULL, NULL, NULL, (EntitySetterFunction)&SetScript, NULL, NULL, NULL, "control=resource|type=*.script", NULL, NULL, -1);
    tNode->RegisterProperty(tINTEGER, "order", (EntityGetterFunction)&GetOrder, NULL, NULL, NULL, (EntitySetterFunction)&SetOrder, NULL, NULL, NULL, nullptr, NULL, NULL, -1);
    tNode->RegisterProperty(tINTEGER, "renderordergroup", (EntityGetterFunction)&GetRenderOrderGroup, NULL, NULL, NULL, (EntitySetterFunction)&SetRenderOrderGroup, NULL, NULL, NULL, "control=slider|min=-1|max=4", NULL, NULL, -1);
    tNode->RegisterProperty(tEntity, "parent", (EntityGetterFunction)&GetParent, NULL, NULL, NULL, (EntitySetterFunction)&SetParent, NULL, NULL, NULL, nullptr, NULL, NULL, 1);
    tNode->RegisterProperty(tTRUTH, "disable_on_cutscene", (EntityGetterFunction)&ShouldDisableOnCutscene, NULL, NULL, NULL, (EntitySetterFunction)&SetShouldDisableOnCutscene, NULL, NULL, NULL, "control=truth", NULL, NULL, -1);
    tNode->RegisterProperty(tSTRING, "fragment", (EntityGetterFunction)&GetFragment, NULL, NULL, NULL, (EntitySetterFunction)&SetFragment, NULL, NULL, NULL, "control=resource|type=*.fragment", NULL, NULL, -1);
    tNode->RegisterProperty(tINTEGER, "flags", (EntityGetterFunction)&GetFlags, NULL, NULL, NULL, (EntitySetterFunction)&SetFlags, NULL, NULL, NULL, "control=flags|flag0=disable|flag1=dynamic|flag2=invisible|flag3=lock|flag4=open|flag5=volatile|flag6=disable collision|flag7=d.b.r.|flag8=l.s.r.|flag9=purge names|flag10=e.r.r|flag11=f.r.r", NULL, NULL, 2);
    tNode->RegisterProperty(tINTEGER, "representation", (EntityGetterFunction)&GetRepresentation, NULL, NULL, NULL, nullptr, NULL, NULL, -1, nullptr, NULL, NULL, -1);
    tNode->RegisterProperty(tVECTOR, "pos", (EntityGetterFunction)&GetPos, NULL, NULL, NULL, (EntitySetterFunction)&SetPos, NULL, NULL, NULL, "control=string", NULL, NULL, 3);
    tNode->RegisterProperty(tQUATERNION, "orient", (EntityGetterFunction)&GetOrient, NULL, NULL, NULL, (EntitySetterFunction)&SetOrient, NULL, NULL, NULL, "control=string", NULL, NULL, 4);
    tNode->RegisterProperty(tTRUTH, "use_aux_quadtree", (EntityGetterFunction)&ShouldUseAuxQuadTree, NULL, NULL, NULL, (EntitySetterFunction)&SetShouldUseAuxQuadTree, NULL, NULL, NULL, "control=truth", NULL, NULL, 5);
    tNode->RegisterProperty(tSTRING, "name", (EntityGetterFunction)&GetName, NULL, NULL, NULL, (EntitySetterFunction)&SetName, NULL, NULL, NULL, "control=string", NULL, NULL, -1);
    tNode->RegisterProperty(tNUMBER, "lod_threshold", (EntityGetterFunction)&GetLodThreshold, NULL, NULL, NULL, (EntitySetterFunction)&SetLodThreshold, NULL, NULL, NULL, "control=slider|min=0|max=0.4|step=0.01", NULL, NULL, 8);
    tNode->RegisterProperty(tNUMBER, "fade_threshold", (EntityGetterFunction)&GetFadeThreshold, NULL, NULL, NULL, (EntitySetterFunction)&SetFadeThreshold, NULL, NULL, NULL, "control=slider|min=0|max=0.2|step=0.01", NULL, NULL, 9);
    tNode->RegisterProperty(tTRUTH, "slow_fade", (EntityGetterFunction)&ShouldSlowFade, NULL, NULL, NULL, (EntitySetterFunction)&SetShouldSlowFade, NULL, NULL, NULL, "control=truth", NULL, NULL, -1);
    tNode->RegisterProperty(tNUMBER, "traverse_distance", (EntityGetterFunction)&GetTraverseDistance, NULL, NULL, NULL, (EntitySetterFunction)&SetTraverseDistance, NULL, NULL, NULL, nullptr, NULL, NULL, -1);
    tNode->RegisterProperty(tEntity, "scene", (EntityGetterFunction)&GetScene, NULL, NULL, NULL, nullptr, NULL, NULL, -1, nullptr, NULL, NULL, -1);
    tNode->RegisterProperty(tEntity, "firstchild", (EntityGetterFunction)&GetFirstChild, NULL, NULL, NULL, nullptr, NULL, NULL, -1, nullptr, NULL, NULL, -1);
    tNode->RegisterProperty(tEntity, "nextsibling", (EntityGetterFunction)&GetNextSibling, NULL, NULL, NULL, nullptr, NULL, NULL, -1, nullptr, NULL, NULL, -1);
    tNode->RegisterProperty(tNUMBER, "lod_distance", (EntityGetterFunction)&GetLodDistance, NULL, NULL, NULL, nullptr, NULL, NULL, -1, nullptr, NULL, NULL, -1);
    tNode->RegisterProperty(tNUMBER, "lod_fade", (EntityGetterFunction)&GetLodFade, NULL, NULL, NULL, nullptr, NULL, NULL, -1, nullptr, NULL, NULL, -1);
    tNode->RegisterProperty(tINTEGER, "lod", (EntityGetterFunction)&GetLod, NULL, NULL, NULL, nullptr, NULL, NULL, -1, nullptr, NULL, NULL, -1);
    tNode->RegisterProperty(tTRUTH, "is_tagged_for_unload", (EntityGetterFunction)&GetIsTaggedForUnload, NULL, NULL, NULL, nullptr, NULL, NULL, -1, nullptr, NULL, NULL, -1);
    tNode->RegisterProperty(tINTEGER, "unique_id0", (EntityGetterFunction)&GetUniqueId0, NULL, NULL, NULL, (EntitySetterFunction)&SetUniqueId0, NULL, NULL, NULL, nullptr, NULL, NULL, -1);
    tNode->RegisterProperty(tINTEGER, "unique_id1", (EntityGetterFunction)&GetUniqueId1, NULL, NULL, NULL, (EntitySetterFunction)&SetUniqueId1, NULL, NULL, NULL, nullptr, NULL, NULL, -1);
    tNode->RegisterProperty(tINTEGER, "UserType", (EntityGetterFunction)&GetUserType, NULL, NULL, NULL, (EntitySetterFunction)&SetUserType, NULL, NULL, NULL, "control=string", NULL, NULL, -1);
    tNode->RegisterProperty(tSTRING, "IgnoreList", (EntityGetterFunction)&GetIgnoreList, NULL, NULL, NULL, (EntitySetterFunction)&SetIgnoreList, NULL, NULL, NULL, "control=string", NULL, NULL, 7);

    tNode->RegisterScript("IsDisabled:truth", (EntityFunctionMember)&IsDisabled, NULL, NULL, NULL, nullptr, "IsDisabledMSG");
    tNode->RegisterScript("IsSuspended:truth", (EntityFunctionMember)&IsSuspended, NULL, NULL, NULL, nullptr, "IsSuspendedMSG");
    tNode->RegisterScript("move(vector)", (EntityFunctionMember)&Move, NULL, NULL, NULL, nullptr, "MoveMSG");
    tNode->RegisterScript("movelocal(vector)", (EntityFunctionMember)&MoveLocal, NULL, NULL, NULL, nullptr, "MoveLocalMSG");

    tNode->PropagateProperties();
}

Node* Node::Create(AllocatorIndex)
{
    return new Node(NODE_MASK_POSITION);
}

Vector4f* NodePosition::GetPosition(Vector4f* outPos)
{
    ((Node*)(this - 9))->GetPos(*outPos);
    return outPos;
}

void NodePosition::GetOrientation(Orientation* outOrient)
{
    if (this + 6)
        *outOrient = *(Orientation*)(this + 6);
    else
        *outOrient = Orientation();
}

void NodePosition::CopyPositionToMatrix(DirectX::XMMATRIX* outMat)
{
    DirectX::XMMATRIX tempMat;

    ((Node*)(this - 9))->GetWorldMatrix(tempMat);

    outMat->r[0] = tempMat.r[3];
}

#pragma message(TODO_IMPLEMENTATION)
Orientation* NodePosition::GetWorldRotation(Orientation* rot)
{
    DirectX::XMMATRIX mat;

    ((Node*)(this - 9))->GetWorldMatrix(mat);   //  NOTE: this f'd up pointer math is here to get the pointer to the owner Node.
    //  TODO: convert rotation vector from world matrix to quaternion.

    return nullptr;
}