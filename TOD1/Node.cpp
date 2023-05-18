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
#include "Model.h"
#include "SavePoint.h"

EntityType* tNode;
std::vector<Node::NodeInfo> Node::NodesWithUpdateOrBlockingScripts;
std::vector<Node::PackedBoundsInfo> Node::NodesList;
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
    if (!m_Owner->m_Id.HasQuadTree)
        return;
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
    m_RightVector = *(Vector4f*)&mat.r[0];
    m_UpVector = *(Vector4f*)&mat.r[1];
    m_ForwardVector = *(Vector4f*)&mat.r[2];
    m_PositionVector = *(Vector4f*)&mat.r[3];
}

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
    delete m_Collision;
    delete[] m_Name;

    if (m_GlobalIdInBlockigList >= 0)
    {
        NodesWithUpdateOrBlockingScripts[m_GlobalIdInBlockigList].m_Flags.m_Id = NULL;
        m_GlobalIdInBlockigList = -1;
    }

    if (m_GlobalIdInSceneList >= 0)
    {
        NodesList[m_GlobalIdInSceneList].QDTree = nullptr;
        m_GlobalIdInSceneList = -1;
    }

    this->~Node();
}

void Node::_484CC0(int)
{
    m_Id.HasQuadTree = true;
    m_Id._3 = 1;
}

#pragma message(TODO_IMPLEMENTATION)
Node* Node::FindNode(const char* nodeName)
{
    if (!nodeName || !nodeName[0])
        return nullptr;

    if (g_AssetManager->m_FastFindNodeVector.size())
    {

    }
    else
    {
        if (m_Name)
        {
            if (String::EqualIgnoreCase(m_Name, nodeName, strlen(m_Name)))
                return this;
        }
        else
        {
            Model* modelEntity = (Model*)EntityType::IsParentOf(tModel, (Entity*)this);
            if (modelEntity && modelEntity->m_ModelRes.m_AssetPtr)
            {
                String fileName;
                FileBuffer::ExtractFileName(fileName, modelEntity->m_ModelRes.m_AssetPtr->GetName());
                if (String::EqualIgnoreCase(fileName.m_Str, nodeName, fileName.m_Length))
                    return this;
            }
        }

        return FindNodeSlowRecursive(nodeName);
    }

    return nullptr;
}

#pragma message(TODO_IMPLEMENTATION)
Node* Node::FindNodeSlowRecursive(const char* const nodeName)
{
    if (!m_FirstChild)
        return nullptr;

    while (true)
    {
        if (m_FirstChild->m_Name)
            if (!_stricmp(m_FirstChild->m_Name, nodeName))
                return m_FirstChild;

        String nodeNameStr(nodeName);
        nodeNameStr.ToLowerCase();
        const unsigned int nodeNameCrc = (unsigned int)Utils::CalcCRC32(nodeNameStr.m_Str, nodeNameStr.m_Length);

        AssetManager::FastFindInfo ffiTemp { 0, nodeNameCrc, nullptr };
        std::vector<AssetManager::FastFindInfo>::iterator ffindNode = std::find(g_AssetManager->m_FastFindNodeVector.begin(), g_AssetManager->m_FastFindNodeVector.end(), ffiTemp);

        Node* result = nullptr;
        if (ffindNode != g_AssetManager->m_FastFindNodeVector.end())
        {
            do
            {
                if (ffindNode->m_NodeNameCRC != nodeNameCrc)
                    break;

                if (ffindNode->m_Node)
                {
                    //  ...
                }

                ffindNode++;
            }while(ffindNode != g_AssetManager->m_FastFindNodeVector.end());
        }

        return result;
    }
}

void Node::StorePropertyData(const uint32_t propertyIndex, const uint32_t* const propertyValue, const DataType* propertyType)
{
    if (!m_ScriptEntity)
        return;

    if (m_Flags.Volatile || m_Flags._29)
        return;

    int32_t* dataBuffer = g_SceneSaveLoad->GetEntityDataBuffer(m_Id.Id);
    if (!dataBuffer)
        return;

    const bool propertyInUse = ((1 << (propertyIndex & 7)) & m_PropertiesSlots[propertyIndex / 8]) != 0;
    if (!propertyInUse)
        return;

    *dataBuffer = propertyIndex & 0xFFF | (m_Id.Id << 8) | ((*(uint32_t*)&m_Id >> 16) & 0x7000);
    int32_t* nextDataPtr = dataBuffer + 1;

    const uint32_t bytesCopied = propertyType->CopyNoAllocate((const char*)propertyValue, (char*)nextDataPtr);
    g_SceneSaveLoad->SaveEntityToDataBuffer(m_Id.Id, nextDataPtr + bytesCopied);
}

void Node::GetScriptEntityPropertyValue(const int propertyId, uint8_t* outPropertyValue) const
{
    m_ScriptEntity->GetPropertyValue(this, m_Parameters, propertyId, outPropertyValue);
}

#pragma message(TODO_IMPLEMENTATION)
void Node::TriggerScript(const uint16_t scriptId, const uint16_t scriptIdA, const uint8_t* scriptParams) const
{
    const bool isBaseEntity = m_ScriptEntity->IsBaseEntity;
    const EntityType* entity = isBaseEntity ? m_ScriptEntity->Parent : m_ScriptEntity;
    const size_t scriptsListSize = entity->ScriptsList.size();

    if (scriptId >= scriptsListSize)
        return;

    /*if (scriptIdA != entity->ScriptsList.at(scriptId))
        return;*/

    const EntityType::ScriptInfo* scriptInfo = entity->ScriptsList.at(scriptId);
    if (scriptInfo->VirtualFunctionOffset);
        //  TODO: 'this' adjustment shenanigans here!
}

void Node::SaveScriptData() const
{
    if (!m_ScriptEntity)
        return;

    if (m_Flags._29 || m_Flags.Volatile)
        return;

    auto dataBuffer = g_SceneSaveLoad->GetEntityDataBuffer(m_Id.Id);
    if (!dataBuffer)
        return;

    if ((m_PropertiesSlots[0] & 1) == 0)
        return;

    *dataBuffer = (m_Id.Id << 8) | (((*(uint32_t*)&m_Id >> 16) & 0x7000) - 1) & 0xF000;
    dataBuffer++;

    if (m_ScriptData && m_ScriptData->m_ScriptThread)
    {
        *dataBuffer = 1;

        const auto scriptDataSize = m_ScriptData->m_ScriptThread->WriteScriptInformation(++dataBuffer);
        dataBuffer += scriptDataSize;
    }
    else
    {
        *dataBuffer = 0;
        dataBuffer++;
    }

    g_SceneSaveLoad->SaveEntityToDataBuffer(m_Id.Id, dataBuffer);
}

void Node::SaveScriptPropertyData(const uint32_t propertyIndex, const uint32_t* param) const
{
    if (!m_ScriptEntity)
        return;

    if (m_Flags._29 || m_Flags.Volatile)
        return;

    auto dataBuffer = g_SceneSaveLoad->GetEntityDataBuffer(m_Id.Id);
    if (!dataBuffer)
        return;

    const bool isUsed = (m_Parameters[propertyIndex / 16] & (2 * (1 << (2 * propertyIndex & 15)))) != 0;
    if (!isUsed)
        return;

    const auto propertyType = m_ScriptEntity->Script->m_PropertiesList[propertyIndex].m_Info->m_PropertyType;
    *dataBuffer = (m_Id.Id << 8) | (((m_Id.BlockId - 1) << 12) & 0xF000) | ((int16_t)propertyIndex + (uint16_t)m_ScriptEntity->GetTotalProperties()) & 0xFFF;
    dataBuffer++;

    const auto propertyDataSize = propertyType->CopyNoAllocate((char*)param, (char*)dataBuffer);
    dataBuffer += propertyDataSize;

    g_SceneSaveLoad->SaveEntityToDataBuffer(m_Id.Id, dataBuffer);
}

