#include "SignPost.h"

EntityType* tSignPost;
std::vector<SignPost*> SignPost::SignPostList;

SignPost* SignPost::Create(AllocatorIndex)
{
    return new SignPost;
}

SignPost::SignPost() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
{
    MESSAGE_CLASS_CREATED(SignPost);

    field_74 = 1;

    SignPostList.push_back(this);
}

#pragma message(TODO_IMPLEMENTATION)
SignPost::~SignPost()
{
    MESSAGE_CLASS_DESTROYED(SignPost);
}

std::vector<int>* SignPost::GetConnections() const
{
    return m_ConnectionsList;
}

#pragma message(TODO_IMPLEMENTATION)
void SignPost::SetConnections(void* args)
{
}

void SignPost::Register()
{
    tSignPost = new EntityType("SignPost");
    tSignPost->InheritFrom(tNode);
    tSignPost->SetCreator((CREATOR)Create);

    tSignPost->RegisterProperty(DataType::LoadScript("list(entity)"), "connections", (EntityGetterFunction)&GetConnections, (EntitySetterFunction)&SetConnections, nullptr);

    tSignPost->PropagateProperties();
}