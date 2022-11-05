#include "Character.h"
#include "StringType.h"
#include "TruthType.h"
#include "NumberType.h"
#include "IntegerType.h"
#include "VectorType.h"
#include "BuiltinType.h"
#include "SkinnedMeshBuffer.h"
#include "PhysSystem.h"

EntityType* tCharacter;
String Character::CurrentAttachedNode;
int Character::UpdateBoneCtrlCommand = -1;

#pragma message(TODO_IMPLEMENTATION)
Character::Character() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
{
    MESSAGE_CLASS_CREATED(Character);
}

#pragma message(TODO_IMPLEMENTATION)
Character::~Character()
{
    MESSAGE_CLASS_DESTROYED(Character);
}

#pragma message(TODO_IMPLEMENTATION)
void Character::Instantiate()
{
}

Vector4f* Character::GetBounds(Vector4f& outBounds)
{
    outBounds = BuiltinType::ZeroVector;
    return &outBounds;
}

#pragma message(TODO_IMPLEMENTATION)
void Character::Animate()
{
    if (m_Id.HasPosition)
        return;
}

void Character::AttachNode(const unsigned int index, Node* node, const String& nodeName1, const String& nodeName2)
{
    StoreProperty(16, GetAttachment(), tSTRING);
    if (m_AttachedNodesList.size() < index)
        m_AttachedNodesList.resize(index + 1);

    m_AttachedNodesList[index].m_NodeMain = node;
    m_AttachedNodesList[index].m_NodeSecond = (Node*)node->FindNode(nodeName1.m_Str);
    m_AttachedNodesList[index].m_NodeThird = (Node*)node->FindNode(nodeName2.m_Str);
}

void Character::DetachNode(const unsigned int index)
{
    StoreProperty(16, GetAttachment(), tSTRING);
    if (m_AttachedNodesList.size() < index)
        m_AttachedNodesList.resize(index + 1);

    m_AttachedNodesList[index].m_NodeMain = nullptr;
}

const int Character::GetPhysAttachmentIndex(const char* attachmentName) const
{
    if (m_PhysAttachments.size() <= 0)
        return -1;

    for (unsigned int i = 0; i < m_PhysAttachments.size(); ++i)
        if (strcmp(m_PhysAttachments[i]->m_Name, attachmentName) == NULL)
            return i;

    return -1;
}

const char* const Character::GetAttachment() const
{
    if (!m_AttachedNodesList.size())
        return nullptr;

    char buf[64] = {};
    sprintf(buf, "%02d;", m_AttachedNodesList.size());
    static String AttachedNodes(buf);

    for (unsigned int i = 0; i < m_AttachedNodesList.size(); ++i)
    {
        const AttachedNode& curAttachedNodeElement = m_AttachedNodesList[i];
        if (!curAttachedNodeElement.m_NodeMain)
            break;

        const int node3Id = curAttachedNodeElement.m_NodeThird ? curAttachedNodeElement.m_NodeThird->m_Id.Id : 0;
        const int node1Id = curAttachedNodeElement.m_NodeMain->m_Id.Id;
        const int node2Id = curAttachedNodeElement.m_NodeSecond->m_Id.Id;

        sprintf(buf, "%010d,%010d,%010d;", node1Id, node2Id, node3Id);
        AttachedNodes.Append(buf);
    }

    sprintf(buf, "%010d;", 0);
    AttachedNodes.Append(buf);

    return AttachedNodes.m_Str;
}

