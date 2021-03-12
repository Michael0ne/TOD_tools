#include "Model.h"

ScriptType_Entity* tModel;

#pragma message(TODO_IMPLEMENTATION)
Model::Model() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE | NODE_MASK_FRAGMENT)
{
	MESSAGE_CLASS_CREATED(Model);

	modelres = nullptr;
	field_54 = 1;
	field_58 = 0;
	field_5C = 1;

	//m_List_1 = List<int>(0x1A300);
	//m_List_2 = List<int>(0x1A300);
	//m_List_3 = List<int>(0x1FB00);
	//m_List_4 = List<int>(0x1FB00);
	//m_List_6 = List<int>(0x1A300);

	field_E0 = 0;
	field_E4 = 1;
	field_E8 = -1;
	m_ActiveTextureSet = 0;
	field_7C = field_80 = field_84 = field_88 = field_8C = field_90 = 0;
	field_60 = 0;
	m_Flags = m_Flags & 0x806440FF | 0x640000;
	field_70 = field_70 & 0xFF2FFFFF | 0xF200000;
	field_74 = 0;
}

#pragma message(TODO_IMPLEMENTATION)
Model::~Model()
{
	MESSAGE_CLASS_DESTROYED(Model);
}

#pragma message(TODO_IMPLEMENTATION)
void Model::Register()
{
	tModel = new ScriptType_Entity("Model");
	tModel->InheritFrom(tNode);
	tModel->SetCreator((ScriptType_Entity::CREATOR)Create);
}

Model* Model::Create(AllocatorIndex)
{
	return new Model();
}