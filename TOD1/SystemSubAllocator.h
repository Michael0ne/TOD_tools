#pragma once
#include "AllocatorBase.h"

class SystemSubAllocator : public Allocator
{
protected:
	unsigned int				m_AllocationsTotal;

public:
	SystemSubAllocator();	//	@47AB90

	virtual void*				Allocate_A(size_t size, int filler, int) override;	//	@47AC00
	virtual void*				AllocateAligned(size_t size, size_t alignment, int filler, int) override;	//	@47AC30
	virtual void				Free(void* ptr) override;	//	@47AC90
	virtual void				FreeAligned(void* ptr) override;	//	@47AC70
	virtual void*				Realloc(void* oldptr, size_t newsize, int filler, int unk) override;	//	@47ACB0
	virtual int					stub8(int* unk) override;	//	@47AD60
	virtual void				stub9() override;	//	@47ADE0
	virtual void				SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size) override;	//	@47ADF0
	virtual const int			GetAllocatedSpaceSize() const override;	//	@47ADA0
	virtual const int			GetTotalAllocations() const override;	//	@47AD30
	virtual const char* const	GetAllocatorName() const override;	//	@47AE10
	virtual const int			stub19() const override;	//	@47AD70
	virtual const int			stub20() const override;	//	@47AD70
	virtual const int			stub21() const override;	//	@47AD90
	virtual const int			GetAvailableMemory() const override;	//	@47ADC0
	virtual int					stub35() override;	//	@47AE00
};

ASSERT_CLASS_SIZE(SystemSubAllocator, 40);