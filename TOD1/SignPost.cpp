#include "SignPost.h"

std::vector<SignPost*> SignPost::SignPostList;

SignPost::SignPost() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
{
	MESSAGE_CLASS_CREATED(SignPost);

	//m_Unknown_1 = List<int>(0x24300);
	//m_Unknown_2 = List<int>(0x24300);

	field_74 = 1;

	SignPostList.push_back(this);
}