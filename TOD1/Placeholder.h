#pragma once
#include "Model.h"

#define PLACEHOLDER_CLASS_SIZE 268

class PlaceHolder : public Model
{
protected:
    union
    {
        struct
        {
            unsigned        ModelBound : 1;
            unsigned        CreateCollisionNode : 1;
            signed          CollisionPivot : 16;
        };
    }       m_Flags;
    float m_BoundRadius;
    Node* m_PlaceholderCollisionPivot;

public:
    PlaceHolder(); // @8D17A0

    const float     GetBoundRadius() const; //  @8C9200
    void            SetBoundRadius(const float radius); //  @5407D0
    const bool      IsUsingModelBound() const;    //  @8D1790
    void            UseModelBound(const bool use);  //  @67B8A0
    const bool      ShouldCreateCollisionNode() const;  //  @7C5310
    void            CreateCollisionNode(const bool create); //  @815170
    const int       GetCollisionPivot() const;  //  @891180
    void            SetCollisionPivot(const short pivotid);   //  @67FC40

    static void     Register(); //  @8D1890

private:
    void            RemoveSiblings();   //  @8D1810
    void            SetCollisionPivotFromModelRes();    //  @67FB80

    static PlaceHolder*     Create(AllocatorIndex); //  @8D1C30

    static int              GetPlaceholderModelCommand; //  @A3DFE4
};

extern EntityType* tPlaceholder;   //  @A3DFE0

ASSERT_CLASS_SIZE(PlaceHolder, 268);