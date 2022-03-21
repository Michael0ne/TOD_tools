#pragma once
#include "MemoryManager.h"
#include "Types.h"

class Node;

class AuxQuadTree
{
public:
    Node           *m_Owner;
    AuxQuadTree    *field_4;
    AuxQuadTree    *m_NextSibling;
    AuxQuadTree    *m_FirstSibling;
    short   field_10[4];
    int    field_18;
    int    m_UserType;
    int    field_20;
    int    field_24;
    int    field_28;
    int    field_2C;
    int    field_30;
    int            *m_ContactNode;
    int            *field_38;
    char   field_3C;
    char   field_3D;
    short   m_TraverseDistance;
    Vector4f  m_Bounds;
    char   field_4D;
    char   m_Lod;
    char   m_LodFade;
    char   m_ContactFilter;
    int    m_LodDistance;

public:
    AuxQuadTree(class Node* owner); // @89F430
    ~AuxQuadTree(); // @8A2470

    void   CalculateLodForAllChildren(); // @8A3820
    void   UpdateLodDistance();  // @8A24F0
    void   _8A3810(void* ptr); // @8A3810
    void   Refresh(); // @8A2EE0;
    void            SetLodLevel(const Vector4f& pos);   //  @8A0B80
    void        CopyOwnerBounds(); //  @89F4D0
    void        _8A36A0(const bool use);    //  @8A36A0
    void        _8A3320();  //  @8A3320
    const int   GetContactMaterialID(const int a1) const; //  @8A2BE0
    const int   GetContactSurfacePropFields(const int a1) const;    //  @8A2AF0
    const int   GetContactFlags(const int index) const; //  @8A2A80
    Node*       GetRealNode(const int index) const; //  @8A0F40
    Node*       GetContactNode(const int index) const;  //  @8A2A60
    Vector4f*   GetPeerContactPoint(const int index) const; //  @8A2A10
    Vector4f*   GetContactPoint(const int index) const; //  @8A29C0
    Vector4f*   GetContactNormal(Vector4f& outNormal, const int index) const;   //  @8A2840
    int         Contacts() const;   //  @8A0CA0

    static AuxQuadTree*    GetForNode(Node* node);    //  @8A0810
};

ASSERT_CLASS_SIZE(AuxQuadTree, 88);