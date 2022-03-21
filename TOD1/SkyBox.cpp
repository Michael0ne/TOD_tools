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
const int SkyBox::StaticMeshIndicies[7] =
{
    0, 1, 2, 0, 2, 3, -1
};

SkyBox::SkyBox() : Model()
{
    MESSAGE_CLASS_CREATED(SkyBox);

    m_QuadTree->m_UserType = m_QuadTree->m_UserType & 0xFFFFFF | m_QuadTree->m_UserType & 0xFF000000 | 0x80000000;

    if (!StaticMesh)
        CreateStaticMesh();
}

SkyBox::~SkyBox()
{
    MESSAGE_CLASS_DESTROYED(SkyBox);

    for (unsigned int i = 0; i < 6; ++i)
        delete m_FrameBuffer[i];
}

#pragma message(TODO_IMPLEMENTATION)
void SkyBox::Render()
{
}

void SkyBox::Register()
{
    tSkyBox = new EntityType("SkyBox");
    tSkyBox->InheritFrom(tModel);
    tSkyBox->SetCreator((CREATOR)Create);

    tSkyBox->PropagateProperties();
}

#pragma message(TODO_IMPLEMENTATION)
void SkyBox::CreateStaticMesh() const
{
}

SkyBox* SkyBox::Create(AllocatorIndex)
{
    return new SkyBox;
}