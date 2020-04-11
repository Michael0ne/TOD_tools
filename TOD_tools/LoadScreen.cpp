#include "LoadScreen.h"

CurrentLoadScreen* g_CurrentLoadScreen = NULL;

void CurrentLoadScreen::Init(const char* szTexturePath)
{
	patch(0xA3D7E0, this, 4);

	if (szTexturePath)
		m_sTexturePath.Set(szTexturePath);

	m_bEnabled = false;
}