void Node::FindNode_Impl(int* args)
{
    *args = (int)FindNode((const char*)args[1]);
}

void Node::ConvertFromWorldSpace(Vector4f& outPos, const Vector4f& inPos)
{
    if (!m_Position)
        return;

    if (m_Id.HasQuadTree)
        m_Position->ApplyMatrixFromQuadTree();

    DirectX::XMMATRIX mat, matTransposed;
    m_Position->GetMatrix(mat);

    matTransposed = DirectX::XMMatrixTranspose(mat);

    outPos =
    {
        (inPos.x * matTransposed.r[0].m128_f32[0]) + (inPos.y * matTransposed.r[1].m128_f32[0]) + (inPos.z * matTransposed.r[2].m128_f32[0]) + matTransposed.r[3].m128_f32[0],
        (inPos.y * matTransposed.r[1].m128_f32[1]) + (inPos.x * matTransposed.r[0].m128_f32[1]) + (inPos.z * matTransposed.r[2].m128_f32[1]) + matTransposed.r[3].m128_f32[1],
        (inPos.y * matTransposed.r[1].m128_f32[2]) + (inPos.x * matTransposed.r[0].m128_f32[2]) + (inPos.z * matTransposed.r[2].m128_f32[2]) + matTransposed.r[3].m128_f32[2],
        (inPos.a)
    };
}

void Node::SetWorldOrient(const Orientation& orientation)
{
    DirectX::XMMATRIX mat;
    m_Parent->GetWorldMatrix(mat);

    DirectX::XMVECTOR rotVector = DirectX::XMQuaternionRotationMatrix(mat);
    rotVector.m128_f32[0] = 0.f - rotVector.m128_f32[0];
    rotVector.m128_f32[1] = 0.f - rotVector.m128_f32[1];
    rotVector.m128_f32[2] = 0.f - rotVector.m128_f32[2];

    const DirectX::XMVECTOR finalRotationVector =
    {
        (orientation.z * rotVector.m128_f32[0]) + (rotVector.m128_f32[3] * orientation.w) + (orientation.x * rotVector.m128_f32[2]) - (orientation.y * rotVector.m128_f32[1]),
        (orientation.z * rotVector.m128_f32[1]) + (rotVector.m128_f32[3] * orientation.x) + (orientation.y * rotVector.m128_f32[2]) - (orientation.w * rotVector.m128_f32[2]),
        (orientation.z * rotVector.m128_f32[2]) + (rotVector.m128_f32[3] * orientation.y) + (orientation.w * rotVector.m128_f32[1]) - (orientation.x * rotVector.m128_f32[0]),
        (orientation.z * rotVector.m128_f32[3]) - (rotVector.m128_f32[2] * orientation.y) + (orientation.x * rotVector.m128_f32[1]) - (orientation.w * rotVector.m128_f32[0])
    };

    SetOrient(finalRotationVector);
}

void Node::_86B4B0(const uint32_t propertyId)
{
    if (m_Flags.HasFragment || m_Flags.Volatile)
        return;

    const unsigned int block = 2 * (propertyId & 15);
    uint32_t* propertyParamValue = &m_Parameters[propertyId / 16];
    const unsigned int slot = *propertyParamValue >> block;
    const uint32_t* scriptPropertyValue = &m_Parameters[m_ScriptEntity->Script->m_PropertiesList[propertyId].m_Offset];

    //  NOTE: check if this property should be saved/added to rewind?
    if ((slot & 3) != 3)
    {
        if ((slot & 1) == 0 && Scene::RewindBuffer2DataPtr)
            _86A930(propertyId, scriptPropertyValue, propertyParamValue, 1 << block);
        if ((slot & 2) == 0)
            _86AA10(propertyId, scriptPropertyValue, propertyParamValue, 2 * (1 << block));
    }
}

void Node::_86A930(const int size, const uint32_t* value, uint32_t* const outval, const int a4)
{
    EntityType* ent = m_ScriptEntity->IsBaseEntity ? m_ScriptEntity->Parent : m_ScriptEntity;
    *Scene::RewindBuffer2DataPtr++ =
        ((m_Id.Id << 8) | ((*(short*)&m_Id & 0x7000) - 1) & 0xF000) |
        ((short)size + (short)ent->LocalPropertiesList.size() + (short)ent->TotalLocalProperties) & 0xFFF;
    Scene::RewindBuffer2DataPtr += ent->Script->m_PropertiesList[size].m_Info->m_PropertyType->CopyNoAllocate((char*)value, (char*)Scene::RewindBuffer2DataPtr);
    if (((int)Scene::SceneInstance->m_RewindBuffer2->m_Buffer + 4 * Scene::SceneInstance->m_RewindBuffer2->m_Chunks - (int)Scene::RewindBuffer2DataPtr) < 0x4000)
        Scene::SceneInstance->m_RewindBuffer2->_8AA1F0(&Scene::RewindBuffer2DataPtr);

    *outval |= a4;
}

void Node::_86AA10(const int propertyId, const uint32_t* value, uint32_t* const outval, const int a4)
{
    int* buffptr = g_SceneSaveLoad->GetEntityDataBuffer(m_Id.Id);
    if (buffptr)
    {
        DataType* propertyType = m_ScriptEntity->Script->m_PropertiesList[propertyId].m_Info->m_PropertyType;
        EntityType* entityType = m_ScriptEntity->IsBaseEntity ? m_ScriptEntity->Parent : m_ScriptEntity;

        buffptr++[0] = (m_Id.Id << 8) | (m_Id.BlockId - 1) & 0xF000 | ((short)propertyId + (short)entityType->LocalPropertiesList.size() + (short)entityType->TotalLocalProperties) & 0xFFF;
        const int propertynewsize = propertyType->CopyNoAllocate((const char*)value, (char*)(++buffptr));
        g_SceneSaveLoad->SaveEntityToDataBuffer(m_Id.Id, &buffptr[4 * propertynewsize]);
    }

    *outval |= a4;
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
    *suspended = m_ScriptData &&
        m_ScriptData->m_ScriptThread &&
        m_ScriptData->m_ScriptThread->m_ThreadFlags.Suspended != false;
}

void Node::IsDisabled(bool* disabled) const
{
    *disabled = m_Flags._16 | m_Flags.Disable;
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
    return m_Flags.RenderOrderGroup;
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

    m_Flags.RenderOrderGroup = renderordergroup;
    m_Id._3 = true;
}

const bool Node::ShouldDisableOnCutscene() const
{
    return m_Flags.DisabledOnCutscene;
}

void Node::SetShouldDisableOnCutscene(const bool disable)
{
    m_Flags.DisabledOnCutscene = disable;
}

const int Node::GetRepresentation() const
{
    int representation = m_Position != nullptr;

    if (m_QuadTree)
        representation |= 2;

    if (m_Flags.HasFragment)
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
    StoreProperty(5, &flag, tTRUTH);
    m_QuadTree->SetIsUsed(use);
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
    return m_QuadTree ? (m_QuadTree->m_LodThreshold * 0.005f) : 0.f;
}

void Node::SetLodThreshold(float threshold)
{
    if (!m_QuadTree)
        return;

    const float thresholdcurrent = (float)m_QuadTree->m_LodThreshold * 0.005f;
    StoreProperty(8, &thresholdcurrent, tNUMBER);

    threshold = clamp<float>(threshold, 0.f, 1.f);

    m_QuadTree->m_LodThreshold = (char)(threshold * 200.0f);
    UpdateQuadTreeLodThreshold();
}

void Node::UpdateQuadTreeLodThreshold()
{
    AuxQuadTree* qdt = m_QuadTree;
    if (qdt->m_LodThreshold == 24 && (qdt->m_FadeThreshold == 3) && !m_Flags.LSR)
    {
        qdt->m_LodThreshold = 60;
        SetFlags(m_Flags.m_Flags & 0xEFF | 0x100);
    }
}

