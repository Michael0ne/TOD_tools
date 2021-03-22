#pragma once
#include "StringsPool.h"
#include <vector>

struct GlyphInfo
{
	unsigned int	field_0;
	unsigned int	field_4;
	char*			field_8;	//	NOTE: size of this is same as 'field_0'.
	void*			field_C;
};

class Font
{
private:
	int				field_0;
	int				field_4;
	int				field_8;
	int				field_C;
	int				field_10;
	int				field_14;
	float			field_18;
	float			field_1C;
	float			field_20;
	float			field_24;
	float			field_28;
	float			field_2C;
	int				field_30;
	float			field_34;
	int				field_38;
	int				field_3C;
	std::vector<String>	m_GlyphsList;
	int				field_50;
	int				field_54;
	float			field_58;
	float			field_5C;
	float			field_60;

public:
	Font(const GlyphInfo& GlyphsInfo);	//	@41EA80

	void* operator new (size_t size)
	{
		return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			MemoryManager::ReleaseMemory(ptr, false);
		ptr = nullptr;
	}

	void			CreateBakedFont(const GlyphInfo& GlyphsInfo);	//	@41E6D0

	static void		MakeCharactersMap(GlyphInfo& GlyphsInfo);	//	@938E10

	static const float	_9B38D4;	//	@9B38D4
	static const float	_9B38D8;	//	@9B38D8
	static GlyphInfo GlyphsInfo;	//	@A1B698
};

extern Font* g_Font;

ASSERT_CLASS_SIZE(Font, 100);