void Character::SetAttachment(const char* attachment)
{
    const char* const currentAttachment = GetAttachment();
    StoreProperty(16, &currentAttachment, tSTRING);

    m_AttachedNodesList.clear();

    if (!attachment)
        return;

#ifdef INCLUDE_FIXES
    const int attachmentSize = std::atoi(attachment);
#else
    const int attachmentSize = attachment[1] - 48 + 10 * (attachment[0] - 48);  //  NOTE: it is expected that first 2 'characters' are digits, convert it to actual number.
#endif
    m_AttachedNodesList.resize(attachmentSize);

    if (!m_AttachedNodesList.size())
        return;

    attachment += 3;
    unsigned int index = 0;
    while (true)
    {
#ifdef INCLUDE_FIXES
        const unsigned int node1Id = std::atoi(attachment);
#else
        //  NOTE: original code, STL does same.
        const unsigned int node1Id =
            (1000000000 * (attachment[0] - 48)) +
            (100000000 * (attachment[1] - 48)) +
            (10000000 * (attachment[2] - 48)) +
            (1000000 * (attachment[3] - 48)) +
            (100000 * (attachment[4] - 48)) +
            (10000 * (attachment[5] - 48)) +
            (1000 * (attachment[6] - 48)) +
            (100 * (attachment[7] - 48)) +
            (10 * (attachment[8] - 48)) +
            (1 * (attachment[9] - 48));
#endif
        if (node1Id)
        {
            m_AttachedNodesList[index].m_NodeMain = g_AssetManager->FindEntityById(node1Id);
            attachment += 10;

#ifdef INCLUDE_FIXES
            const unsigned int node2Id = std::atoi(attachment);
#else
            //  NOTE: original code, STL does same.
            const unsigned int node2Id =
                (1000000000 * (attachment[0] - 48)) +
                (100000000 * (attachment[1] - 48)) +
                (10000000 * (attachment[2] - 48)) +
                (1000000 * (attachment[3] - 48)) +
                (100000 * (attachment[4] - 48)) +
                (10000 * (attachment[5] - 48)) +
                (1000 * (attachment[6] - 48)) +
                (100 * (attachment[7] - 48)) +
                (10 * (attachment[8] - 48)) +
                (1 * (attachment[9] - 48));
#endif

            if (node2Id)
            {
                m_AttachedNodesList[index].m_NodeSecond = g_AssetManager->FindEntityById(node2Id);
                attachment += 10;

#ifdef INCLUDE_FIXES
                const unsigned int node3Id = std::atoi(attachment);
#else
                //  NOTE: original code, STL does same.
                const unsigned int node3Id =
                    (1000000000 * (attachment[0] - 48)) +
                    (100000000 * (attachment[1] - 48)) +
                    (10000000 * (attachment[2] - 48)) +
                    (1000000 * (attachment[3] - 48)) +
                    (100000 * (attachment[4] - 48)) +
                    (10000 * (attachment[5] - 48)) +
                    (1000 * (attachment[6] - 48)) +
                    (100 * (attachment[7] - 48)) +
                    (10 * (attachment[8] - 48)) +
                    (1 * (attachment[9] - 48));
#endif

                if (node3Id)
                {
                    m_AttachedNodesList[index].m_NodeThird = g_AssetManager->FindEntityById(node3Id);
                    attachment += 10;
                }
            }
        }

        index++;
        if (index >= m_AttachedNodesList.size())
            break;
    }
}

const bool Character::IsPlayerCharacter() const
{
    return m_Flags.IsPlayerCharacter;
}

void Character::SetIsPlayerCharacter(const bool playerchar)
{
    m_Flags.IsPlayerCharacter = playerchar;
    m_Id._3 = 5;
}

const bool Character::IsPhysicsOn() const
{
    return m_Flags_2.PhysicsOn;
}

void Character::SetPhysicsOn(const bool physicson)
{
    m_Flags_2.PhysicsOn = physicson;
}

void Character::GetPhysWorldGravityVec(Vector4f& outGravityVec) const
{
    outGravityVec = m_PhysWorldGravityVector;
}

void Character::SetPhysWorldGravityVec(const Vector4f& gravityVec)
{
    m_PhysWorldGravityVector = gravityVec;
}

const bool Character::GetCharacterSep1() const
{
    return false;
}

void Character::SetCharacterSep1(const bool)
{
}

const float Character::GetPhysPonyDamping() const
{
    return m_PhysPony ? m_PhysPony->m_Damping * 100.f : 0.f;
}

void Character::SetPhysPonyDamping(const float damping)
{
    if (m_PhysPony)
        m_PhysPony->m_Damping = damping * 0.009f;
}

const float Character::GetPhysPonyWeight() const
{
    return m_PhysPony ? m_PhysPony->m_Weight : 0.f;
}

void Character::SetPhysPonyWeight(const float weight)
{
    if (m_PhysPony)
        m_PhysPony->m_Weight = weight;
}

const float Character::GetPhysPonyRestlength() const
{
    return m_PhysPonyRestlength;
}

void Character::SetPhysPonyRestlength(const float restlength)
{
    m_PhysPonyRestlength = restlength;
    if (m_PhysPony)
    {
        auto& phyPonyList = m_PhysPony->m_RestLength;
        for (unsigned int i = 0; i < phyPonyList.size(); ++i)
            phyPonyList[i].m_RestLengthSquared = restlength * restlength;
    }
}

const int Character::GetPhysPonyNumIterations() const
{
    return m_PhysPony ? m_PhysPony->m_NumIterations : 1;
}

