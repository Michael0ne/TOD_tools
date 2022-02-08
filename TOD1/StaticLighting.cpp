#include "StaticLighting.h"

EntityType* tStaticLighting;

#pragma message(TODO_IMPLEMENTATION)
StaticLighting::~StaticLighting()
{
    MESSAGE_CLASS_DESTROYED(StaticLighting);
}

void StaticLighting::Instantiate()
{
    Node::Instantiate();

    if (!m_LightingFile)
        return;

    Node* n = _8A6EB0();
    if (n)
        _8A6E10(n, n);
}

#pragma message(TODO_IMPLEMENTATION)
void StaticLighting::Register()
{
    tStaticLighting = new EntityType("StaticLighting");
    tStaticLighting->InheritFrom(tNode);
    tStaticLighting->SetCreator((CREATOR)Create);

}

#pragma message(TODO_IMPLEMENTATION)
Node* StaticLighting::_8A6EB0()
{
    return nullptr;
}

#pragma message(TODO_IMPLEMENTATION)
void StaticLighting::_8A6E10(Node*, Node*)
{
}

StaticLighting* StaticLighting::Create(AllocatorIndex)
{
    return new StaticLighting;
}