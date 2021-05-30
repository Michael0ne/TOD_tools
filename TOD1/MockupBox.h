#pragma once
#include "Node.h"
#include "MeshBuffer.h"
#include "FrameBuffer.h"
#include "TextureAsset.h"
#include <vector>

class MockupBox : public Node
{
private:
    float               m_Left;
    float               m_Right;
    float               m_Front;
    float               m_Back;
    float               m_Height;
    TextureAsset       *m_Texture;
    int                *field_68;
    float               m_Opacity;
    FrameBuffer        *m_FrameBuffer;
    MeshBuffer         *m_MeshBuffer;
    char                m_MetricTextureMode;
    float               m_MetricTextureMultiplier;
    int                *field_80;
    unsigned int        m_Flags;
    Vector4f            m_Dimensions;
    std::vector<int>    m_LightsList;
    std::vector<int>    m_List_2;
    int				    field_B8;
    int				    field_BC;
    int				    field_C0;
public:
    MockupBox();	//	@8CF870
    virtual            ~MockupBox();   //  @8D1170
    virtual void        GetBounds(Vector4f& bounds);   //  @8CE330
    virtual void        Render();   //  @8CE930

    static unsigned int TotalCreated;	//	@A3DFD8
    static MeshBuffer* MeshBufferPtr;   //  @A3DFD4

    static void         Register(); //  @8D1260

private:
    void                CalculateDimensions();  //  @8CE200

    static MockupBox*   Create(AllocatorIndex);   //  @8D1720
};

extern EntityType* tMockupBox;  //  @A3DFDC

ASSERT_CLASS_SIZE(MockupBox, 196);