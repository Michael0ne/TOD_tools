#include "EditorCamera.h"
#include "NumberType.h"

EditorCamera* tEditorCamera = nullptr;

#pragma message(TODO_IMPLEMENTATION)
EditorCamera::EditorCamera()
{
 MESSAGE_CLASS_CREATED(EditorCamera);

 m_Orient_1 = Orientation(0.0f, 0.0f, 0.0f, 1.0f);
 m_Orient_2 = Orientation(0.0f, 0.0f, 0.0f, 1.0f);
 m_fD8 = 10.0f;
 m_fDC = 50.0f;

 field_D4 = field_D5 = field_E0 = NULL;
 SetParam(10, &m_Offset, tNUMBER);
 m_Offset = 50.0f;
 m_Order = 1000;

 if (m_Parent)
 {
#ifdef _EXE
  MESSAGE_NOT_IMPLEMENTED("Node::SetChildrenPositionToSame");
  MESSAGE_NOT_IMPLEMENTED("Node::SetParent_Impl");
#else
  (*(void(__thiscall*)(Node*))0x88D3B0)(this); // Node::SetChildrenPositionToSame() // TODO: implementation!
  (*(void(__thiscall*)(Node*, Node*))0x88E8A0)(this, m_Parent); // Node::SetParent_Impl() // TODO: implementation!
#endif
 }

 field_108 = field_134 = 0;
 field_130 = -1;
}