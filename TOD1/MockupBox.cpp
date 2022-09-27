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
        m.AddVertex(1, { 1, -1, -1 }, (Vector3f)BuiltinType::OutVector, { 1, 0 });
        m.AddVertex(2, { 1, 1, -1 }, (Vector3f)BuiltinType::OutVector, { 1, 1 });
        m.AddVertex(3, { -1, 1, -1 }, (Vector3f)BuiltinType::OutVector, { 0, 1 });
        m.AddVertex(4, { 1, -1, 1 }, (Vector3f)BuiltinType::InVector, { 0, 0 });
        m.AddVertex(5, { -1, -1, 1 }, (Vector3f)BuiltinType::InVector, { 1, 0 });
        m.AddVertex(6, { -1, 1, 1 }, (Vector3f)BuiltinType::InVector, { 1, 1 });
        m.AddVertex(7, { 1, 1, 1 }, (Vector3f)BuiltinType::InVector, { 0, 1 });
        m.AddVertex(8, { -1, -1, 1 }, (Vector3f)BuiltinType::LeftVector, { 0, 0 });
        m.AddVertex(9, { -1, -1, -1 }, (Vector3f)BuiltinType::LeftVector, { 1, 0 });
        m.AddVertex(10, { -1, 1, -1 }, (Vector3f)BuiltinType::LeftVector, { 1, 1 });
        m.AddVertex(11, { -1, 1, 1 }, (Vector3f)BuiltinType::LeftVector, { 0, 1 });
        m.AddVertex(12, { 1, -1, -1 }, (Vector3f)BuiltinType::RightVector, { 0, 0 });
        m.AddVertex(13, { 1, -1, 1 }, (Vector3f)BuiltinType::RightVector, { 1, 0 });
        m.AddVertex(14, { 1, 1, 1 }, (Vector3f)BuiltinType::RightVector, { 1, 1 });
        m.AddVertex(15, { 1, 1, -1 }, (Vector3f)BuiltinType::RightVector, { 0, 1 });
        m.AddVertex(16, { -1, 1, -1 }, (Vector3f)BuiltinType::UpVector, { 0, 0 });
        m.AddVertex(17, { 1, 1, -1 }, (Vector3f)BuiltinType::UpVector, { 1, 0 });
        m.AddVertex(18, { 1, 1, 1 }, (Vector3f)BuiltinType::UpVector, { 1, 1 });
        m.AddVertex(19, { -1, 1, 1 }, (Vector3f)BuiltinType::UpVector, { 0, 1 });
        m.AddVertex(20, { 1, -1, -1 }, (Vector3f)BuiltinType::DownVector, { 0, 0 });
        m.AddVertex(21, { -1, -1, -1 }, (Vector3f)BuiltinType::DownVector, { 1, 0 });
        m.AddVertex(22, { -1, -1, 1 }, (Vector3f)BuiltinType::DownVector, { 1, 1 });
        m.AddVertex(23, { 1, -1, 1 }, (Vector3f)BuiltinType::DownVector, { 0, 1 });

        m.SetVertexIndex(0, 0);
        m.SetVertexIndex(1, 1);
        m.SetVertexIndex(2, 2);
        m.SetVertexIndex(3, 0);
        m.SetVertexIndex(4, 2);
        m.SetVertexIndex(5, 3);
        m.SetVertexIndex(6, 4);
        m.SetVertexIndex(7, 5);
        m.SetVertexIndex(8, 6);
        m.SetVertexIndex(9, 4);
        m.SetVertexIndex(10, 6);
        m.SetVertexIndex(11, 7);
        m.SetVertexIndex(12, 8);
        m.SetVertexIndex(13, 9);
        m.SetVertexIndex(14, 10);
        m.SetVertexIndex(15, 8);
        m.SetVertexIndex(16, 10);
        m.SetVertexIndex(17, 11);
        m.SetVertexIndex(18, 12);
        m.SetVertexIndex(19, 13);
        m.SetVertexIndex(20, 14);
        m.SetVertexIndex(21, 12);
        m.SetVertexIndex(22, 14);
        m.SetVertexIndex(23, 15);
        m.SetVertexIndex(24, 16);
        m.SetVertexIndex(25, 17);
        m.SetVertexIndex(26, 18);
        m.SetVertexIndex(27, 16);
        m.SetVertexIndex(28, 18);
        m.SetVertexIndex(29, 19);
        m.SetVertexIndex(30, 20);
        m.SetVertexIndex(31, 21);
        m.SetVertexIndex(32, 22);
        m.SetVertexIndex(33, 20);
        m.SetVertexIndex(34, 22);
        m.SetVertexIndex(35, 23);

        MeshBufferPtr = new MeshBuffer(&m, 0);
    }

    CalculateDimensions();

    m_Flags.Solid = true;
    m_Flags.BulletProof = true;
    m_Flags.Opaque = true;
    m_Flags.CameraBlock = true;
    m_Flags.Physics = true;

    field_80 = new int[5];  //  TODO: turn into struct constructor call once known.
    m_QuadTree->_8A3810(field_80);

    m_Flags.DynamicallyLit = true;
    m_Flags.StaticallyLit = true;
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

#pragma message(TODO_IMPLEMENTATION)
void MockupBox::Instantiate()
{
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

#pragma message(TODO_IMPLEMENTATION)
char MockupBox::ProcessCollision(int, int)
{
    return 0;
}

#pragma message(TODO_IMPLEMENTATION)
float MockupBox::_8F8650(int, int)
{
    return 0.0f;
}

void MockupBox::DestroyFrameBuffers()
{
    delete m_FrameBuffer;

    m_Id._3 = 1;
}

#pragma message(TODO_IMPLEMENTATION)
void MockupBox::nullsub_3(int)
{
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