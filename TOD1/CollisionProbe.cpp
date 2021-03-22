#include "CollisionProbe.h"
#include "ScriptTypes.h"

ScriptType_Entity* tCollisionProbe;
std::vector<CollisionProbe*> CollisionProbe::ProbesList;
unsigned int CollisionProbe::CachedProbes;

CollisionProbe::CollisionProbe(int, float) : Node(NODE_MASK_POSITION)
{
	MESSAGE_CLASS_CREATED(CollisionProbe);

	m_TouchingNodes = (Node*)MemoryManager::AllocatorsList[DEFRAGMENTING]->m_Defragmentator;
	field_AC = 0x2007C00;
	m_Nodes = (ScriptType_Entity*)MemoryManager::AllocatorsList[DEFRAGMENTING]->m_Defragmentator;
	field_D0 = nullptr;

	Reset();

	field_FC = true;

	ProbesList.push_back(this);
	field_D0 = nullptr;

	Reset();
}

#pragma message(TODO_IMPLEMENTATION)
void CollisionProbe::Reset()
{
	m_UserMask = -1;
	m_CollisionMask = -1;
	m_DynamicMask = NULL;
	m_Angle = -1.f;
	m_LineMode = NULL;
	m_LineThickness = 0.f;
	m_LineWidth = NULL;
	m_LineHeight = NULL;
	m_ClosestNode = nullptr;
	m_ClosestCollisionVolume = NULL;
	m_RealNode = nullptr;
	m_IgnoredNodes.clear();
	m_SurfaceID = -1;
	m_MaterialID = -1;
	m_HintNode = nullptr;
	m_OptimisticMode = NULL;
	field_AC = NULL;

//	_A3DE18->8B3590();
//	_A3DE18->8B4570(ScriptType_Builtin::ZeroVector);

	ClearCache();
}

#pragma message(TODO_IMPLEMENTATION)
void CollisionProbe::ClearCache()
{
}

#pragma message(TODO_IMPLEMENTATION)
void CollisionProbe::Register()
{
	tCollisionProbe = new ScriptType_Entity("CollisionProbe");
	tCollisionProbe->InheritFrom(tNode);
	tCollisionProbe->SetCreator((ScriptType_Entity::CREATOR)Create);
}

CollisionProbe* CollisionProbe::Create()
{
	return new CollisionProbe();
}