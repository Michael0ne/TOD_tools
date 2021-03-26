#pragma once

#include "Node.h"

#define LOAD_SCREEN_NODE_CLASS_SIZE 104

class LoadScreenNode : public Node
{
protected:
	String		m_ImageFile;
	Node*		m_RenderOnTopNode;
	class Sprite* m_LoadBarSprite;

	void		Activate_Impl();	//	@87C790

public:
	LoadScreenNode();	//	@87CAE0

	const char* GetImageFile() const;	//	@8A9B10
	void		SetImageFile(const char*);	//	@87C800

	void		Activate();	//	@87CAD0
	void		Deactivate(int);	//	@87C9D0

	void		SetRenderOnTopNode(Node* _node);	//	@87C6E0
	void		SetLoadbarSprite(class Sprite* _sprite);	//	@87C9E0
};

extern EntityType* tLoadScreenNode;	//	@A3D7E4

static_assert(sizeof(LoadScreenNode) == LOAD_SCREEN_NODE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(LoadScreenNode));