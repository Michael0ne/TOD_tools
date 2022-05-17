#pragma once
#include "BaseAsset.h"

//  NOTE: original name in EXE is 'Cutscene'.
class CutsceneAsset : public Asset
{
    struct AnimationResourceInfo
    {
        enum
        {
            CAMERA = 1,
            CHARACTER_BIPED = 2,
            MODEL = 3,

        }                               m_AnimationType;
        int                             field_4;
        char                           *m_AnimationFilename;
        int                             field_C;
        int                             field_10;
    };

    struct AssetInfo
    {
        bool                            m_Used;
        Asset                          *m_Asset;
    };

protected:
    int                                 field_1C;
    std::vector<AssetInfo>              m_AnimationResources;
    std::vector<AnimationResourceInfo>  m_AnimationResourcesInfo;
    int                                 field_40;
    int                                 field_44;
    int                                 field_48;
    String                              m_CutscenePath;
    int                                 field_5C;

private:
    CutsceneAsset(); // @916080

    static AssetInstance* Instance; //  @A3E12C

public:
    virtual ~CutsceneAsset();   //  @916DB0
    virtual AssetInstance*              GetInstancePtr() const override;    //  @916BF0

    static void                         CreateInstance(); // @9164C0
    static CutsceneAsset*               Create(); // @916100
};

ASSERT_CLASS_SIZE(CutsceneAsset, 96);