#include "Character.h"
#include "StringType.h"

EntityType* tCharacter;
String Character::CurrentAttachedNode;

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
    SetParam(16, GetAttachment(), tSTRING);
    if (m_AttachedNodesList.size() < index)
        m_AttachedNodesList.resize(index + 1);

    m_AttachedNodesList[index].m_NodeMain = node;
    m_AttachedNodesList[index].m_NodeSecond = (Node*)node->FindNode(nodeName1.m_Str);
    m_AttachedNodesList[index].m_NodeThird = (Node*)node->FindNode(nodeName2.m_Str);
}

void Character::DetachNode(const unsigned int index)
{
    SetParam(16, GetAttachment(), tSTRING);
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
}

const char* Character::GetModelRes() const
{
    return m_ModelRes ? m_ModelRes->AddResToOpenListAndReturnName() : nullptr;
}

void Character::SetModelRes(const char* const modelname)
{
    const char* const modelname_ = m_ModelRes ? m_ModelRes->AddResToOpenListAndReturnName() : nullptr;
    SetParam(17, modelname_, tSTRING);

    AssetLoader assload(modelname);
    m_ModelRes = (ModelAsset*)assload.m_AssetPtr;

    TryInstantiate();
}

#pragma message(TODO_IMPLEMENTATION)
void Character::Register()
{
    tCharacter = new EntityType("Character");
    tCharacter->InheritFrom(tNode);
    tCharacter->SetCreator((EntityType::CREATOR)Create);

    tCharacter->RegisterProperty(tSTRING, "modelres", &GetModelRes, NULL, NULL, NULL, &SetModelRes, NULL, NULL, NULL, "control=resource|type=*.model", NULL, NULL, 17);
}

Character* Character::Create(AllocatorIndex)
{
    return new Character;
}