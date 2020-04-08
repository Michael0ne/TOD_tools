#include "Renderer.h"

Renderer* g_Renderer = NULL;
bool& Renderer::WideScreen = *(bool*)0xA39F12;
bool Renderer::FSAA = 0;
float& Renderer::RatioXY = *(float*)0xA119F4;

void Renderer::CreateRenderer(void* resolution, int unk1, int unk2, int fsaa, int buffersCount, int unk4, void* buffers)
{
	void(__thiscall * _CreateRend)(Renderer * _this, void* _res, int _unk1, int _unk2, int _fsaa, int _buffc, int _unk4, void* _buff) = (void(__thiscall*)(Renderer*, void*, int, int, int, int, int, void*))0x421320;

	_CreateRend(this, resolution, unk1, unk2, fsaa, buffersCount, unk4, buffers);
}