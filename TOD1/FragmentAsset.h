#pragma once
#include "BaseAsset.h"

class FragmentAsset : public Asset
{
    typedef AllocatorIndex (*AllocatorIndexForSize)(size_t size, FragmentAsset* assetPtr);

    struct FragmentInfo
    {
        uint32_t    field_0;
        uint32_t   *field_4;
        uint32_t    field_8;
        uint32_t    field_C;

        AllocatorIndexForSize   field_10;
        FragmentAsset          *field_14;

        FragmentInfo(FragmentInfo** rhs, AllocatorIndex (*FindSuitableAllocatorProc)(size_t desiredsize, FragmentAsset*), FragmentAsset* owner); //  @406750
        inline FragmentInfo()
        {
            MESSAGE_CLASS_CREATED(FragmentInfo);

            field_0 = NULL;
            field_C = 10240;
            field_10 = NULL;
            field_14 = NULL;

            _406490();

        }
        ~FragmentInfo(); //  @85E0A0

        uint32_t*   _406450();  //  @406450
        void        _406490();  //  @406490
        void        _4069F0();  //  @4069F0
        uint32_t*   _406320(uint32_t* data);    //  @406320
    };

public:
    int                     field_1C;
    FragmentInfo           *field_20;
    int                     field_24;

private:
    FragmentAsset(char a1); // @85DD80
    virtual ~FragmentAsset();   //  @85E3C0 //  NOTE: another d-tor @85DD60, virtual destructor uses inlined version.

    static AllocatorIndex   GetAllocatorForAsset(size_t size, FragmentAsset* asset);    //  @85D6D0

public:
    void* operator new(size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate_A(size, nullptr, NULL);
    }
    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, false);
        ptr = nullptr;
    }

    virtual AssetInstance*  GetInstancePtr() const override;
    virtual void            ApplyAssetData(CompiledAssetInfo* assetInfoPtr) override;  //  @85DEC0

    static void             CreateInstance(); // @85DFA0
    static FragmentAsset*   Create(); // @85DE30

    void                    ApplyFragmentResource(const int32_t entityId, bool); // @85D990

    static AssetInstance* Instance; // @A3BE58
};

ASSERT_CLASS_SIZE(FragmentAsset, 40);