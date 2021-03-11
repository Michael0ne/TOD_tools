#include "ParticleSystem.h"
#include "ScriptDatabase.h"

bool ParticleSystem::LodAndFade;

ParticleSystem::ParticleSystem() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
{
	MESSAGE_CLASS_CREATED(ParticleSystem);

	m_Texture = nullptr;
	field_54 = 1;
	m_ModelRes = nullptr;
	field_5C = 1;

	//m_List_1 = List<int>(0x1FB00);
	//m_List_2 = List<int>(0x1FB00);

	field_60 = ParticleSystem_Struct96();

	field_E4 = 0;
	field_E8 = 1;
	field_EC = -1;
	field_C0 = 0;
	m_IndexInSceneList = -1;

	LodAndFade = Script::LodAndFade;

	//	FIXME: woah!
	Orientation orient;
	SetOrient((*(Orientation& (__thiscall*)(Orientation*, const Vector4f&, const Vector4f&))0x46BD70)(&orient, Vector4f(0.0f, 0.0f, 1.0f, 0.0f), Vector4f(0.0f, 1.0f, 0.0f, 0.0f)));

	field_60._4250C0();
}

#pragma message(TODO_IMPLEMENTATION)
ParticleSystem_Struct96::ParticleSystem_Struct96()
{
	(*(ParticleSystem_Struct96 * (__thiscall*)(ParticleSystem_Struct96*))0x425470)(this);
}

#pragma message(TODO_IMPLEMENTATION)
void ParticleSystem_Struct96::_4250C0()
{
	(*(void(__thiscall*)(ParticleSystem_Struct96*))0x4250C0)(this);
}