void Character::SetPhysPonyNumIterations(const int iterations)
{
    if (m_PhysPony)
        m_PhysPony->m_NumIterations = iterations;
}

const bool Character::PhysPonyZAxisLimitOn() const
{
    return m_Flags_3.PhysPonyZAxislimit;
}

void Character::SetPhysPonyZAxisLimitOn(const bool on)
{
    m_Flags_3.PhysPonyZAxislimit = on;
}

const float Character::GetPhysPonyZAxisEnd() const
{
    return m_PhysPonyZAxisEnd;
}

void Character::SetPhysPonyZAxisEnd(const float zaxisend)
{
    m_PhysPonyZAxisEnd = zaxisend;
}

const bool Character::GetCharacterSep2() const
{
    return false;
}

void Character::SetCharacterSep2(const bool)
{
}

const float Character::GetPhysBreastLDamping() const
{
    return m_PhysBreastL ? m_PhysBreastL->m_Damping * 100.f : 0.f;
}

void Character::SetPhysBreastLDamping(const float damping)
{
    if (m_PhysBreastL)
        m_PhysBreastL->m_Damping = damping * 0.009f;
}

const float Character::GetPhysBreastLWeight() const
{
    return m_PhysBreastL ? m_PhysBreastL->m_Weight : 0.f;
}

void Character::SetPhysBreastLWeight(const float weight)
{
    if (m_PhysBreastL)
        m_PhysBreastL->m_Weight = weight;
}

const float Character::GetPhysBreastLRestlength() const
{
    return m_PhysBreastLRestLength;
}

void Character::SetPhysBreastLRestlength(const float restlength)
{
    m_PhysBreastLRestLength = restlength;
    if (m_PhysBreastL)
    {
        auto& phyBreastLList = m_PhysBreastL->m_RestLength;
        for (unsigned int i = 0; i < phyBreastLList.size(); ++i)
            phyBreastLList[i].m_RestLengthSquared = restlength * restlength;
    }
}

const short Character::GetPhysBreastLNumIterations() const
{
    return m_PhysBreastL ? m_PhysBreastL->m_NumIterations : 1;
}

void Character::SetPhysBreastLNumIterations(const short iterations)
{
    if (m_PhysBreastL)
        m_PhysBreastL->m_NumIterations = iterations;
}

const bool Character::GetCharacterSep3() const
{
    return false;
}

void Character::SetCharacterSep3(const bool)
{
}

const float Character::GetPhysBreastRDamping() const
{
    return m_PhysBreastR ? m_PhysBreastR->m_Damping : 0.f;
}

void Character::SetPhysBreastRDamping(const float damping)
{
    if (m_PhysBreastR)
        m_PhysBreastR->m_Damping = damping * 0.009f;
}

const float Character::GetPhysBreastRWeight() const
{
    return m_PhysBreastR ? m_PhysBreastR->m_Weight : 0.f;
}

void Character::SetPhysBreastRWeight(const float weight)
{
    if (m_PhysBreastR)
        m_PhysBreastR->m_Weight = weight;
}

const float Character::GetPhysBreastRRestlength() const
{
    return m_PhysBreastLRestLength;
}

void Character::SetPhysBreastRRestlength(const float restlength)
{
    m_PhysBreastRRestLength = restlength;
    if (m_PhysBreastR)
    {
        auto& phyBreastRRestLength = m_PhysBreastR->m_RestLength;
        for (unsigned int i = 0; i < phyBreastRRestLength.size(); ++i)
            phyBreastRRestLength[i].m_RestLengthSquared = restlength * restlength;
    }
}

const short Character::GetPhysBreastRNumIterations() const
{
    return m_PhysBreastR ? m_PhysBreastR->m_NumIterations : 1;
}

void Character::SetPhysBreastRNumIterations(const short iterations)
{
    if (m_PhysBreastR)
        m_PhysBreastR->m_NumIterations = iterations;
}

const bool Character::GetCharacterSep4() const
{
    return false;
}

void Character::SetCharacterSep4(const bool)
{
}

const float Character::GetPhysBellyDamping() const
{
    return m_PhysBelly ? m_PhysBelly->m_Damping : 0.f;
}

void Character::SetPhysBellyDamping(const float damping)
{
    if (m_PhysBelly)
        m_PhysBelly->m_Damping = damping;
}

const float Character::GetPhysBellyWeight() const
{
    return m_PhysBelly ? m_PhysBelly->m_Weight : 0.f;
}

