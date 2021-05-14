#pragma once
#include "StringsPool.h"
#include "TextureAsset.h"
#include "Node.h"

#define LOADSCREEN_DEFAULT_TEXTURE "/data/Textures/Hud/LoadScreens/default_loadscreen.bmp"

class LoadScreenInfo
{
public:
    TextureAsset       *m_TextureResource;
    String              m_TexturePath;
    bool                m_Enabled;
    int                 m_StartTime;

    void			    LoadTexture();	//	@87C2F0

public:
    LoadScreenInfo(const char* texturePath);	//	@87C230

    void                SetTexturePath(const char*);	//	@87C2A0
    void                Enable(Node* topNode);	//	@87C6A0
    void                Show(Node* topNode);	//	@87C430
    void                Deactivate();	//	@87C1F0

    static unsigned int AllocatorIdForTextureResourceLoading;	//	@A3BE18
    static bool         IsLoadingLoadScreenTexture;	//	@A3BE2D
};

extern LoadScreenInfo* g_LoadScreenInfo;