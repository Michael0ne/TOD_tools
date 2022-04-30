#include "MockupBox.h"
#include "BuiltinType.h"
#include "Mesh.h"
#include "IntegerType.h"
#include "NumberType.h"
#include "StringType.h"
#include "TruthType.h"
#include "Light.h"

unsigned int MockupBox::TotalCreated;
MeshBuffer* MockupBox::MeshBufferPtr;
EntityType* tMockupBox;

#pragma message(TODO_IMPLEMENTATION)
MockupBox::MockupBox() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
{
    MESSAGE_CLASS_CREATED(MockupBox);

    m_FrameBuffer = nullptr;
    field_B8 = 0;
    field_BC = 1;
    field_C0 = -1;
    m_Left = -1.5;
    m_Front = -1.5;
    m_Height = 3;
    m_Right = 1.5;
    m_Back = 1.5;
    m_Opacity = 1;

    TotalCreated++;

    if (!MeshBufferPtr)
    {
        Mesh m(1, 0, 1);

        m.AddVertex(0, { -1, -1, -1 }, (Vector3f)BuiltinType::OutVector, { 0, 0 });
        m.SetVertexIndex(0, 0);
        // TODO: Add 22 more faces.

        MeshBufferPtr = new MeshBuffer(&m, 0);
    }

    CalculateDimensions();

    // TODO: add to quadtree.

    m_MetricTextureMode = 0;
    m_MeshBuffer = nullptr;
    m_MetricTextureMultiplier = 1;
}

#pragma message(TODO_IMPLEMENTATION)
MockupBox::~MockupBox()
{
    MESSAGE_CLASS_DESTROYED(MockupBox);

    TotalCreated--;

    delete m_FrameBuffer;

    if (!TotalCreated)
        delete MeshBufferPtr;

    delete m_MeshBuffer;
    delete field_80;
}

Vector4f* MockupBox::GetBounds(Vector4f& bounds)
{
    bounds = MeshBufferPtr ? m_Dimensions : Vector4f();
    return &bounds;
}

#pragma message(TODO_IMPLEMENTATION)
void MockupBox::Render()
{
    if (!m_QuadTree || m_QuadTree->m_Lod >= 6 || (m_QuadTree->m_LodFade * (float)(1 / 255)) == 0)
    {
        delete m_FrameBuffer;

        m_Id._3 = true;
        return;
    }

    // TODO: actually submit commands to render buffer.
}

const float MockupBox::GetLeft() const
{
    return m_Left;
}

void MockupBox::SetLeft(const float left)
{
    m_Left = left;

    TryInstantiate();
}

const float MockupBox::GetRight() const
{
    return m_Right;
}

void MockupBox::SetRight(const float right)
{
    m_Right = right;

    TryInstantiate();
}

const float MockupBox::GetFront() const
{
    return m_Front;
}

void MockupBox::SetFront(const float front)
{
    m_Front = front;

    TryInstantiate();
}

const float MockupBox::GetBack() const
{
    return m_Back;
}

void MockupBox::SetBack(const float back)
{
    m_Back = back;

    TryInstantiate();
}

const float MockupBox::GetHeight() const
{
    return m_Height;
}

void MockupBox::SetHeight(const float height)
{
    m_Height = height;

    TryInstantiate();
}

const char* MockupBox::GetTexture() const
{
    return m_TextureAsset.m_AssetPtr ? m_TextureAsset.m_AssetPtr->GetName() : nullptr;
}

void MockupBox::SetTexture(const char* const texturename)
{
    m_TextureAsset = AssetLoader(texturename);

    TryInstantiate();
}

const float MockupBox::GetOpacity() const
{
    return m_Opacity;
}

void MockupBox::SetOpacity(const float opacity)
{
    m_Opacity = opacity;

    m_Id._3 = true;
}

const int MockupBox::GetCollisionFlags() const
{
    return *(int*)&m_Flags;
}

void MockupBox::SetCollisionFlags(const int flags)
{
    *(char*)&m_Flags = (char)flags;

    Instantiate();
}

const bool MockupBox::GetMetricTextureMode() const
{
    return m_MetricTextureMode;
}

void MockupBox::SetMetricTextureMode(const bool enabled)
{
    m_MetricTextureMode;

    TryInstantiate();
}

const float MockupBox::GetMetricTextureMultiplier() const
{
    return m_MetricTextureMultiplier;
}

void MockupBox::SetMetricTextureMultiplier(const float mult)
{
    m_MetricTextureMultiplier = mult;

    TryInstantiate();
}