const float Node::GetFadeThreshold() const
{
    return m_QuadTree ? ((float)m_QuadTree->m_FadeThreshold * 0.009f) : 0.f;
}

void Node::SetFadeThreshold(float threshold)
{
    if (!m_QuadTree)
        return;

    const float fadethreshold = (float)m_QuadTree->m_FadeThreshold * 0.009f;
    StoreProperty(9, &fadethreshold, tNUMBER);

    threshold = clamp<float>(threshold, 0.f, 1.f);

    m_QuadTree->m_FadeThreshold = (char)(threshold * 100.0f);
    UpdateModelFadeThreshold();
}

void Node::UpdateModelFadeThreshold()
{
    if (!m_ScriptEntity)
        return;

    EntityType* script = m_ScriptEntity;
    while (tModel != script)
    {
        script = script->Parent;
        if (!script)
            return;
    }

    if (m_Flags.FRR)
        return;

    if (!m_QuadTree || ((float)m_QuadTree->m_FadeThreshold * 0.009f) < 0.12f)
    {
        m_QuadTree->m_SlowFade = true;
        m_QuadTree->m_FadeThreshold = 11;
        SetFlags(m_Flags.m_Flags & 0x7FF | 0x800);  //  NOTE: set 'FRR' flag to true.
    }
}

const bool Node::ShouldSlowFade() const
{
    return m_QuadTree ? m_QuadTree->m_SlowFade : false;
}

void Node::SetShouldSlowFade(const bool slowfade)
{
    if (m_QuadTree)
        m_QuadTree->m_SlowFade = slowfade;
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
        m_QuadTree->m_TraverseDistance = (short)distance;
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
    return m_QuadTree ? m_QuadTree->m_LodFade * (1.f / 255.f) : 0.f;
}

const bool Node::GetIsTaggedForUnload() const
{
    Folder_* parentfolder = FindParentFolder();
    if (!parentfolder || parentfolder == this)
        return false;
    else
        return parentfolder->m_TaggedForUnload;
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
            folderscript = folderscript->Parent;
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
        StoreProperty(6, &m_QuadTree->m_UserType, tINTEGER);
        m_QuadTree->m_UserType ^= (type ^ m_QuadTree->m_UserType) & 0xFFFFFF;
    }
}

const char* const Node::GetIgnoreList() const
{
    if (!m_Collision)
        return nullptr;

    if (!m_Collision->m_CollisionProbesList.size())
        return IgnoredCollisionNodes.m_Str;

    char buffer[12] = {};
    for (unsigned int i = 0; i < m_Collision->m_CollisionProbesList.size(); ++i)
    {
        if (!m_Collision->m_CollisionProbesList[i])
            continue;

        sprintf(buffer, "%010d;", m_Collision->m_CollisionProbesList[i]->m_Id.Id);
        IgnoredCollisionNodes.Append(buffer);

        memset(buffer, NULL, 12);
    }

    return IgnoredCollisionNodes.m_Str;
}

//  Helper utility function to convert number string into actual number compatible with entity ID.
static inline uint32_t NumberStringToNumber(const char* str)
{
    return (
        1'000'000'000 * (*str - 8) +
        100'000'000 * (*str - 7) +
        10'000'000 * ((*str - 6) - 48) +
        1'000'000 * ((*str - 5) - 48) +
        100'000 * ((*str - 4) - 48) +
        10'000 * ((*str - 3) - 48) +
        1'000 * ((*str - 2) - 48) +
        100 * ((*str - 1) - 48) +
        10 * (*str - 48) +
        str[1] - 48 -
        0x1E1A3000
    );
}

