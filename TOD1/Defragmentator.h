#pragma once
#include "BestFitAllocator.h"

class DefragmentatorBase
{
	struct Defragmentator_Space
	{
		unsigned int	m_ChunkId;
		int				field_4;
		int				field_8;
	};
protected:
	BestFitAllocator*	m_DefragmentAllocator;
	BestFitAllocator*	m_DefragmentAllocator_1;
	unsigned int		m_Size;

	Defragmentator_Space* m_AllocatedSpace;
	int					field_14;
	int					field_18;
	int					field_1C;
	char				field_20;
	int					m_LastDefragmentationTime;
	int					field_28;
	char				field_2C;
	int					field_30;

private:
	void				_47BC70(int chunkind);	//	@47BC70
	void				_47BCD0(int chunkind);	//	@47BCD0
	void				_47BD40(int chunkind);	//	@47BD40

public:
	DefragmentatorBase(BestFitAllocator* bestfitallocator, char a2, int size);	//	@47BBD0
	virtual ~DefragmentatorBase();	//	@47BDD0

	virtual int			stub2(int size, int a2, int a3, int a4, int alignment, char a6);	//	@47BE60
	virtual int			stub3(int chunkind, int a2, const char* const a3, int a4, unsigned int size);	//	@47BF20
	virtual char		stub4(int a1, int a2);	//	@47BD20
	virtual void		stub5(int a1);	//	@47BDB0
	virtual int			stub6(int a1);	//	@4783F0
	virtual void		stub7();
	virtual void		stub8();
};

ASSERT_CLASS_SIZE(DefragmentatorBase, 52);

class Defragmentator : public DefragmentatorBase
{
public:
	inline Defragmentator(BestFitAllocator* bestfitallocator, char unk1, int size) : DefragmentatorBase(bestfitallocator, 1, 10000)	//	NOTE: inlined @478630
	{
		field_28 = NULL;
		field_2C = NULL;
	}

	virtual void		stub8() override;	//	@47AE70
};