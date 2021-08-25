#include "CharacterPlaceholder.h"

EntityType* tCharacterPlaceHolder;

CharacterPlaceHolder::~CharacterPlaceHolder()
{
    MESSAGE_CLASS_DESTROYED(CharacterPlaceHolder);

    delete m_DynamicNodeName;
}

Vector4f* CharacterPlaceHolder::GetBounds(Vector4f& outbounds) const
{
    outbounds = {0, 0, 0, 1};
    return &outbounds;
}

CharacterPlaceHolder* CharacterPlaceHolder::Create(AllocatorIndex)
{
    return new CharacterPlaceHolder;
}

#pragma message(TODO_IMPLEMENTATION)
void CharacterPlaceHolder::Register()
{
    tCharacterPlaceHolder = new EntityType("CharacterPlaceHolder");
    tCharacterPlaceHolder->InheritFrom(tNode);
    tCharacterPlaceHolder->SetCreator((EntityType::CREATOR)Create);
}