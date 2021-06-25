#pragma once
#include "BaseAsset.h"
#include "Font.h"

class FontAsset : public Asset
{
protected:
    int             field_1C;
    int            *field_20;
    Font		   *m_Font;

private:
    inline FontAsset() : Asset(false)	//	NOTE: always inlined.
    {
        MESSAGE_CLASS_CREATED(FontAsset);

        field_20 = nullptr;
        m_Font = nullptr;
        
        SetReferenceCount(1);
    }

public:
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

    virtual         ~FontAsset();   //  @85B730
    virtual AssetInstance* GetInstancePtr() const override;
    virtual void    ApplyAssetData(int*) override;  //  @85B560

    static void     CreateInstance();	//	@85B460
    static FontAsset* Create();	//	@85B350

    static AssetInstance*   Instance;   //  @A3BE48
};

ASSERT_CLASS_SIZE(FontAsset, 40);