#include "LensFlareManager.h"
#include "CollisionProbe.h"

LensFlareManager::LensFlareManager() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
{
    MESSAGE_CLASS_CREATED(LensFlareManager);

    field_64 = NULL;
    m_FrameBuffer = nullptr;
    m_FadeDistance = 100;
    m_LineCheckLength = 100;
    field_68 = { 10000, 10000, 10000, 0 };
    m_SizeReduction = 0;
    m_EmitterSize = 0;

    for (unsigned int i = 0; i < 4; ++i)
    {
        m_CollisionProbe[i] = (CollisionProbe*)tCollisionProbe->CreateNode();
        m_CollisionProbe[i]->m_ScriptEntity = tCollisionProbe;
        m_CollisionProbe[i]->SetFlags(m_CollisionProbe[i]->m_Flags.m_Flags & 0xFDF | 0x20);
        m_CollisionProbe[i]->Reset();
        m_CollisionProbe[i]->SetPos({ 1000, 1000, 1000, 0 });
    }
}

LensFlareManager::~LensFlareManager()
{
    MESSAGE_CLASS_DESTROYED(LensFlareManager);
}

void LensFlareManager::Destroy()
{
    delete m_FrameBuffer;
    for (unsigned int i = 0; i < 4; ++i)
        delete m_CollisionProbe[i];

    Node::Destroy();
}