#pragma once
#include "RenderBuffer.h"
#include <vector>

//	TODO: what is this for?
class RenderBuffer92
{
protected:
	int					field_0;
	std::vector<int>	List_1;
	int					field_14;
	RenderBuffer		m_RenderBuffer[3];
	int					field_54;
	int					field_58;

public:
	RenderBuffer92(unsigned int a1, unsigned char a2, unsigned int a3);	//	@436B00
	~RenderBuffer92();	//	@4361E0
};

ASSERT_CLASS_SIZE(RenderBuffer92, 92);