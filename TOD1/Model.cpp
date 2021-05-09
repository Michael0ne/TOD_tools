#include "Model.h"

EntityType* tModel;

#pragma message(TODO_IMPLEMENTATION)
Model::Model() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE | NODE_MASK_FRAGMENT)
{
	MESSAGE_CLASS_CREATED(Model);
}

#pragma message(TODO_IMPLEMENTATION)
Model::~Model()
{
	MESSAGE_CLASS_DESTROYED(Model);
}

#pragma message(TODO_IMPLEMENTATION)
void Model::Register()
{
	tModel = new EntityType("Model");
	tModel->InheritFrom(tNode);
	tModel->SetCreator((EntityType::CREATOR)Create);
}

void Model::_884530()
{
	for (auto it = m_AttachedEffects.cbegin(); it != m_AttachedEffects.cend(); it++)
	{
		if (it->m_Node && it->m_Node->m_ScriptEntity == tNode && strcmp(it->m_Node->GetScript(), "") == NULL && it->m_Node->m_Flags.m_Flags != NULL)
		{
			it->m_Node->SetFlags(it->m_Node->m_Flags.m_Flags & 0xFFE);	//	NOTE: remove 'Disabled' flag.
			it->m_Node->SetFlags(it->m_Node->m_Flags.m_Flags & 0xF7F | 0x80);	//	NOTE: set flag 'DER'.
		}
	}
}

Model* Model::Create(AllocatorIndex)
{
	return new Model();
}