void Node::SetIgnoreList(const char* list)
{
    if (!m_Collision || !list)
        return;

    m_Collision->m_CollisionProbesList.clear();
    const size_t listlen = strlen(list);
    const size_t blocksize = 11;

    if (listlen < blocksize)
        return;

    auto listMutable = const_cast<char*>(list);
    listMutable += 8;
    for (uint32_t i = listlen / blocksize; i; --i)
    {
        const uint32_t entityId = NumberStringToNumber(listMutable);
        if (entityId != 0x2D05E000)
            m_Collision->AddNode(g_AssetManager->FindEntityById(entityId - 0x2D05E000));
        listMutable += 11;
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

    EntityType* folderent = m_ScriptEntity;
    while (tFolder != folderent)
    {
        folderent = folderent->Parent;
        if (!folderent)
            return;
    }

    const int32_t folderBlockId = reinterpret_cast<const Folder_*>(this)->GetBlockId();
    if (folderBlockId)
        *outBlockId = folderBlockId;
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
        StoreProperty(4, &m_Position, tQUATERNION);
        if (m_Collision)
            m_Collision->m_Orientation = orient;

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

    StoreProperty(3, &m_Position->m_Position, tVECTOR);
    if (m_Collision)
    {
        const float dist =
            ((pos.z - m_Collision->m_Position.z) * (pos.z - m_Collision->m_Position.z)) +
            ((pos.y - m_Collision->m_Position.y) * (pos.y - m_Collision->m_Position.y)) +
            ((pos.x - m_Collision->m_Position.x) * (pos.x - m_Collision->m_Position.x));
        if (dist > 100)
            m_Collision->m_Position = { pos.x, pos.y, pos.z };
    }

    m_Position->m_Position = pos;
}

void Node::PurgeNames(const int dummy)
{
    PurgeNames_Impl(false);
}

void Node::PurgeNames_Impl(bool onlyChildren)
{
    if (m_Flags.PurgeNames)
        onlyChildren = true;

    if (onlyChildren)
        delete[] m_Name;

    for (Node* child = m_FirstChild; child; child = child->m_NextSibling)
        PurgeNames_Impl(onlyChildren);
}

void Node::ResetIgnoreList()
{
    if (!m_Collision)
        return;

    const char* const ignorelist = GetIgnoreList();
    StoreProperty(7, &ignorelist, tSTRING);

    m_Collision->m_CollisionProbesList.clear();
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
    * args = PlatformNumber[0];
#endif
#ifdef PLATFORM_PS2
    * args = PlatformNumber[1];
#endif
#ifdef PLATFORM_XBOX
    * args = PlatformNumber[2];
#endif
#else
    * args = NULL;
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
    if (m_Collision)
        m_Collision->CommitCollision();
}

void Node::SetSafePos(const Vector4f& pos, const Orientation& orient)
{
    if (!m_Collision)
        return;

    m_Collision->m_SafePosition = { pos.x, pos.y, pos.z };
    m_Collision->m_SafeOrientation = orient;
}

void Node::Rotate(float* args)
{
    Orientation rot = *(Orientation*)args;
    Rotate_Impl(rot);
}

void Node::RotateX(const float* args)
{
    RotateX_Impl(args[0]);
}

void Node::RotateY(const float* args)
{
    RotateY_Impl(args[0]);
}

void Node::RotateZ(const float* args)
{
    RotateZ_Impl(args[0]);
}

void Node::RotateLocal(const float* args)
{
    Orientation orient = *(Orientation*)args;
    SetOrientation(orient);
}

void Node::TouchPivot(const int dummy)
{
    _484CC0(0);

    if (m_QuadTree)
        m_QuadTree->_8A3320();

    for (Node* child = m_FirstChild; child; child = child->m_NextSibling)
    {
        child->_484CC0(0);
        child->_88D230(0);
    }
}

void Node::FastSetPos(int* args)
{
    FastSetPos_Impl({ (float)args[0], (float)args[1], (float)args[2], 0.f });
}

void Node::RemoveIgnoreNode(int* args)
{
    if (!m_Collision)
        return;

    const char* ignoreList = GetIgnoreList();
    StoreProperty(7, &ignoreList, tSTRING);
    m_Collision->RemoveNode((Node*)args[0]);
}

void Node::IgnoreNode(int* args)
{
    if (!m_Collision)
        return;

    const char* ignoreList = GetIgnoreList();
    StoreProperty(7, &ignoreList, tSTRING);
    m_Collision->AddNode((Node*)args[0]);
}

void Node::AnnotateSphere(int* args) const
{
    const Vector4f spherePos((float)args[0], (float)args[1], (float)args[2], 0.f);
    const int unk1 = args[3],
        unk2 = args[4],
        unk3 = args[5];

    Scene::SceneInstance->AnnotateSphere_Impl(spherePos, unk1, unk2, unk3);
}

void Node::AnnotateLine(int* args) const
{
    const Vector4f lineStart((float)args[0], (float)args[1], (float)args[2], 0.f);
    const Vector4f lineEnd((float)args[3], (float)args[4], (float)args[5], 0.f);
    const int unk1 = args[6], unk2 = args[7];

    Scene::SceneInstance->AnnotateLine_Impl(lineStart, lineEnd, unk1, unk2);
}

void Node::AnnotatePoint(int* args) const
{
    const Vector4f point((float)args[0], (float)args[1], (float)args[2], 0.f);
    const int unk1 = args[3], unk2 = args[4];

    Scene::SceneInstance->AnnotatePoint_Impl(point, unk1, unk2);
}

void Node::DestroyNode(int* args) const
{
    ((Entity*)args[0])->Destroy();
}

void Node::ResolveObjectUsingProbe(int* args) const
{
    ResolveObjectUsingProbe_Impl(this, (int*)args[0], (CollisionProbe*)args[1]);
}

void Node::ResolveObject(int* args) const
{
    ResolveObject_Impl(this, args[0]);
}

#pragma message(TODO_IMPLEMENTATION)
void Node::GetContactRegion(int* args) const
{
}

void Node::GetContactMaterialID(int* args) const
{
    if (m_QuadTree)
        args[0] = m_QuadTree->GetContactMaterialID(args[1]);
    else
        args[0] = 0;
}

void Node::GetContactSurfacePropFields(int* args) const
{
    if (m_QuadTree)
        args[0] = m_QuadTree->GetContactSurfacePropFields(args[1]);
    else
        args[0] = ModelAsset::DefaultSurfacePropFields;
}

void Node::GetRealNode(int* args) const
{
    if (!m_QuadTree)
        args[0] = NULL;
    else
        args[0] = (int)m_QuadTree->GetRealNode(args[1]);
}

void Node::GetContactNode(int* args) const
{
    if (!m_QuadTree)
        args[0] = NULL;
    else
        args[0] = (int)m_QuadTree->GetContactNode(args[1]);
}

void Node::GetSelfRealNode(int* args) const
{
    if (!m_QuadTree)
        args[0] = NULL;
    else
        args[0] = (int)m_QuadTree->GetRealNode(args[1]);
}

void Node::GetPeerContactPoint(int* args) const
{
    Vector4f contactpoint = BuiltinType::ZeroVector;
    if (m_QuadTree)
        contactpoint = *m_QuadTree->GetPeerContactPoint(args[3]);

    *(Vector3f*)args = { contactpoint.x, contactpoint.y, contactpoint.z };
}

void Node::GetContactPoint(int* args) const
{
    Vector4f contactpoint = BuiltinType::ZeroVector;
    if (m_QuadTree)
        contactpoint = *m_QuadTree->GetContactPoint(args[3]);

    *(Vector3f*)args = { contactpoint.x, contactpoint.y, contactpoint.z };
}

void Node::GetContactNormal(int* args) const
{
    Vector4f contactnormal = BuiltinType::ZeroVector;
    Vector4f dummy;
    if (m_QuadTree)
        contactnormal = *m_QuadTree->GetContactNormal(dummy, args[3]);

    *(Vector3f*)args = { contactnormal.x, contactnormal.y, contactnormal.z };
}

void Node::Contacts(int* args) const
{
    args[0] = m_QuadTree ? m_QuadTree->Contacts() : NULL;
}

void Node::SetContactFilter(int* args) const
{
    if (m_QuadTree)
        m_QuadTree->m_ContactFilter = args[0];
}

void Node::SetWorldPos(int* args)
{
    Vector4f pos;
    m_Parent->ConvertFromWorldSpace(pos, { (float)args[0], (float)args[1], (float)args[2], 0.f });
    SetPos(pos);
}

void Node::GetWorldOrient(int* args) const
{
    DirectX::XMMATRIX mat;
    GetWorldMatrix(mat);
    DirectX::XMVECTOR quat = DirectX::XMQuaternionRotationMatrix(mat);

    *(DirectX::XMVECTOR*)args = quat;
}

void Node::ConvertDirectionFromWorldSpace(int* args) const
{
    Vector4f offset((float)args[3], (float)args[4], (float)args[5], 0.f);
    Vector4f dir;

    GetWorldSpaceDirection(dir, offset);

    *(Vector3f*)args = { dir.x, dir.y, dir.z };
}

void Node::ConvertDirectionToWorldSpace(int* args) const
{
    Vector4f offset((float)args[3], (float)args[4], (float)args[5], 0.f);
    Vector4f dir;

    GetLocalSpaceDirection(dir, offset);

    *(Vector3f*)args = { dir.x, dir.y, dir.z };
}

void Node::CallPropertySetter(const unsigned short propertyId, const void* data)
{
    std::map<unsigned short, unsigned short>::const_iterator propertyIndex = m_ScriptEntity->IsBaseEntity
        ? m_ScriptEntity->Parent->PropertiesMappings.find(propertyId)
        : m_ScriptEntity->PropertiesMappings.find(propertyId);

    if (propertyIndex->first)
    {
        EntityType* script = m_ScriptEntity;
        unsigned short propargind = propertyIndex->first;
        EntityType::PropertyInfo* scriptpropertyinfo = nullptr;

        if (propargind >= 0)
        {
            if (propargind < m_ScriptEntity->TotalLocalProperties)
            {
                do
                {
                    script = script->Parent;
                } while (propargind < script->TotalLocalProperties);
            }

            scriptpropertyinfo = &script->LocalPropertiesList[propargind - script->TotalLocalProperties];
        }
        else
        {
            propargind = -propargind;
            if (propargind < m_ScriptEntity->TotalGlobalProperties)
            {
                do
                {
                    script = script->Parent;
                } while (propargind < script->TotalGlobalProperties);
            }

            scriptpropertyinfo = &script->GlobalPropertiesList[propargind - script->TotalGlobalProperties];
        }

        if (scriptpropertyinfo && scriptpropertyinfo->Setter)
            scriptpropertyinfo->ReturnType->CallSetterFunction(
                data,
                this,
                scriptpropertyinfo->Setter,
                scriptpropertyinfo->_f1C,
                scriptpropertyinfo->_f20,
                scriptpropertyinfo->_f24);
    }
    else
    {
        if (m_ScriptEntity->Script)
            m_ScriptEntity->Script->AddPropertyByReference(this, propertyId, data);
    }
}

void Node::_86B560(const unsigned int propertyId, const void* data)
{
    if (!m_ScriptEntity || m_Flags._29 || m_Flags.Volatile)
        return;

    uint32_t* param = &m_Parameters[propertyId / 16];
    const unsigned int index = 1 << (2 * (propertyId & 15));

    if ((param[0] & index) == 0 || (param[0] & (2 * index)) == 0)
    {
        if (Scene::RewindBuffer2DataPtr && (param[0] & index) == 0)
            _86A930(propertyId, (uint32_t*)data, param, index);

        if ((param[0] & (2 * index)) == 0)
            _86AA10(propertyId, (uint32_t*)data, param, 2 * index);
    }
}

void Node::Rotate_Impl(const Orientation& orient)
{
    Orientation currentOrient = BuiltinType::Orient;
    Orientation newOrientation;

    if (m_Position)
        currentOrient = m_Position->m_Orientation;

    newOrientation.w = (currentOrient.w * orient.z) + (currentOrient.z * orient.w) + ((currentOrient.y * orient.x) - (currentOrient.x * orient.y));
    newOrientation.y = (currentOrient.y * orient.z) + (currentOrient.z * orient.y) + ((currentOrient.x * orient.w) - (currentOrient.w * orient.x));
    newOrientation.x = (currentOrient.x * orient.z) + (currentOrient.z * orient.x) + ((currentOrient.w * orient.y) - (currentOrient.y * orient.w));
    newOrientation.z = (currentOrient.z * orient.z) - ((currentOrient.x * orient.x) + (currentOrient.w * orient.w)) + (currentOrient.y * orient.y);

    SetOrient(newOrientation);
}

void Node::RotateX_Impl(const float orientX)
{
    const float negHalfOrientX = orientX * -0.5f;
    const float nhoxCos = cosf(negHalfOrientX);
    const float nhoxSin = sinf(negHalfOrientX);
    Orientation currentOrientation = BuiltinType::Orient;
    Orientation newOrientation;

    if (m_Position)
        currentOrientation = m_Position->m_Orientation;

    newOrientation.x = (currentOrientation.x * nhoxCos) + (currentOrientation.y * nhoxSin);
    newOrientation.w = (currentOrientation.z * nhoxSin) + (currentOrientation.w * nhoxCos);
    newOrientation.y = (currentOrientation.y * nhoxCos) - (currentOrientation.x * nhoxSin);
    newOrientation.z = (currentOrientation.z * nhoxCos) - (currentOrientation.w * nhoxSin);

    SetOrient(newOrientation);
}

void Node::RotateY_Impl(const float orientY)
{
    const float negHalfOrientY = orientY * -0.5f;
    const float nhoyCos = cosf(negHalfOrientY);
    const float nhoySin = sinf(negHalfOrientY);
    Orientation currentOrientation = BuiltinType::Orient;
    Orientation newOrientation;

    if (m_Position)
        currentOrientation = m_Position->m_Orientation;

    newOrientation.x = (currentOrientation.z * nhoySin) + (currentOrientation.x * nhoyCos);
    newOrientation.w = (currentOrientation.w * nhoyCos) - (currentOrientation.y * nhoySin);
    newOrientation.y = (currentOrientation.y * nhoyCos) + (currentOrientation.w * nhoySin);
    newOrientation.z = (currentOrientation.z * nhoyCos) - (currentOrientation.x * nhoySin);

    SetOrient(newOrientation);
}

void Node::RotateZ_Impl(const float orientZ)
{
    const float negHalfOrientZ = orientZ * -0.5f;
    const float nhozCos = cosf(negHalfOrientZ);
    const float nhozSin = sinf(negHalfOrientZ);
    Orientation currentOrientation = BuiltinType::Orient;
    Orientation newOrientation;

    if (m_Position)
        currentOrientation = m_Position->m_Orientation;

    newOrientation.w = (currentOrientation.w * nhozCos) + (currentOrientation.x * nhozSin);
    newOrientation.x = (currentOrientation.x * nhozCos) - (currentOrientation.w * nhozSin);
    newOrientation.y = (currentOrientation.z * nhozSin) + (currentOrientation.y * nhozCos);
    newOrientation.z = (currentOrientation.z * nhozCos) - (currentOrientation.y * nhozSin);

    SetOrient(newOrientation);
}

#pragma message(TODO_IMPLEMENTATION)
void Node::SetOrientation(const Orientation& orient)
{
    DirectX::XMMATRIX finalMatrix;
    Orientation currentOrientation = BuiltinType::Orient;

    if (m_Position)
        memcpy(finalMatrix.r[0].m128_f32, &m_Position->m_Orientation, sizeof(Orientation));
    else
        memcpy(finalMatrix.r[0].m128_f32, &BuiltinType::Orient, sizeof(Orientation));

    finalMatrix.r[1].m128_f32[0] = -finalMatrix.r[0].m128_f32[0];
    finalMatrix.r[1].m128_f32[1] = -finalMatrix.r[0].m128_f32[1];
    finalMatrix.r[1].m128_f32[2] = -finalMatrix.r[0].m128_f32[2];

    if (m_Position)
        currentOrientation = m_Position->m_Orientation;
}

void Node::GetBlockIDBelow(int* args) const
{
    EntityType* script = m_ScriptEntity;
    const int blockId = m_Id.BlockId - 1;
    if (!script)
    {
        args[0] = blockId;
        return;
    }

    while (tFolder != script)
    {
        script = script->Parent;
        if (!script)
        {
            args[0] = blockId;
            return;
        }
    }

    const int folderBlockId = ((Folder_*)this)->GetBlockId();
    if (folderBlockId)
        args[0] = folderBlockId;
    else
        args[0] = blockId;
}

void Node::GetBlockID(int* args) const
{
    args[0] = m_Id.BlockId - 1;
}

void Node::FastSetPos_Impl(const Vector4f& pos)
{
    if (!m_Position || m_Position->m_Owner != this || m_Position->m_Position == pos)
        return;

    StoreProperty(3, &m_Position->m_Position, tVECTOR);
    if (m_Collision)
    {
        if (pos.Magnitude(m_Collision->m_Position) > 100.f)
            m_Collision->m_Position = { pos.x, pos.y, pos.z };
    }

    m_Position->m_Position = pos;
}

void Node::Project(float* args)
{
    const Vector4f inVec(args[3], args[4], args[5], 0.f);
    Vector2f projectedPos;
    Scene::SceneInstance->m_ActiveCamera->Project_Impl(projectedPos, inVec);

    *(Vector3f*)args = { projectedPos.x, projectedPos.y, 0.f };
}

void Node::HasContactFlags(int* args) const
{
    if (!m_QuadTree)
        args[0] = NULL;
    else
        args[0] = (args[2] & m_QuadTree->GetContactFlags(args[1])) != NULL;
}

void Node::GetContactFlags(int* args) const
{
    if (!m_QuadTree)
        args[0] = NULL;
    else
        args[0] = m_QuadTree->GetContactFlags(args[2]);
}

#pragma message(TODO_IMPLEMENTATION)
void Node::ResolveObjectUsingProbe_Impl(const Node* node, int* a1, CollisionProbe* probe)
{
    if (!node)
    {
        LogDump::LogA("ResolveObject() can only be used on dynamic nodes");
        return;
    }
}

#pragma message(TODO_IMPLEMENTATION)
void Node::ResolveObject_Impl(const Node* node, const int a2)
{
    if (!node->m_Collision)
    {
        LogDump::LogA("ResolveObject() can only be used on dynamic nodes");
        return;
    }
}

void Node::GetLocalSpaceDirection(Vector4f& outDir, const Vector4f& inOffset) const
{
    if (!m_Position)
        return;

    if (m_Id.HasQuadTree)
        m_Position->ApplyMatrixFromQuadTree();

    DirectX::XMMATRIX mat;
    m_Position->GetMatrix(mat);

    const float x = (inOffset.z * mat.r[2].m128_f32[0]) + (inOffset.y * mat.r[1].m128_f32[0]) + (inOffset.x * mat.r[0].m128_f32[0]);
    const float y = (inOffset.z * mat.r[2].m128_f32[1]) + (inOffset.y * mat.r[1].m128_f32[1]) + (inOffset.x * mat.r[0].m128_f32[1]);
    const float z = (inOffset.z * mat.r[2].m128_f32[2]) + (inOffset.y * mat.r[1].m128_f32[2]) + (inOffset.x * mat.r[0].m128_f32[2]);
    const float a = 0.f;

    outDir = { x, y, z, a };
}

void Node::GetWorldSpaceDirection(Vector4f& outDir, const Vector4f& inOffset) const
{
    if (!m_Position)
        return;

    if (m_Id.HasQuadTree)
        m_Position->ApplyMatrixFromQuadTree();

    DirectX::XMMATRIX matOriginal;
    m_Position->GetMatrix(matOriginal);
    DirectX::XMMATRIX mat = DirectX::XMMatrixTranspose(matOriginal);

    const float x = (inOffset.y * mat.r[1].m128_f32[0]) + (inOffset.z * mat.r[2].m128_f32[0]) + (inOffset.x * mat.r[0].m128_f32[0]);
    const float y = (inOffset.y * mat.r[1].m128_f32[1]) + (inOffset.z * mat.r[2].m128_f32[1]) + (inOffset.x * mat.r[0].m128_f32[1]);
    const float z = (inOffset.y * mat.r[1].m128_f32[2]) + (inOffset.z * mat.r[2].m128_f32[2]) + (inOffset.x * mat.r[0].m128_f32[2]);
    const float a = inOffset.a;
    outDir = { x, y, z, a };
}

#pragma message(TODO_IMPLEMENTATION)
void Node::_88D230(const int a1)
{
}

#pragma message(TODO_IMPLEMENTATION)
void Node::GetAllUniqueIds(std::vector<Utils::UniqueId>& outList) const
{
    /*outList.clear();

    if (!m_Parent)
        return;

    outList.push_back(m_Fragment ? m_Fragment->m_UniqueId : Utils::UniqueId::Instance);

    Node* parent = m_Parent;
    Folder_* folderParent = (Folder_*)parent;
    while (true)
    {
        EntityType* script = parent->m_ScriptEntity;
        if (script)
        {
            while (tFolder != script)
            {
                script = script->m_Parent;
                if (!script)
                {
                    folderParent = nullptr;
                    break;
                }
            }
        }

        if (!parent->m_Fragment)
            break;

        const char* fragmentName;
        if (parent->m_Flags.HasFragment)
            if (!parent->m_Fragment->m_FragmentRes)
                break;
            else
                fragmentName = parent->m_Fragment->m_FragmentRes->GetName();
        else
            fragmentName = parent->m_Fragment->m_Name;

        if (!fragmentName || !parent->m_Parent)
            break;

        outList.push_back(parent->m_Fragment ? parent->m_Fragment->m_UniqueId : Utils::UniqueId::Instance);

        parent = parent->m_Parent;
        if (!parent)
            return;
    }

    if (!folderParent || !folderParent->GetBlockId())*/
}

void Node::SetFlags(int flags)
{
    if (!m_Flags.Volatile && (flags & 0x20) == 0 && flags != (*(int*)&m_Flags & 0xFFF))
    {
        unsigned int _flags = *(int*)&m_Flags & 0xFFF;
        StoreProperty(2, &_flags, tINTEGER);
    }

    if (((flags ^ m_Flags.Dynamic) & 2) != 0)
    {
        if (m_QuadTree && m_QuadTree->field_38)
        {
            CollisionInfo* collst = AuxQuadTree::GetForNode(this)->m_Owner->m_Collision;
            if (collst)
                collst->field_78 |= 0x80000000;
        }
    }

    *(unsigned short*)&m_Flags ^= (flags ^ *(unsigned short*)&m_Flags) & 0xFFF;
    _88E6A0(this);

    if (m_QuadTree || (m_Parent && m_Parent->GetEntityQuadTreeOrParentQuadTree()))
        m_Parent->GetEntityQuadTreeOrParentQuadTree()->m_Owner->m_Id._3 = true;
}

void Node::Instantiate()
{
    if (m_Collision)
        m_Collision->field_78 |= 0x80000000;

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
    Scene::_A3D890 = true;
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

void Node::SaveData()
{
    m_ScriptEntity->SaveData(this);
}

void Node::nullsub_4(int)
{
    return;
}

void Node::DestroyFrameBuffers()
{
    return;
}

void Node::nullsub_6(const std::vector<void*>&)
{
    return;
}

String* Node::GetResourceName(String* unk)
{
    *unk = String();

    return unk;
}

Vector4f* Node::GetBounds(Vector4f& outBounds)
{
    outBounds = BuiltinType::ZeroVector;
    return &outBounds;
}

#pragma message(TODO_IMPLEMENTATION)
Node::Node(unsigned char allocationBitmask)
{
    MESSAGE_CLASS_CREATED(Node);

    m_Flags.HasFragment = true;

    m_GlobalIdInSceneList = NULL;
    m_QuadTree = nullptr;
    m_NextSibling = nullptr;
    m_Collision = nullptr;
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

    m_Flags.HasFragment = m_Fragment != nullptr;

    m_GlobalIdInSceneList = -1;
}

Node::~Node()
{
    MESSAGE_CLASS_DESTROYED(Node);
}

const char* Node::GetTypename() const
{
    return m_ScriptEntity->TypeName.m_Str;
}

const char* Node::GetScript() const
{
    return m_ScriptEntity->Script ? m_ScriptEntity->Script->m_Name.m_Str : nullptr;
}

unsigned int Node::GetFlags() const
{
    return *(unsigned int*)&m_Flags & 0xFFF;
}

void Node::SaveScriptThreadData()
{
    if (!Scene::RewindBuffer2DataPtr)
        return;

    *Scene::RewindBuffer2DataPtr++ = (m_Id.Id << 15) | ((m_Id.BlockId & 0x7000) - 1) & 0xF000;
    if (m_ScriptData && m_ScriptData->m_ScriptThread)
    {
        *Scene::RewindBuffer2DataPtr++ = 1;
        Scene::RewindBuffer2DataPtr += m_ScriptData->m_ScriptThread->WriteScriptInformation(Scene::RewindBuffer2DataPtr);
    }
    else
    {
        *Scene::RewindBuffer2DataPtr++ = 0;
    }

    TransactionBuffer* sceneRewBuffer = Scene::SceneInstance->m_RewindBuffer2;
    if (ALIGN_4BYTES(sceneRewBuffer->m_Buffer + 4 * sceneRewBuffer->m_Chunks - Scene::RewindBuffer2DataPtr) < 0x4000)
        sceneRewBuffer->_8AA1F0(&Scene::RewindBuffer2DataPtr);

    m_ScriptSlots[0] |= 1;
}

void Node::StoreScriptData()
{
    if (!m_ScriptEntity || m_Flags._29 || m_Flags.Volatile)
        return;

    if (!m_ScriptSlots[0])
        SaveScriptThreadData();

    if (!m_PropertiesSlots[0])
    {
        int* entityDataBuffer = g_SceneSaveLoad->GetEntityDataBuffer(m_Id.Id);
        if (entityDataBuffer)
        {
            *entityDataBuffer++ = (m_Id.Id << 15) | ((m_Id.BlockId & 0x7000) - 1) & 0xF000;
            if (m_ScriptData && m_ScriptData->m_ScriptThread)
            {
                *entityDataBuffer++ = 1;
                entityDataBuffer += m_ScriptData->m_ScriptThread->WriteScriptInformation(entityDataBuffer);
            }

            g_SceneSaveLoad->SaveEntityToDataBuffer(m_Id.Id, entityDataBuffer);
        }

        m_PropertiesSlots[0] |= 1;
    }
}

void Node::SetScriptData(Defragmentator* defrag, EntityScriptData* data)
{
    const int blockinglistid = m_GlobalIdInBlockigList;
    if (blockinglistid >= 0)
    {
        NodesWithUpdateOrBlockingScripts[blockinglistid].m_Flags.m_GlobalId = -1;
        NodesWithUpdateOrBlockingScripts[blockinglistid].m_Flags.m_Id = -1;

        if (data && data->m_ScriptThread)
            NodesWithUpdateOrBlockingScripts[blockinglistid].m_Flags.PositionInStack = data->m_ScriptThread->m_StackSize;
    }

    m_ScriptData = data;
    m_Defragmentator = defrag;
}

void Node::GetWorldPos(Vector4f& pos) const
{
    DirectX::XMMATRIX mat;
    GetWorldMatrix(mat);

    pos = *(Vector4f*)&mat.r[3].m128_f32;
}

void Node::StoreProperty(const int index, const void* param, DataType* type)
{
    if (!m_ScriptEntity || m_Flags.HasFragment || m_Flags.Volatile)
        return;

    const int slotBit = 1 << (index & 7);
    if ((m_ScriptSlots[index / 8] & slotBit) == 0)
        _869EC0(index, param, *type);
    if ((m_PropertiesSlots[index / 8] & slotBit) == 0)
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

    StoreProperty(1, &m_Parent, tEntity);

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

    StoreProperty(3, &m_Position, tVECTOR);

    if (m_Collision)
    {
        if (Script::_A11C90 != 1.f)
        {
            float vecdiffx = newpos.x - m_Collision->m_Position.x;
            float vecdiffy = newpos.y - m_Collision->m_Position.y;
            float vecdiffz = newpos.z - m_Collision->m_Position.z;
            if (((vecdiffz * vecdiffz) + (vecdiffy * vecdiffy) + (vecdiffx * vecdiffx)) <= 100.f)
            {
                float negflt = 1.f - Script::_A11C90;
                m_Position->m_Position =
                {
                    (m_Collision->m_Position.x * negflt) + (newpos.x * Script::_A11C90),
                    (m_Collision->m_Position.y * negflt) + (newpos.y * Script::_A11C90),
                    (m_Collision->m_Position.z * negflt) + (newpos.z * Script::_A11C90),
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

        m_Collision->m_Position = { newpos.x, newpos.y, newpos.z };
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

    if (!m_Flags.HasFragment)
        return m_Fragment->m_Name;

    if (m_Fragment->m_FragmentRes)
        return m_Fragment->m_FragmentRes.GetAsset<FragmentAsset>()->GetName();

    return nullptr;
}

void Node::CalculateLod(unsigned int a1)
{
    if (m_QuadTree)
        m_QuadTree->CalculateLodForAllChildren();
    else
        for (Node* chld = m_FirstChild; chld; chld = chld->m_NextSibling)
            chld->CalculateLod(a1);
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
    if (!Scene::RewindBuffer1DataPtr)
        return;

    *Scene::RewindBuffer1DataPtr++ = paramind & 0xFFF | (m_Id.Id << 8) | (m_Id.BlockId - 1) & 0xF000;
    Scene::RewindBuffer1DataPtr += paramtype.CopyNoAllocate((char*)paramptr, (char*)Scene::RewindBuffer1DataPtr);

    TransactionBuffer* tb = Scene::SceneInstance->m_RewindBuffer1;
    if (tb->m_Buffer[tb->m_Chunks - (int)Scene::RewindBuffer1DataPtr] < 1024)
        tb->_8AA1F0(&Scene::RewindBuffer1DataPtr);

    m_ScriptSlots[paramind / 8] |= 1 << (paramind & 7);
}

void Node::_869F80(const unsigned int paramind, const void* paramptr, DataType& paramtype)
{
    int* dataBuffer = g_SceneSaveLoad->GetEntityDataBuffer(m_Id.Id);
    if (dataBuffer)
    {
        *dataBuffer = paramind & 0xFFF | (m_Id.Id << 8) | (m_Id.BlockId - 1) & 0xF000;
        dataBuffer++;
        g_SceneSaveLoad->SaveEntityToDataBuffer(m_Id.Id, &dataBuffer[paramtype.CopyNoAllocate((char*)paramptr, (char*)dataBuffer)]);
    }

    m_PropertiesSlots[paramind / 8] |= 1 << (paramind & 7);
}

#pragma message(TODO_IMPLEMENTATION)
void Node::TriggerGlobalScript(int scriptId, void* args)
{
    if (!m_ScriptEntity->Script || !m_ScriptEntity->Script->_48A7E0(this, scriptId, args))
    {
        EntityType::ScriptInfo* scriptinfo = m_ScriptEntity->IsBaseEntity ? m_ScriptEntity->Parent->ScriptsList[scriptId] : m_ScriptEntity->ScriptsList[scriptId];
        if (scriptinfo)
#ifdef INCLUDE_FIXES
            //  TODO: THIS IS VERY UGLY! I don't think original code had something like this!
            ((Entity*)(this + (int)scriptinfo->_f4)->*(scriptinfo->ScriptPtr))(args);
#else
            //  NOTE: this code is probably bugged so don't use it.
            if (scriptinfo->VirtualFunctionOffset)
                scriptinfo->ScriptPtr((void*)(this + (int)scriptinfo->_f4 + (int)scriptinfo->_f8), args);    //  NOTE: never used, since _fC is usually 0, same as _f4.
            else
                scriptinfo->ScriptPtr((void*)(this + (int)scriptinfo->_f4), args);
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
        NodesList[m_GlobalIdInSceneList].QDTree = 0;
        m_GlobalIdInSceneList = -1;
    }
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

    if (m_Flags.HasFragment)
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

void Node::_891E70(const String& s, String& outs)
{
    if (FileBuffer::DirectoryMappingsList.size() <= 0)
    {
        outs = s;
        return;
    }

    for (unsigned int i = 0; i < FileBuffer::DirectoryMappingsList.size(); ++i)
    {
        if (strncmp(s.m_Str, FileBuffer::DirectoryMappingsList[i].m_String_1.m_Str, FileBuffer::DirectoryMappingsList[i].m_String_1.m_Length) == NULL)
        {
            outs = FileBuffer::DirectoryMappingsList[i].m_String_2.m_Str;
            outs.Append(&s.m_Str[FileBuffer::DirectoryMappingsList[i].m_String_1.m_Length]);
        }
    }

    outs = s;
}

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
    tNode->RegisterScript("setrotationx(number)", (EntityFunctionMember)&SetRotationX, NULL, NULL, NULL, nullptr, "SetRotationXMSG");
    tNode->RegisterScript("setrotationy(number)", (EntityFunctionMember)&SetRotationY, NULL, NULL, NULL, nullptr, "SetRotationYMSG");
    tNode->RegisterScript("setrotationz(number)", (EntityFunctionMember)&SetRotationZ, NULL, NULL, NULL, nullptr, "SetRotationZMSG");
    tNode->RegisterScript("rotate(quaternion)", (EntityFunctionMember)&Rotate, NULL, NULL, NULL, nullptr, "RotateMSG");
    tNode->RegisterScript("rotatex(number)", (EntityFunctionMember)&RotateX, NULL, NULL, NULL, nullptr, "RotateXMSG");
    tNode->RegisterScript("rotatey(number)", (EntityFunctionMember)&RotateY, NULL, NULL, NULL, nullptr, "RotateYMSG");
    tNode->RegisterScript("rotatez(number)", (EntityFunctionMember)&RotateZ, NULL, NULL, NULL, nullptr, "RotateZMSG");
    tNode->RegisterScript("rotatelocal(quaternion)", (EntityFunctionMember)&RotateLocal, NULL, NULL, NULL, nullptr, "RotateLocalMSG");
    tNode->RegisterScript("rotatelocalx(number)", (EntityFunctionMember)&RotateLocalX, NULL, NULL, NULL, nullptr, "RotateLocalXMSG");
    tNode->RegisterScript("rotatelocaly(number)", (EntityFunctionMember)&RotateLocalY, NULL, NULL, NULL, nullptr, "RotateLocalYMSG");
    tNode->RegisterScript("rotatelocalz(number)", (EntityFunctionMember)&RotateLocalZ, NULL, NULL, NULL, nullptr, "RotateLocalZMSG");
    tNode->RegisterScript("converttoworldspace(vector):vector", (EntityFunctionMember)&ConvertToWorldSpace, NULL, NULL, NULL, nullptr, "ConvertToWorldSpaceMSG");
    tNode->RegisterScript("convertfromworldspace(vector):vector", (EntityFunctionMember)&ConvertFromWorldSpace, NULL, NULL, NULL, nullptr, "ConvertFromWorldSpaceMSG");
    tNode->RegisterScript("convertdirectiontoworldspace(vector):vector", (EntityFunctionMember)&ConvertDirectionToWorldSpace, NULL, NULL, NULL, nullptr, "ConvertDirectionToWorldSpaceMSG");
    tNode->RegisterScript("convertdirectionfromworldspace(vector):vector", (EntityFunctionMember)&ConvertDirectionFromWorldSpace, NULL, NULL, NULL, nullptr, "ConvertDirectionFromWorldSpaceMSG");
    tNode->RegisterScript("getworldpos:vector", (EntityFunctionMember)&GetWorldPos, NULL, NULL, NULL, nullptr, "GetWorldPosMSG");
    tNode->RegisterScript("getworldorient:quaternion", (EntityFunctionMember)&GetWorldOrient, NULL, NULL, NULL, nullptr, "GetWorldOrientMSG");
    tNode->RegisterScript("setworldpos(vector)", (EntityFunctionMember)&SetWorldPos, NULL, NULL, NULL, nullptr, "SetWorldPosMSG");
    tNode->RegisterScript("setworldorient(quaternion)", (EntityFunctionMember)&SetWorldOrient, NULL, NULL, NULL, nullptr, "SetWorldOrientMSG");
    tNode->RegisterScript("findnode(string):entity", (EntityFunctionMember)&FindNode, NULL, NULL, NULL, nullptr, "FindNodeMSG");
    tNode->RegisterScript("setcontactfilter(integer)", (EntityFunctionMember)&SetContactFilter, NULL, NULL, NULL, nullptr, "SetContactFilterMSG");
    tNode->RegisterScript("contacts:integer", (EntityFunctionMember)&Contacts, NULL, NULL, NULL, nullptr, "ContactsMSG");
    tNode->RegisterScript("getcontactnormal(integer):vector", (EntityFunctionMember)&GetContactNormal, NULL, NULL, NULL, nullptr, "GetContactNormalMSG");
    tNode->RegisterScript("getcontactpoint(integer):vector", (EntityFunctionMember)&GetContactPoint, NULL, NULL, NULL, nullptr, "GetContactPointMSG");
    tNode->RegisterScript("getpeercontactpoint(integer):vector", (EntityFunctionMember)&GetPeerContactPoint, NULL, NULL, NULL, nullptr, "GetPeerContactPointMSG");
    tNode->RegisterScript("getselfrealnode(integer):entity", (EntityFunctionMember)&GetSelfRealNode, NULL, NULL, NULL, nullptr, "GetSelfRealNodeMSG");
    tNode->RegisterScript("getcontactnode(integer):entity", (EntityFunctionMember)&GetContactNode, NULL, NULL, NULL, nullptr, "GetContactNodeMSG");
    tNode->RegisterScript("getrealnode(integer):entity", (EntityFunctionMember)&GetRealNode, NULL, NULL, NULL, nullptr, "GetRealNodeMSG");
    tNode->RegisterScript("getcontactflags(integer):integer", (EntityFunctionMember)&GetContactFlags, NULL, NULL, NULL, nullptr, "GetContactFlagsMSG");
    tNode->RegisterScript("hascontactflags(integer,integer):truth", (EntityFunctionMember)&HasContactFlags, NULL, NULL, NULL, nullptr, "HasContactFlagsMSG");
    tNode->RegisterScript("getcontactsurfacepropfields(integer):integer", (EntityFunctionMember)&GetContactSurfacePropFields, NULL, NULL, NULL, nullptr, "GetContactSurfacePropFieldsMSG");
    tNode->RegisterScript("getcontactmaterialid(integer):integer", (EntityFunctionMember)&GetContactMaterialID, NULL, NULL, NULL, nullptr, "GetContactMaterialIDMSG");
    tNode->RegisterScript("getcontactregion(integer,list(vector),integer)", (EntityFunctionMember)&GetContactRegion, NULL, NULL, NULL, nullptr, "GetContactRegionMSG");
    tNode->RegisterScript("SetSafePos(vector,quaternion)", (EntityFunctionMember)&SetSafePos, NULL, NULL, NULL, nullptr, "SetSafePosMSG");
    tNode->RegisterScript("resolveobject(integer)", (EntityFunctionMember)&ResolveObject, NULL, NULL, NULL, nullptr, "ResolveObjectMSG");
    tNode->RegisterScript("resolveobjectusingprobe(integer,entity)", (EntityFunctionMember)&ResolveObjectUsingProbe, NULL, NULL, NULL, nullptr, "ResolveObjectUsingProbeMSG");
    tNode->RegisterScript("commitcollision", (EntityFunctionMember)&CommitCollision, NULL, NULL, NULL, nullptr, "CommitCollisionMSG");
    tNode->RegisterScript("createnode(string):entity", (EntityFunctionMember)&CreateNode);
    tNode->RegisterScript("destroynode(entity)", (EntityFunctionMember)&DestroyNode);
    tNode->RegisterScript("annotatepoint(vector,integer,integer)", (EntityFunctionMember)&AnnotatePoint);
    tNode->RegisterScript("annotateline(vector,vector,integer,integer)", (EntityFunctionMember)&AnnotateLine);
    tNode->RegisterScript("annotatesphere(vector,number,integer,integer)", (EntityFunctionMember)&AnnotateSphere);
    tNode->RegisterScript("setcurrentcamera(entity)", (EntityFunctionMember)&SetCurrentCamera, NULL, NULL, NULL, nullptr, "SetCurrentCameraMSG");
    tNode->RegisterScript("getcurrentcamera:entity", (EntityFunctionMember)&GetCurrentCamera, NULL, NULL, NULL, nullptr, "GetCurrentCameraMSG");
    tNode->RegisterScript("GetSharedProbe:entity", (EntityFunctionMember)&GetSharedProbe, NULL, NULL, NULL, nullptr, "GetSharedProbeMSG");
    tNode->RegisterScript("project(vector):vector", (EntityFunctionMember)&Project, NULL, NULL, NULL, nullptr, "ProjectMSG");
    tNode->RegisterScript("getscreensize:vector", (EntityFunctionMember)&GetScreenSize, NULL, NULL, NULL, nullptr, "GetScreenSizeMSG");
    tNode->RegisterScript("getplatform:integer", (EntityFunctionMember)&GetPlatform, NULL, NULL, NULL, nullptr, "GetPlatformMSG");
    tNode->RegisterScript("forcelodcalculation", (EntityFunctionMember)&ForceLodCalculation, NULL, NULL, NULL, nullptr, "ForceLodCalculationMSG");
    tNode->RegisterScript("ignorenode(entity)", (EntityFunctionMember)&IgnoreNode, NULL, NULL, NULL, nullptr, "IgnoreNodeMSG");
    tNode->RegisterScript("removeignorenode(entity)", (EntityFunctionMember)&RemoveIgnoreNode, NULL, NULL, NULL, nullptr, "RemoveIgnoreNodeMSG");
    tNode->RegisterScript("resetignorelist", (EntityFunctionMember)&ResetIgnoreList, NULL, NULL, NULL, nullptr, "ResetIgnoreListMSG");
    tNode->RegisterScript("purgenames", (EntityFunctionMember)&PurgeNames, NULL, NULL, NULL, nullptr, "PurgeNamesMSG");
    tNode->RegisterScript("fastsetpos(vector)", (EntityFunctionMember)&FastSetPos, NULL, NULL, NULL, nullptr, "FastSetPosMSG");
    tNode->RegisterScript("fastsetorient(quaternion)", (EntityFunctionMember)&FastSetOrient, NULL, NULL, NULL, nullptr, "FastSetOrientMSG");
    tNode->RegisterScript("touchpivot", (EntityFunctionMember)&TouchPivot, NULL, NULL, NULL, nullptr, "TouchPivotMSG");
    tNode->RegisterScript("touchthispivot", (EntityFunctionMember)&TouchThisPivot);
    tNode->RegisterScript("getblockid:integer", (EntityFunctionMember)&GetBlockID, NULL, NULL, NULL, nullptr, "GetBlockIDMSG");
    tNode->RegisterScript("getblockidbelow:integer", (EntityFunctionMember)&GetBlockIDBelow, NULL, NULL, NULL, nullptr, "GetBlockIDBelowMSG");

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