#include "SkyBox.h"

EntityType* tSkyBox;
int* SkyBox::StaticMesh[5];
const SkyBox::SkyBoxMesh SkyBox::StaticMeshInfo[5] =
{
  {
	{ {  1.0,  1.0, -1.0,  1.0 }, {  0.0,  0.0,  1.0,  0.0 } },
	{ {  1.0,  1.0,  1.0,  1.0 }, {  0.0,  0.0,  1.0,  0.0 } },
	{ {  0.0, -1.0,  1.0,  1.0 }, {  0.0,  0.0,  1.0,  1.0 } },
	{ {  0.0, -1.0, -1.0, -1.0 }, { -1.0,  0.0,  0.0,  1.0 } }
  },
  {
	{ {  1.0, -1.0, -1.0,  1.0 }, { -1.0,  0.0,  0.0,  0.0 } },
	{ {  1.0, -1.0,  1.0,  1.0 }, { -1.0,  0.0,  0.0,  0.0 } },
	{ {  0.0, -1.0,  1.0, -1.0 }, { -1.0,  0.0,  0.0,  1.0 } },
	{ {  0.0,  1.0, -1.0, -1.0 }, {  0.0,  0.0, -1.0,  1.0 } }
  },
  {
	{ {  1.0, -1.0, -1.0, -1.0 }, {  0.0,  0.0, -1.0,  0.0 } },
	{ {  1.0, -1.0,  1.0, -1.0 }, {  0.0,  0.0, -1.0,  0.0 } },
	{ {  0.0,  1.0,  1.0, -1.0 }, {  0.0,  0.0, -1.0,  1.0 } },
	{ {  0.0,  1.0, -1.0,  1.0 }, {  1.0,  0.0,  0.0,  1.0 } }
  },
  {
	{ {  1.0,  1.0, -1.0, -1.0 }, {  1.0,  0.0,  0.0,  0.0 } },
	{ {  1.0,  1.0,  1.0, -1.0 }, {  1.0,  0.0,  0.0,  0.0 } },
	{ {  0.0,  1.0,  1.0,  1.0 }, {  1.0,  0.0,  0.0,  1.0 } },
	{ {  0.0, -1.0,  1.0,  1.0 }, {  0.0,  1.0,  0.0,  0.0 } }
  },
  {
	{ {  0.0,  1.0,  1.0,  1.0 }, {  0.0,  1.0,  0.0,  1.0 } },
	{ {  0.0,  1.0,  1.0, -1.0 }, {  0.0,  1.0,  0.0,  1.0 } },
	{ {  1.0, -1.0,  1.0, -1.0 }, {  0.0,  1.0,  0.0,  0.0 } },
	{ {  1.0,  0.0,  0.0,  0.0 }, {  0.0,  0.0,  0.0,  0.0 } }
  }
};
const unsigned short SkyBox::StaticMeshIndicies[12] =
{
	0, 0, 1, 0, 2, 0, 0, 0, 2, 0, 3, 0
};

SkyBox::SkyBox() : Model()
{
	MESSAGE_CLASS_CREATED(SkyBox);

	m_QuadTree->field_1C = m_QuadTree->field_1C & 0xFFFFFF | m_QuadTree->field_1C & 0xFF000000 | 0x80000000;

	if (!StaticMesh)
		CreateStaticMesh();
}

SkyBox::~SkyBox()
{
	MESSAGE_CLASS_DESTROYED(SkyBox);

	delete m_FrameBuffer;
}

#pragma message(TODO_IMPLEMENTATION)
void SkyBox::Render()
{
}

void SkyBox::Register()
{
	tSkyBox = new EntityType("SkyBox");
	tSkyBox->InheritFrom(tModel);
	tSkyBox->SetCreator((EntityType::CREATOR)Create);

	tSkyBox->_86E9B0();
}

#pragma message(TODO_IMPLEMENTATION)
void SkyBox::CreateStaticMesh() const
{
}

SkyBox* SkyBox::Create(AllocatorIndex)
{
	return new SkyBox;
}