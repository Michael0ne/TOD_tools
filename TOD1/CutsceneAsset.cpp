#include "CutsceneAsset.h"
#include "Window.h"

AssetInstance* CutsceneAsset::Instance;

CutsceneAsset::CutsceneAsset() : Asset(false)
{
	MESSAGE_CLASS_CREATED(CutsceneAsset);

	field_40 = 1;
	field_44 = 30;
	field_48 = 1;
}

CutsceneAsset::~CutsceneAsset()
{
	MESSAGE_CLASS_DESTROYED(CutsceneAsset);
}

AssetInstance* CutsceneAsset::GetInstancePtr() const
{
	return Instance;
}