void Character::SetPhysBellyWeight(const float weight)
{
    if (m_PhysBelly)
        m_PhysBelly->m_Weight = weight;
}

const float Character::GetPhysBellyRestlength() const
{
    return m_PhysBellyRestlength;
}

void Character::SetPhysBellyRestlength(const float restlength)
{
    m_PhysBellyRestlength = restlength;
    if (m_PhysBelly)
    {
        auto& phyBellyRestLength = m_PhysBelly->m_RestLength;
        for (unsigned int i = 0; i < phyBellyRestLength.size(); ++i)
            phyBellyRestLength[i].m_RestLengthSquared = restlength * restlength;
    }
}

const short Character::GetPhysBellyNumIterations() const
{
    return m_PhysBelly ? m_PhysBelly->m_NumIterations : 1;
}

void Character::SetPhysBellyNumIterations(const short iterations)
{
    if (m_PhysBelly)
        m_PhysBelly->m_NumIterations = iterations;
}

const bool Character::GetCharacterSep5() const
{
    return false;
}

void Character::SetCharacterSep5(const bool)
{
}

const float Character::GetPhysTrouserLDamping() const
{
    return m_PhysTrouserL ? m_PhysTrouserL->m_Damping * 100.f : 0.f;
}

void Character::SetPhysTrouserLDamping(const float damping)
{
    if (m_PhysTrouserL)
        m_PhysTrouserL->m_Damping = damping * 0.009f;
}

const float Character::GetPhysTrouserLWeight() const
{
    return m_PhysTrouserL ? m_PhysTrouserL->m_Weight : 0.f;
}

void Character::SetPhysTrouserLWeight(const float weight)
{
    if (m_PhysTrouserL)
        m_PhysTrouserL->m_Weight = weight;
}

const float Character::GetPhysTrouserLRestlength() const
{
    return (float)m_PhysTrouserLRestLength;
}

void Character::SetPhysTrouserLRestlength(const float restlength)
{
    m_PhysTrouserLRestLength = (int)restlength;
    if (m_PhysTrouserL)
    {
        auto& phyTrouserLRestLength = m_PhysTrouserL->m_RestLength;
        for (unsigned int i = 0; i < phyTrouserLRestLength.size(); ++i)
            phyTrouserLRestLength[i].m_RestLengthSquared = restlength * restlength;
    }
}

const float Character::GetPhysTrouserLXLimit() const
{
    return m_PhysTrouserLXLimit;
}

void Character::SetPhysTrouserLXLimit(const float xlimit)
{
    m_PhysTrouserLXLimit = xlimit;
}

const short Character::GetPhysTrouserLNumIterations() const
{
    return m_PhysTrouserL ? m_PhysTrouserL->m_NumIterations : 1;
}

void Character::SetPhysTrouserLNumIterations(const short iterations)
{
    if (m_PhysTrouserL)
        m_PhysTrouserL->m_NumIterations = iterations;
}

const bool Character::GetCharacterSep6() const
{
    return false;
}

void Character::SetCharacterSep6(const bool)
{
}

const float Character::GetPhysTrouserRDamping() const
{
    return m_PhysTrouserR ? m_PhysTrouserR->m_Damping * 100.f : 0.f;
}

void Character::SetPhysTrouserRDamping(const float damping)
{
    if (m_PhysTrouserR)
        m_PhysTrouserR->m_Damping = damping * 0.009f;
}

const float Character::GetPhysTrouserRWeight() const
{
    return m_PhysTrouserR ? m_PhysTrouserR->m_Weight : 0.f;
}

void Character::SetPhysTrouserRWeight(const float weight)
{
    if (m_PhysTrouserR)
        m_PhysTrouserR->m_Weight = weight;
}

const float Character::GetPhysTrouserRRestlength() const
{
    return m_PhysTrouserRRestLength;
}

void Character::SetPhysTrouserRRestlength(const float restlength)
{
    m_PhysTrouserRRestLength = restlength;
    if (m_PhysTrouserRRestLength)
    {
        auto& phyTrouserRRestLength = m_PhysTrouserR->m_RestLength;
        for (unsigned int i = 0; i < phyTrouserRRestLength.size(); ++i)
            phyTrouserRRestLength[i].m_RestLengthSquared = restlength;
    }
}

const float Character::GetPhysTrouserRXLimit() const
{
    return m_PhysTrouserRXLimit;
}

