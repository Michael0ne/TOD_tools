#include "Character.h"
#include "StringType.h"
#include "TruthType.h"
#include "NumberType.h"
#include "IntegerType.h"
#include "VectorType.h"

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

#pragma message(TODO_IMPLEMENTATION)
const char* Character::GetAttachment() const
{
    if (!m_AttachedNodesList.size())
        return nullptr;

    return nullptr;
}

const char* Character::GetModelRes() const
{
    return m_ModelRes ? m_ModelRes->GetName() : nullptr;
}

void Character::SetModelRes(const char* const modelname)
{
    const char* const modelname_ = m_ModelRes ? m_ModelRes->GetName() : nullptr;
    StoreProperty(17, modelname_, tSTRING);

    AssetLoader assload(modelname);
    m_ModelRes = (ModelAsset*)assload.m_AssetPtr;

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
        ModelAsset::Mesh& mesh = m_ModelRes->m_PivotsList[i];
        m_PhysAttachments[i]->SetPos(mesh.m_Position);
        m_PhysAttachments[i]->SetOrient(mesh.m_Orientation);
    }
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
    tCharacter->RegisterProperty(tINTEGER, "number_of_textures_sets", (EntityGetterFunction)&GetNumberOfTexturesSets, nullptr, "control=string");
    tCharacter->RegisterProperty(tSTRING, "attachment", (EntityGetterFunction)&GetAttachment, (EntitySetterFunction)&SetAttachment, nullptr, 16);
    tCharacter->RegisterProperty(tTRUTH, "is_player_character", (EntityGetterFunction)&IsPlayerCharacter, (EntitySetterFunction)&SetIsPlayerCharacter, "control=truth");
    tCharacter->RegisterProperty(tTRUTH, "physics_on", (EntityGetterFunction)&IsPhysicsOn, (EntitySetterFunction)&SetPhysicsOn, "control=truth");
    tCharacter->RegisterProperty(tVECTOR, "phys_world_gravity_vec", (EntityGetterFunction)&GetPhysWorldGravityVec, (EntitySetterFunction)&SetPhysWorldGravityVec, "control=string");

    tCharacter->RegisterProperty(tTRUTH, "character_sep1", (EntityGetterFunction)&GetCharacterSep1, (EntitySetterFunction)&SetCharacterSep2, "control=drawline|name=Pony Tail Physics");
    tCharacter->RegisterProperty(tNUMBER, "phys_Pony_damping", (EntityGetterFunction)&GetPhysPonyDamping, (EntitySetterFunction)&SetPhysPonyDamping, "control=slider|min=0|max=1");
    tCharacter->RegisterProperty(tNUMBER, "phys_Pony_weight", (EntityGetterFunction)&GetPhysPonyWeight, (EntitySetterFunction)&SetPhysPonyWeight, "control=slider|min=0|max=10");
    tCharacter->RegisterProperty(tNUMBER, "phys_Pony_restlength", (EntityGetterFunction)&GetPhysPonyRestlength, (EntitySetterFunction)&SetPhysPonyRestlength, "control=slider|min=0|max=2");
    tCharacter->RegisterProperty(tINTEGER, "phys_Pony_num_iterations", (EntityGetterFunction)&GetPhysPonyNumIterations, (EntitySetterFunction)&SetPhysPonyNumIterations, "control=slider|min=1|max=50");
    tCharacter->RegisterProperty(tTRUTH, "phys_Pony_z_axislimit_on", (EntityGetterFunction)&GetPhysPonyZAxislimitOn, (EntitySetterFunction)&SetPhysPonyZAxislimitOn, "control=truth");
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

    UpdateBoneCtrlCommand = RegisterGlobalCommand("update_bone_ctrl", true);

    tCharacter->PropagateProperties();
}

Character* Character::Create(AllocatorIndex)
{
    return new Character;
}