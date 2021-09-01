#include "Cloth.h"
#include "PhysSystem.h"
#include "AssetManager.h"
#include "ModelAsset.h"
#include "FrameBuffer.h"
#include "MeshBuffer_Dx9.h"

const float Cloth::MaximumImpulse = 9999999.0;

Cloth::Cloth() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
{
    MESSAGE_CLASS_CREATED(Cloth);

    m_ModelAsset = nullptr;
    field_68 = 1;
    m_FrameBuffer = nullptr;
    m_MeshBuffer = nullptr;
    field_A8 = NULL;
    field_AC = 1;
    field_B0 = -1;
    m_PhysSystem = new PhysSystem(120, this, 1);
    m_Weight = 1;
    m_Damping = 1;
    m_Opacity = 1;
    m_WindInfluence = 10;
    m_Flags.Iterations = 2;
    m_BoundingRadius = { 0, 0, 0, 0.5 };

    m_PhysSystem->m_Weight = 1;
    m_PhysSystem->m_Damping = 0.0099999998f;
    m_PhysSystem->m_NumIterations = 2;
}

Cloth::~Cloth()
{
    MESSAGE_CLASS_DESTROYED(Cloth);

    delete m_FrameBuffer;
    delete m_PhysSystem;
    delete m_MeshBuffer;

    g_AssetManager->DecreaseResourceReferenceCount(m_ModelAsset);
}

#pragma message(TODO_IMPLEMENTATION)
void Cloth::Instantiate()
{
    Node::Instantiate();
}

#pragma message(TODO_IMPLEMENTATION)
void Cloth::Update()
{
}

#pragma message(TODO_IMPLEMENTATION)
void Cloth::Render()
{
}

Cloth* Cloth::Create(AllocatorIndex)
{
    return new Cloth;
}