void Character::SetPhysTrouserRXLimit(const float xlimit)
{
    m_PhysTrouserRXLimit = xlimit;
}

const short Character::GetPhysTrouserRNumIterations() const
{
    return m_PhysTrouserR ? m_PhysTrouserR->m_NumIterations : 1;
}

void Character::SetPhysTrouserRNumIterations(const short iterations)
{
    if (m_PhysTrouserR)
        m_PhysTrouserR->m_NumIterations = iterations;
}

const bool Character::GetCharacterSep7() const
{
    return false;
}

void Character::SetCharacterSep7(const bool)
{
}

const float Character::GetPhysTrouserSideWayTolerance() const
{
    return m_PhysTrouserSidewayTolerance;
}

void Character::SetPhysTrouserSideWayTolerance(const float tolerance)
{
    m_PhysTrouserSidewayTolerance = tolerance;
}

const float Character::GetPhysTrouserWideness() const
{
    return m_PhysTrouserWideness;
}

void Character::SetPhysTrouserWideness(const float wideness)
{
    m_PhysTrouserWideness = wideness;
}

void Character::SetModelResFromModel(int* args)
{
    SetModelResFromModel_Impl((Node*)args[0]);
}

void Character::ForceAnimationUpdate(int* args)
{
    ForceAnimationUpdate_Impl(0);
}

#pragma message(TODO_IMPLEMENTATION)
void Character::SetModelResFromModel_Impl(Node* model)
{
}

#pragma message(TODO_IMPLEMENTATION)
void Character::ForceAnimationUpdate_Impl(const int)
{
}

const char* Character::GetModelRes() const
{
    return m_ModelRes ? m_ModelRes.m_AssetPtr->GetName() : nullptr;
}

void Character::SetModelRes(const char* const modelname)
{
    const char* const modelname_ = m_ModelRes ? m_ModelRes.m_AssetPtr->GetName() : nullptr;
    StoreProperty(17, modelname_, tSTRING);

    m_ModelRes = AssetLoader(modelname);

    TryInstantiate();
}

const bool Character::IsFrozen() const
{
    return m_Flags_1.Freeze;
}

void Character::SetIsFrozen(const bool frozen)
{
    if (frozen != m_Flags_1.Freeze)
        StoreProperty(10, &frozen, tTRUTH);

    m_Id._3 = true;
    m_Flags_1.Freeze = frozen;
}

const bool Character::IsBoneControl() const
{
    return m_Flags_1.BoneControl;
}

void Character::SetIsBoneControl(const bool bonecontrol)
{
    const bool bonecontrolcurrent = m_Flags_1.BoneControl;
    StoreProperty(11, &bonecontrolcurrent, tTRUTH);
    m_Flags_1.BoneControl = bonecontrol;
}

const float Character::GetOpacity() const
{
    return m_Opacity;
}

void Character::SetOpacity(const float opacity)
{
    if (opacity != m_Opacity)
        StoreProperty(12, &m_Opacity, tNUMBER);

    m_Opacity = opacity;
}

void Character::SetRestPose()
{
    field_1B0 |= 10000;

    if (!m_ModelRes || !m_PhysAttachments.size())
        return;

    for (unsigned int i = 0; i < m_PhysAttachments.size(); ++i)
    {
        ModelAsset::Mesh& mesh = ((ModelAsset*)m_ModelRes.m_AssetPtr)->m_PivotsList[i];
        m_PhysAttachments[i]->SetPos(mesh.m_Position);
        m_PhysAttachments[i]->SetOrient(mesh.m_Orientation);
    }
}

const bool Character::ShouldDrawPhysData() const
{
    return m_Flags_2.DrawPhysData;
}

void Character::SetShouldDrawPhysData(const bool drphdata)
{
    m_Flags_2.DrawPhysData = drphdata;
}

const int Character::GetActiveTextureSet() const
{
    return m_TextureSets & 0xFFFFFF;
}

void Character::SetActiveTextureSet(const int texset)
{
    unsigned int texsetActual = (unsigned int)texset;
    const size_t texsetsTotal = GetTextureSetSize();
    if (texsetActual > texsetsTotal)
        texsetActual = texsetsTotal - 1;

    const int texsetsCurrent = m_TextureSets & 0xFFFFFF;
    if (texsetsCurrent != texsetActual)
        StoreProperty(18, &texsetsCurrent, tINTEGER);

    m_TextureSets = texsetActual;
    m_Id._3 = 5;
}

