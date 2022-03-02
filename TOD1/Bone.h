#pragma once
#include "Node.h"
#include <map>

struct BoneRewindData
{
    Vector4f    m_Position;
    Orientation m_Orientation;
};

class Bone : public Node
{
protected:
    static Bone* Create(AllocatorIndex); // @907220

public:
    inline Bone() : Node(NODE_MASK_POSITION) // NOTE: has no constructor.
    {
        MESSAGE_CLASS_CREATED(Bone);
    };
    virtual ~Bone();

    void            SetBoneWorldPos(float* args);   //  @907100
    void            SetBonePos(float* args);    //  @9071A0
    void            SetBoneWorldOrient(float* args);    //  @907160
    void            SetBoneOrient(float* args); //  @9071E0

    static void  Register(); // @907270

    static std::map<int, short>     BonesList;    //  @A3D8C0 //  NOTE: key - bone entity ID, value - ?
    static BoneRewindData          *BonePositionData;    //  @A3D894
};

extern EntityType* tBone; // @A3E124

ASSERT_CLASS_SIZE(Bone, 80);