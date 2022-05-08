#pragma once
#include "Node.h"
#include <vector>

class CollisionProbe : public Node
{
    struct CachedProbe 
    {
        std::vector<Node*>  List_1;
        std::vector<int>    m_HullsList;
        Node               *field_14;
        int                 field_18;
        CachedProbe        *m_Next;
        int                *field_20;
    };
protected:
    Vector4f                m_ResolvedPos;
    Vector4f                m_ContactPos;
    Vector4f                m_ClosestNormal;
    Node                   *m_ClosestNode;
    Node                   *m_RealNode;
    int                     m_ClosestCollisionVolume;
    float                   m_MinDistance;
    std::vector<Node*>      m_TouchingNodes;
    int                     field_A0;
    int                     m_SurfaceID;
    int                     m_MaterialID;
    int                     field_AC;
    float                   m_Angle;
    float                   m_Radius;
    std::vector<Node*>      m_IgnoredNodes;
    int                     m_DynamicMask;
    int                     m_UserMask;
    CachedProbe            *m_CachedProbes;
    std::vector<Node*>      m_Nodes;
    int                     field_E4;
    int                     m_CollisionMask;
    int                     m_LineMode;
    float                   m_LineThickness;
    int                     m_LineWidth;
    int                     m_LineHeight;
    char                    field_FC;
    Vector4f                m_Unknown_1;
    int                     field_110;
    int                     field_114;
    Node                   *m_HintNode;
    int                     m_HintCollisionVolume;
    char                    m_OptimisticMode;

public:
    CollisionProbe(int, float); // @8BA600

    const float             GetRadius() const;  //  @4ADB40
    void                    SetRadius(const float radius);  //  @571A00
    const float             GetAngle() const;   //  @91CE60
    void                    SetAngle(const float angle);    //  @571A20
    const int               GetDynamicMask() const; //  @8B5AA0
    void                    SetDynamicMask(const int mask); //  @522FC0
    const int               GetUserMask() const;    //  @8B5AB0
    void                    SetUserMask(const int mask);    //   @8B5AC0
    std::vector<Node*>&     GetNodes();   //  @8B5A60
    std::vector<Node*>&     GetTouchingNodes(); //  @8B5A70
    Node*                   GetClosestNode();   //  @4A0C70
    const int               GetClosestCollisionVolume() const;  //  @89AA70
    Node*                   GetRealNode();  //  @4A66A0
    void                    GetClosestNormal(Vector4f& outNormal) const;    //  @496C60
    void                    GetResolvedPos(Vector4f& outPos) const; //  @91CC90
    void                    GetContactPos(Vector4f& outPos) const;  //  @91CD80
    const float             GetMinDistance() const; //  @501090
    const int               GetSurfaceID() const;   //  @917860
    const int               GetMaterialID() const;  //  @4A66B0
    const int               GetCollisionFlags() const;  //  @8B5A80
    Node*                   GetHintNode();  //  @8B5AD0
    void                    SetHintNode(Node* node);    //  @4CF0B0
    const int               GetHintCollisionVolume() const; //  @8B5AE0
    void                    SetHintCollisionVolume(const int hintcoll); //  @571A40
    const bool              IsOptimisticMode() const;   //  @8B5AF0
    void                    SetOptimisticMode(const bool mode); //  @571A50

    void                    Update(int* args);  //  @8BDC60
    void                    Update_Impl(const bool updateForLine, const Vector4f& vec1, const Vector4f& vec2);  //  @8B62B0
    void                    UpdateForLine(int* args);   //  @8BDC80
    void                    IsLineColliding(int* args); //  @8BDB60
    bool                    IsLineColliding_Impl(const int, const Vector4f& vec1, const Vector4f& vec2);    //  @8B7000
    void                    sGetClosestNode(int* args);  //  @8BDBE0
    bool                    GetClosestNode_Impl(const int, const Vector4f& vec1, const Vector4f& vec2);  //  @8B8720
    void                    GetFieldOnFace(int* args);  //  @8BB200
    const int               GetFieldOnFace_Impl(const int face) const;  //  @8B5B40
    void                    SetLineMode(int* args); //  @571A60
    void                    SetLineThickness(int* args);    //   @4A0C80
    void                    SetLineWidthHeight(int* args);    //  @810A30
    void                    SetCollisionMask(int* args);    //  @4A0C90
    void                    IgnoreNode(int* args);  //  @8BDB40
    void                    IgnoreNode_Impl(Node* node);    //  @8BDAD0
    void                    RemoveIgnoreNode(int* args);    //  @8BD800
    void                    RemoveIgnoreNode_Impl(Node* node);  //  @891D20
    void                    ResetIgnoreList();  //  @8BC630
    void                    Reset(int* args);   //  @8BDCA0
    void                    Reset_Impl();   //  @8B61D0
    void                    ClearCache(int* args);  //  @8BDB50
    void                    ClearCache_Impl();  //  @8B6130
    void                    ClearCacheLine(int* args);  //  @883EC0

    static std::vector<CollisionProbe*> ProbesList; // @A3DD4C
    static unsigned int     CachedProbes; // @A3DE48

    static void             Register(); // @8BA830
    static CollisionProbe*  Create(); // @8BDEA0
};

extern EntityType*          tCollisionProbe; // @A3DE38

ASSERT_CLASS_SIZE(CollisionProbe, 292);