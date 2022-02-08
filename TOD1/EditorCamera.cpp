#include "EditorCamera.h"
#include "FrameBuffer.h"
#include "BuiltinType.h"
#include "NumberType.h"

EntityType* tEditorCamera;

void EditorCamera::AddToFov(const float val)
{
    m_Fov += val;
}

EditorCamera::EditorCamera()
{
    MESSAGE_CLASS_CREATED(EditorCamera);

    m_Orient_1 = BuiltinType::Orient;
    m_Orient_2 = BuiltinType::Orient;

    field_D4 = 0;
    field_D5 = 0;
    field_D8 = 10.f;
    field_DC = 50.f;
    m_HasParentNode = false;

    SetParam(10, &m_Offset, tNUMBER);
    m_Offset = 50.f;

    m_Order = 1000;
    if (m_Parent)
    {
        SetChildrenPositionToSame();
        SetParent(m_Parent);
    }

    field_108 = 0;
    m_FrameBuffer = nullptr;
    m_GamepadIndex = -1;
}

EditorCamera::~EditorCamera()
{
    MESSAGE_CLASS_DESTROYED(EditorCamera);

    delete m_FrameBuffer;
}

EditorCamera* EditorCamera::Create(AllocatorIndex)
{
    return new EditorCamera;
}

void EditorCamera::Register()
{
    tEditorCamera = new EntityType("EditorCamera");
    tEditorCamera->InheritFrom(tCamera);
    tEditorCamera->SetCreator((CREATOR)Create);

    tEditorCamera->PropagateProperties();
}

#pragma message(TODO_IMPLEMENTATION)
void EditorCamera::Update()
{
}