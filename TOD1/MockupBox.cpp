#include "MockupBox.h"
#include "BuiltinType.h"
#include "Mesh.h"

unsigned int MockupBox::TotalCreated;
MeshBuffer* MockupBox::MeshBufferPtr;
EntityType* tMockupBox;

#pragma message(TODO_IMPLEMENTATION)
MockupBox::MockupBox() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
{
	MESSAGE_CLASS_CREATED(MockupBox);

	m_Texture = nullptr;
	m_FrameBuffer = nullptr;
	field_68 = (int*)1;
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

		m.AddFace(0, { -1, -1, -1 }, (Vector3f)BuiltinType::OutVector, { 0, 0 });
		
		m.SetFaceVertexIndex(0, 0);

		MeshBufferPtr = new MeshBuffer(m);
	}

	CalculateDimensions();

	//	TODO: add to quadtree.

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

	if (m_Texture)
		g_AssetManager->DecreaseResourceReferenceCount(m_Texture);

	delete field_68;
}

void MockupBox::GetBounds(Vector4f& bounds)
{
	bounds = MeshBufferPtr ? m_Dimensions : Vector4f();
}

#pragma message(TODO_IMPLEMENTATION)
void MockupBox::Render()
{
	if (!m_QuadTree || m_QuadTree->m_Lod >= 6 || (m_QuadTree->m_LodFade * (float)(1 / 255)) == 0)
	{
		delete m_FrameBuffer;

		m_Id |= 8;
		return;
	}

	//	TODO: actually submit commands to render buffer.
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