#pragma once
#include "AllocatorBase.h"

class StackBasedSubAllocator : public Allocator
{
	struct StackElement
	{
		StackElement*			m_PreviousElement;
		StackElement*			m_NextElement;
		void*					m_ActualSpacePtr;
	};

protected:
	StackElement*				m_StackSpace;
	StackElement*				m_StackSpace_1;
	void*						m_StackEndPtr;
	int							m_ElementsInStack;
	int							field_34;

public:
	StackBasedSubAllocator();	//	@47A820

	virtual void*				Allocate_A(size_t size, int filler, int unk) override;	//	@47A9D0
	virtual void*				AllocateAligned(size_t size, size_t alignment, int filler, int unk) override;	//	@47A930
	virtual void				Free(void* ptr) override;	//	@47A9F0
	virtual void				FreeAligned(void* ptr) override;	//	@47AA10
	virtual void*				Realloc(void* oldptr, size_t newsize, int filler, int unk) override;	//	@47AA20
	virtual int					stub8(int* unk) override;	//	@47AAA0
	virtual void				stub9() override;	//	@47AAC0
	virtual void				SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size) override;	//	@47A890
	virtual const int			GetTotalAllocations() const override;	//	@47AAD0
	virtual const char* const	GetAllocatorName() const override;	//	@47AB20
	virtual const int			stub19() const override;	//	@47AAE0
	virtual const int			stub20() const override;	//	@47AAF0
	virtual const int			stub21() const override;	//	@47AB10
	virtual const int			GetAvailableMemory() const override;	//	@47AB00
};

ASSERT_CLASS_SIZE(StackBasedSubAllocator, 56);