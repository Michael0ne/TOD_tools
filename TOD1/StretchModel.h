#pragma once
#include "Model.h"

class StretchModel : public Model
{
protected:
    Vector4f        m_Scale;
public:
    StretchModel(); // @8F87A0

    void            GetScale(Vector4f& outScale) const;   //  @8A6D50
    void            SetScale(const Vector4f& scale);    //  @8F8760

    static void     Register(); //  @8F8810

private:
    static StretchModel* Create(AllocatorIndex);    //  @8F9100
};

extern EntityType* tStretchModel;   //  @A3E0D4

ASSERT_CLASS_SIZE(StretchModel, 272);