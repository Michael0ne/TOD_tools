#include "StretchModel.h"
#include "VectorType.h"

EntityType* tStretchModel;

StretchModel::StretchModel() : Model()
{
    MESSAGE_CLASS_CREATED(StretchModel);

    m_Scale = { 1, 1, 1, 0 };
    m_FrameBuffer[0] = nullptr;
}

void StretchModel::GetScale(Vector4f& outScale) const
{
    outScale = m_Scale;
}

void StretchModel::SetScale(const Vector4f& scale)
{
    m_Scale = scale;

    if (m_QuadTree)
        m_QuadTree->Refresh();

    m_Id._3 = 5;
}

void StretchModel::Register()
{
    tStretchModel = new EntityType("StretchModel");
    tStretchModel->InheritFrom(tModel);
    tStretchModel->SetCreator((CREATOR)Create);

    tStretchModel->RegisterProperty(tVECTOR, "scale", (EntityGetterFunction)&GetScale, (EntitySetterFunction)&SetScale, "control=string");

    tStretchModel->PropagateProperties();
}

StretchModel* StretchModel::Create(AllocatorIndex)
{
    return new StretchModel;
}