const bool MockupBox::IsDynamicallyLit() const
{
    return m_Flags.DynamicallyLit;
}

void MockupBox::SetDynamicallyLit(const bool lit)
{
    if (m_Flags.DynamicallyLit != lit)
        m_Id._3 = true;

    m_Id._3 = 8;

    m_Flags.DynamicallyLit = lit;
    Light::GetGlobalList()->m_StaticLightsTotal++;
}

const bool MockupBox::IsStaticallyLit() const
{
    return m_Flags.StaticallyLit;
}

void MockupBox::SetStaticallyLit(const bool lit)
{
    if (m_Flags.StaticallyLit != lit)
        m_Id._3 = true;

    m_Id._3 = 8;

    m_Flags.StaticallyLit = lit;
    Light::GetGlobalList()->m_StaticLightsTotal++;
}

void MockupBox::Register()
{
    tMockupBox = new EntityType("MockupBox");
    tMockupBox->InheritFrom(tNode);
    tMockupBox->SetCreator((CREATOR)Create);

    tMockupBox->RegisterProperty(tNUMBER, "left", (EntityGetterFunction)&GetLeft, (EntitySetterFunction)&SetLeft, "control=slider|min=-10|max=0|step=0.1");
    tMockupBox->RegisterProperty(tNUMBER, "right", (EntityGetterFunction)&GetRight, (EntitySetterFunction)&SetRight, "control=slider|min=0|max=10|step=0.1");
    tMockupBox->RegisterProperty(tNUMBER, "front", (EntityGetterFunction)&GetFront, (EntitySetterFunction)&SetFront, "control=slider|min=-10|max=0|step=0.1");
    tMockupBox->RegisterProperty(tNUMBER, "back", (EntityGetterFunction)&GetBack, (EntitySetterFunction)&SetBack, "control=slider|min=0|max=10|step=0.1");
    tMockupBox->RegisterProperty(tNUMBER, "height", (EntityGetterFunction)&GetHeight, (EntitySetterFunction)&SetHeight, "control=slider|min=0|max=10|step=0.1");
    tMockupBox->RegisterProperty(tSTRING, "texture", (EntityGetterFunction)&GetTexture, (EntitySetterFunction)&SetTexture, "control=resource|type=*.bmp;*.tga");
    tMockupBox->RegisterProperty(tNUMBER, "opacity", (EntityGetterFunction)&GetOpacity, (EntitySetterFunction)&SetOpacity, "control=slider|min=0|max=1");
    tMockupBox->RegisterProperty(tINTEGER, "CollisionFlags", (EntityGetterFunction)&GetCollisionFlags, (EntitySetterFunction)&SetCollisionFlags, "control=flags|flag0=Solid|flag1=BulletProof|flag2=Opaque|flag3=CameraBlock|flag4=Trigger|flag5=NavigationBlock|flag6=Physics|flag7=Activate triggers");
    tMockupBox->RegisterProperty(tTRUTH, "metrictexturemode", (EntityGetterFunction)&GetMetricTextureMode, (EntitySetterFunction)&SetMetricTextureMode, "control=truth");
    tMockupBox->RegisterProperty(tNUMBER, "metrictexturemultiplier", (EntityGetterFunction)&GetMetricTextureMultiplier, (EntitySetterFunction)&SetMetricTextureMultiplier, "control=slider|min=0|max=10|step=0.1");
    tMockupBox->RegisterProperty(tTRUTH, "dynamically_lit", (EntityGetterFunction)&IsDynamicallyLit, (EntitySetterFunction)&SetDynamicallyLit, "control=truth|name=");
    tMockupBox->RegisterProperty(tTRUTH, "statically_lit", (EntityGetterFunction)&IsStaticallyLit, (EntitySetterFunction)&SetStaticallyLit, "control=truth|name=");

    tMockupBox->PropagateProperties();
}

void MockupBox::CalculateDimensions()
{
    const float hfrbk = (m_Front + m_Back) * 0.5f;
    const float hheig = m_Height * 0.5f;
    const float hlfrt = (m_Right + m_Left) * 0.5f;
    const float sq = sqrtf(
        (m_Back - hfrbk) * (m_Back - hfrbk) +
        (m_Height - hheig) * (m_Height - hheig) +
        (m_Left - hlfrt) * (m_Left - hlfrt)
    );

    m_Dimensions = { hlfrt, hheig, hfrbk, sq };
}

MockupBox* MockupBox::Create(AllocatorIndex)
{
    return new MockupBox;
}