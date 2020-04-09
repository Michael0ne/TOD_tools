#include "Renderer.h"

Renderer* g_Renderer = NULL;
bool& Renderer::WideScreen = *(bool*)0xA39F12;
bool Renderer::FSAA = 0;
float& Renderer::RatioXY = *(float*)0xA119F4;
int& Renderer::_A08704 = *(int*)0xA08704;
int& Renderer::_A0870C = *(int*)0xA0870C;
int& Renderer::_A0872C = *(int*)0xA0872C;

ScreenProperties& Renderer::g_ScreenProperties = *(ScreenProperties*)0xA08810;

void Renderer::CreateRenderer(void* resolution, int unk1, int unk2, int fsaa, int buffersCount, int unk4, void* buffers)
{
	void(__thiscall * _CreateRend)(Renderer * _this, void* _res, int _unk1, int _unk2, int _fsaa, int _buffc, int _unk4, void* _buff) = (void(__thiscall*)(Renderer*, void*, int, int, int, int, int, void*))0x421320;

	_CreateRend(this, resolution, unk1, unk2, fsaa, buffersCount, unk4, buffers);

#ifndef INCLUDE_FIXES
	//	For testing purposes - hide cursor.
	ShowCursor(0);
#endif
}

void Renderer::_41FDF0(Vector4<float>* size, int bufferIndex)
{
	(*(void(__thiscall*)(Renderer*, Vector4<float>*, int))0x41FDF0)(this, size, bufferIndex);
}

void ScreenProperties::SetHudScreenSize(float width, float height, float unk1, float unk2)
{
	field_8 = unk1;
	field_C = unk2;
	m_fVirtualHudScreensizeWidth = width;
	m_fVirtualHudScreensizeHeight = height;

	AdjustWindowScalings();
}

void ScreenProperties::AdjustWindowScalings()
{
	(*(void(__thiscall*)(ScreenProperties*))0x420190)(this);
}

void ScreenProperties::SetSafeArea(float area)
{
	m_bSafeArea = true;
	m_fScreenSafeArea = area;

	AdjustWindowScalings();
}