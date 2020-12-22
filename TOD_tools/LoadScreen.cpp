#include "LoadScreen.h"
#include "Blocks.h"
#include "LoadScreenInfo.h"
#include "Progress.h"

LoadScreenNode* tLoadScreenNode = nullptr;	//	@A3D7E4

void LoadScreenNode::Activate_Impl()
{
	g_LoadScreenInfo->SetTexturePath(g_Blocks->GetResourcePath(m_ImageFile.m_szString));
	g_Progress->SetLoadScreenSprite(m_LoadBarSprite);
	g_LoadScreenInfo->Enable(m_RenderOnTopNode);
}

LoadScreenNode::LoadScreenNode() : Node(NODE_MASK_EMPTY)
{
	MESSAGE_CLASS_CREATED(LoadScreenNode);

	m_ImageFile;
	m_RenderOnTopNode = nullptr;
	m_LoadBarSprite = nullptr;
}

const char* LoadScreenNode::GetImageFile() const
{
	return m_ImageFile.m_szString;
}

void LoadScreenNode::SetImageFile(const char* _fname)
{
	if (_fname && *_fname && g_Blocks)
		g_Blocks->GetInternalFileName(m_ImageFile, _fname);
}

void LoadScreenNode::Activate()
{
	Activate_Impl();
}

void LoadScreenNode::Deactivate(int)
{
	g_LoadScreenInfo->Deactivate();
}

void LoadScreenNode::SetRenderOnTopNode(Node* _node)
{
	m_RenderOnTopNode = _node;
}

void LoadScreenNode::SetLoadbarSprite(Node* _sprite)
{
	m_LoadBarSprite = _sprite;
	g_Progress->SetLoadScreenSprite(_sprite);
}