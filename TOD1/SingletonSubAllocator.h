#pragma once
#include "AllocatorBase.h"

class SingletonSubAllocator : public Allocator
{
protected:
	bool						m_Instantiated;
	void*						m_InstancePtr;

public:
	SingletonSubAllocator();	//	@47A690

	virtual void*				Allocate_A(size_t size, int filler, int unk) override;	//	@47A6B0
	virtual void*				AllocateAligned(size_t size, size_t alignment, int filler, int unk) override;	//	@47A6D0
	virtual void				Free(void* ptr) override;	//	@47A720
	virtual void				FreeAligned(void* ptr) override;	//	@47A710
	virtual void*				Realloc(void* oldptr, size_t newsize, int filler, int unk) override;	//	@47A730
	virtual int					stub8(int* unk) override;	//	@47A770
	virtual void				stub9() override;
	virtual const int			GetTotalAllocations() const override;	//	@47A790
	virtual const char* const	GetAllocatorName() const override;	//	@47A7F0
	virtual const int			stub19() const override;	//	@47A7A0
	virtual const int			stub20() const override;	//	@47A7B0
	virtual const int			stub21() const override;	//	@47A7C0
	virtual const int			GetAvailableMemory() const override;	//	@47A7D0
};

ASSERT_CLASS_SIZE(SingletonSubAllocator, 44);