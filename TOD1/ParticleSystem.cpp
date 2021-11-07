#include "ParticleSystem.h"
#include "ScriptDatabase.h"
#include "BuiltinType.h"

bool ParticleSystem::LodAndFade;
Vector4f ParticleSystem::CameraOrigin = BuiltinType::ZeroVector;

ParticleSystem::ParticleSystem() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
{
    MESSAGE_CLASS_CREATED(ParticleSystem);

    m_Texture = nullptr;
    field_54 = 1;
    m_ModelRes = nullptr;
    field_5C = 1;

    //m_List_1 = List<int>(0x1FB00);
    //m_List_2 = List<int>(0x1FB00);

    field_E4 = 0;
    field_E8 = 1;
    field_EC = -1;
    field_C0 = 0;
    m_IndexInSceneList = -1;

    LodAndFade = Script::LodAndFade;

    // FIXME: woah!
    Orientation orient;
    SetOrient((*(Orientation & (__thiscall*)(Orientation*, const Vector4f&, const Vector4f&))0x46BD70)(&orient, Vector4f(0.0f, 0.0f, 1.0f, 0.0f), Vector4f(0.0f, 1.0f, 0.0f, 0.0f)));

    m_Properties._4250C0();
}

#pragma message(TODO_IMPLEMENTATION)
Properties::Properties()
{
    (*(Properties * (__thiscall*)(Properties*))0x425470)(this);
}

#pragma message(TODO_IMPLEMENTATION)
void Properties::_4250C0()
{
    (*(void(__thiscall*)(Properties*))0x4250C0)(this);
}