const size_t Character::GetTextureSetSize() const
{
    if (!m_ModelRes)
        return 1;

    auto& asModelPivotList = ((ModelAsset*)m_ModelRes.m_AssetPtr)->m_PivotsList;
    if (!asModelPivotList.size())
        return 1;

    size_t size = 0;
    size_t i = 0;
    for (; asModelPivotList[i].m_SkinnedMeshesList.size(); ++i)
        if (++size > asModelPivotList.size())
            return 1;

    return asModelPivotList[size].m_SkinnedMeshesList[i]->m_TextureSets.size();
}

void Character::Register()
{
    tCharacter = new EntityType("Character");
    tCharacter->InheritFrom(tNode);
    tCharacter->SetCreator((CREATOR)Create);

    tCharacter->RegisterProperty(tSTRING, "modelres", (EntityGetterFunction)&GetModelRes, NULL, NULL, NULL, (EntitySetterFunction)&SetModelRes, NULL, NULL, NULL, "control=resource|type=*.model", NULL, NULL, 17);
    tCharacter->RegisterProperty(tTRUTH, "freeze", (EntityGetterFunction)&IsFrozen, NULL, NULL, NULL, (EntitySetterFunction)&SetIsFrozen, NULL, NULL, NULL, nullptr, NULL, NULL, 10);
    tCharacter->RegisterProperty(tTRUTH, "bonecontrol", (EntityGetterFunction)&IsBoneControl, NULL, NULL, NULL, (EntitySetterFunction)&SetIsBoneControl, NULL, NULL, NULL, nullptr, NULL, NULL, 11);
    tCharacter->RegisterProperty(tNUMBER, "opacity", (EntityGetterFunction)&GetOpacity, NULL, NULL, NULL, (EntitySetterFunction)&SetOpacity, NULL, NULL, NULL, "control=slider|min=0|max=1", NULL, NULL, 12);
    tCharacter->RegisterProperty(tTRUTH, "draw_phys_data", (EntityGetterFunction)&ShouldDrawPhysData, (EntitySetterFunction)&SetShouldDrawPhysData, "control=truth");
    tCharacter->RegisterProperty(tINTEGER, "active_texture_set", (EntityGetterFunction)&GetActiveTextureSet, (EntitySetterFunction)&SetActiveTextureSet, "control=slider|min=0|max=16", 18);
    tCharacter->RegisterProperty(tINTEGER, "number_of_textures_sets", (EntityGetterFunction)&GetTextureSetSize, nullptr, "control=string");
    tCharacter->RegisterProperty(tSTRING, "attachment", (EntityGetterFunction)&GetAttachment, (EntitySetterFunction)&SetAttachment, nullptr, 16);
    tCharacter->RegisterProperty(tTRUTH, "is_player_character", (EntityGetterFunction)&IsPlayerCharacter, (EntitySetterFunction)&SetIsPlayerCharacter, "control=truth");
    tCharacter->RegisterProperty(tTRUTH, "physics_on", (EntityGetterFunction)&IsPhysicsOn, (EntitySetterFunction)&SetPhysicsOn, "control=truth");
    tCharacter->RegisterProperty(tVECTOR, "phys_world_gravity_vec", (EntityGetterFunction)&GetPhysWorldGravityVec, (EntitySetterFunction)&SetPhysWorldGravityVec, "control=string");

    tCharacter->RegisterProperty(tTRUTH, "character_sep1", (EntityGetterFunction)&GetCharacterSep1, (EntitySetterFunction)&SetCharacterSep2, "control=drawline|name=Pony Tail Physics");
    tCharacter->RegisterProperty(tNUMBER, "phys_Pony_damping", (EntityGetterFunction)&GetPhysPonyDamping, (EntitySetterFunction)&SetPhysPonyDamping, "control=slider|min=0|max=1");
    tCharacter->RegisterProperty(tNUMBER, "phys_Pony_weight", (EntityGetterFunction)&GetPhysPonyWeight, (EntitySetterFunction)&SetPhysPonyWeight, "control=slider|min=0|max=10");
    tCharacter->RegisterProperty(tNUMBER, "phys_Pony_restlength", (EntityGetterFunction)&GetPhysPonyRestlength, (EntitySetterFunction)&SetPhysPonyRestlength, "control=slider|min=0|max=2");
    tCharacter->RegisterProperty(tINTEGER, "phys_Pony_num_iterations", (EntityGetterFunction)&GetPhysPonyNumIterations, (EntitySetterFunction)&SetPhysPonyNumIterations, "control=slider|min=1|max=50");
    tCharacter->RegisterProperty(tTRUTH, "phys_Pony_z_axislimit_on", (EntityGetterFunction)&PhysPonyZAxisLimitOn, (EntitySetterFunction)&SetPhysPonyZAxisLimitOn, "control=truth");
    tCharacter->RegisterProperty(tNUMBER, "phys_Pony_z_axis_end", (EntityGetterFunction)&GetPhysPonyZAxisEnd, (EntitySetterFunction)&SetPhysPonyZAxisEnd, "control=slider|min=-1|max=1");

    tCharacter->RegisterProperty(tTRUTH, "character_sep2", (EntityGetterFunction)&GetCharacterSep2, (EntitySetterFunction)&SetCharacterSep2, "control=drawline|name=Left Breast Physics");
    tCharacter->RegisterProperty(tNUMBER, "phys_BreastL_damping", (EntityGetterFunction)&GetPhysBreastLDamping, (EntitySetterFunction)&SetPhysBreastLDamping, "control=slider|min=0|max=1");
    tCharacter->RegisterProperty(tNUMBER, "phys_BreastL_weight", (EntityGetterFunction)&GetPhysBreastLWeight, (EntitySetterFunction)&SetPhysBreastLWeight, "control=slider|min=0|max=10");
    tCharacter->RegisterProperty(tNUMBER, "phys_BreastL_restlength", (EntityGetterFunction)&GetPhysBreastLRestlength, (EntitySetterFunction)&SetPhysBreastLRestlength, "control=slider|min=0|max=2");
    tCharacter->RegisterProperty(tINTEGER, "phys_BreastL_num_iterations", (EntityGetterFunction)&GetPhysBreastLNumIterations, (EntitySetterFunction)&SetPhysBreastLNumIterations, "control=slider|min=1|max=50");

    tCharacter->RegisterProperty(tTRUTH, "character_sep3", (EntityGetterFunction)&GetCharacterSep3, (EntitySetterFunction)&SetCharacterSep3, "control=drawline|name=Right Breast Physics");
    tCharacter->RegisterProperty(tNUMBER, "phys_BreastR_damping", (EntityGetterFunction)&GetPhysBreastRDamping, (EntitySetterFunction)&SetPhysBreastRDamping, "control=slider|min=0|max=1");
    tCharacter->RegisterProperty(tNUMBER, "phys_BreastR_weight", (EntityGetterFunction)&GetPhysBreastRWeight, (EntitySetterFunction)&SetPhysBreastRWeight, "control=slider|min=0|max=10");
    tCharacter->RegisterProperty(tNUMBER, "phys_BreastR_restlength", (EntityGetterFunction)&GetPhysBreastRRestlength, (EntitySetterFunction)&SetPhysBreastRRestlength, "control=slider|min=0|max=2");
    tCharacter->RegisterProperty(tINTEGER, "phys_BreastR_num_iterations", (EntityGetterFunction)&GetPhysBreastRNumIterations, (EntitySetterFunction)&SetPhysBreastRNumIterations, "control=slider|min=1|max=50");

    tCharacter->RegisterProperty(tTRUTH, "character_sep4", (EntityGetterFunction)&GetCharacterSep4, (EntitySetterFunction)&SetCharacterSep4, "control=drawline|name=(Beer) Belly Physics");
    tCharacter->RegisterProperty(tNUMBER, "phys_Belly_damping", (EntityGetterFunction)&GetPhysBellyDamping, (EntitySetterFunction)&SetPhysBellyDamping, "control=slider|min=0|max=1");
    tCharacter->RegisterProperty(tNUMBER, "phys_Belly_weight", (EntityGetterFunction)&GetPhysBellyWeight, (EntitySetterFunction)&SetPhysBellyWeight, "control=slider|min=0|max=10");
    tCharacter->RegisterProperty(tNUMBER, "phys_Belly_restlength", (EntityGetterFunction)&GetPhysBellyRestlength, (EntitySetterFunction)&SetPhysBellyRestlength, "control=slider|min=0|max=2");
    tCharacter->RegisterProperty(tINTEGER, "phys_Belly_num_iterations", (EntityGetterFunction)&GetPhysBellyNumIterations, (EntitySetterFunction)&SetPhysBellyNumIterations, "control=slider|min=1|max=50");

    tCharacter->RegisterProperty(tTRUTH, "character_sep5", (EntityGetterFunction)&GetCharacterSep5, (EntitySetterFunction)&SetCharacterSep5, "control=drawline|name=Left Trouser Physics");
    tCharacter->RegisterProperty(tNUMBER, "phys_TrouserL_damping", (EntityGetterFunction)&GetPhysTrouserLDamping, (EntitySetterFunction)&SetPhysTrouserLDamping, "control=slider|min=0|max=1");
    tCharacter->RegisterProperty(tNUMBER, "phys_TrouserL_weight", (EntityGetterFunction)&GetPhysTrouserLWeight, (EntitySetterFunction)&SetPhysTrouserLWeight, "control=slider|min=0|max=10");
    tCharacter->RegisterProperty(tNUMBER, "phys_TrouserL_restlength", (EntityGetterFunction)&GetPhysTrouserLRestlength, (EntitySetterFunction)&SetPhysTrouserLRestlength, "control=slider|min=0|max=2");
    tCharacter->RegisterProperty(tNUMBER, "phys_TrouserL_XLimit", (EntityGetterFunction)&GetPhysTrouserLXLimit, (EntitySetterFunction)&SetPhysTrouserLXLimit, "control=slider|min=-1|max=1");
    tCharacter->RegisterProperty(tINTEGER, "phys_TrouserL_num_iterations", (EntityGetterFunction)&GetPhysTrouserLNumIterations, (EntitySetterFunction)&SetPhysTrouserLNumIterations, "control=slider|min=1|max=50");

    tCharacter->RegisterProperty(tTRUTH, "character_sep6", (EntityGetterFunction)&GetCharacterSep6, (EntitySetterFunction)&SetCharacterSep6, "control=drawline|name=Right Trouser Physics");
    tCharacter->RegisterProperty(tNUMBER, "phys_TrouserR_damping", (EntityGetterFunction)&GetPhysTrouserRDamping, (EntitySetterFunction)&SetPhysTrouserRDamping, "control=slider|min=0|max=1");
    tCharacter->RegisterProperty(tNUMBER, "phys_TrouserR_weight", (EntityGetterFunction)&GetPhysTrouserRWeight, (EntitySetterFunction)&SetPhysTrouserRWeight, "control=slider|min=0|max=10");
    tCharacter->RegisterProperty(tNUMBER, "phys_TrouserR_restlength", (EntityGetterFunction)&GetPhysTrouserRRestlength, (EntitySetterFunction)&SetPhysTrouserRRestlength, "control=slider|min=0|max=2");
    tCharacter->RegisterProperty(tNUMBER, "phys_TrouserR_XLimit", (EntityGetterFunction)&GetPhysTrouserRXLimit, (EntitySetterFunction)&SetPhysTrouserRXLimit, "control=slider|min=-1|max=1");
    tCharacter->RegisterProperty(tINTEGER, "phys_TrouserR_num_iterations", (EntityGetterFunction)&GetPhysTrouserRNumIterations, (EntitySetterFunction)&SetPhysTrouserRNumIterations, "control=slider|min=1|max=50");

    tCharacter->RegisterProperty(tTRUTH, "character_sep7", (EntityGetterFunction)&GetCharacterSep7, (EntitySetterFunction)&SetCharacterSep7, "control=drawline|name=Common Trouser Physics Parameters");
    tCharacter->RegisterProperty(tNUMBER, "phys_TrouserSideWayTolerance", (EntityGetterFunction)&GetPhysTrouserSideWayTolerance, (EntitySetterFunction)&SetPhysTrouserSideWayTolerance, "control=slider|min=0|max=0.5");
    tCharacter->RegisterProperty(tNUMBER, "phys_TrouserWideness", (EntityGetterFunction)&GetPhysTrouserWideness, (EntitySetterFunction)&SetPhysTrouserWideness, "control=slider|min=0.01|max=1.0");

    tCharacter->RegisterScript("attachnode(integer,entity,string,string)", (EntityFunctionMember)&AttachNode);
    tCharacter->RegisterScript("detachnode(integer)", (EntityFunctionMember)&DetachNode);
    tCharacter->RegisterScript("setrestpose", (EntityFunctionMember)&SetRestPose);
    tCharacter->RegisterScript("setmodelresfrommodel(entity)", (EntityFunctionMember)&SetModelResFromModel);
    tCharacter->RegisterScript("forceanimationupdate", (EntityFunctionMember)&ForceAnimationUpdate);

    UpdateBoneCtrlCommand = GetMessage("update_bone_ctrl", true);

    tCharacter->PropagateProperties();
}

Character* Character::Create(AllocatorIndex)
{
    return new Character;
}