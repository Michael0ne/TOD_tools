#pragma once
#include "Node.h"
#include "Sprite.h"

//  NOTE: actual header file path "/Kernel/Miscellaneous/LoadScreenNode.h".
class LoadScreenNode : public Node
{
protected:
    String      m_ImageFile;
    Node       *m_RenderOnTopNode;
    Sprite     *m_LoadBarSprite;

    void        Activate_Impl(); // @87C790

public:
    LoadScreenNode(); // @87CAE0
    virtual ~LoadScreenNode();  //  @87CB50

    const char* GetImageFile() const; // @8A9B10
    void        SetImageFile(const char*); // @87C800

    void        Activate(); // @87CAD0
    void        Deactivate(int); // @87C9D0

    void        SetRenderOnTopNode(Node* _node); // @87C6E0
    void        SetLoadbarSprite(class Sprite* _sprite); // @87C9E0

    static void Register(); // @87C860

private:
    static LoadScreenNode*  Create(AllocatorIndex); // @87CAE0
};

extern EntityType*  tLoadScreenNode; // @A3D7E4

ASSERT_CLASS_SIZE(LoadScreenNode, 104);