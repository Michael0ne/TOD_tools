#include "Renderer.h"
#include "Performance.h"
#include "Builtin.h"

Renderer* g_Renderer = NULL;
bool& Renderer::WideScreen = *(bool*)0xA39F12;
bool Renderer::FSAA = 0;
float& Renderer::RatioXY = *(float*)0xA119F4;
int& Renderer::_A08704 = *(int*)0xA08704;
int& Renderer::_A0870C = *(int*)0xA0870C;
int& Renderer::_A0872C = *(int*)0xA0872C;

ScreenProperties& Renderer::g_ScreenProperties = *(ScreenProperties*)0xA08810;

void Renderer::CreateRenderer(void* resolution, int unk1, int unk2, int fsaa, int buffersCount, int unk4, Vector3<float>* buffers)
{
	patch(0xA35E04, this, 4);

	//void(__thiscall * _CreateRend)(Renderer * _this, void* _res, int _unk1, int _unk2, int _fsaa, int _buffc, int _unk4, void* _buff) = (void(__thiscall*)(Renderer*, void*, int, int, int, int, int, void*))0x421320;

	//_CreateRend(this, resolution, unk1, unk2, fsaa, buffersCount, unk4, buffers);

	m_TexturesList = List<GfxInternal_Dx9_Texture>();
	m_fTimeDelta = 0.0f;
	field_30 = 0;
	m_nTimeMilliseconds = Performance::GetMilliseconds();
	field_34 = 0;

	if (!Allocators::Released)
		if (g_RendererDx = new GfxInternal_Dx9())
			g_RendererDx->Init(resolution, unk1, unk2, fsaa, 0);

	field_20 = unk4;
	m_nUnkBuffersCount = buffersCount;

	if (!Allocators::Released)
		m_pUnkBuffersArray = new Scene_Buffer276[buffersCount];

	if (buffersCount > 0) {
		Vector3<float>* buffers_ = buffers;
		int index = 0;

		do {
			m_pUnkBuffersArray[index].Init(*buffers_);

			m_pUnkBuffersArray[index].SetResolution(Vector2<float>{g_RendererDx->m_nViewportWidth, g_RendererDx->m_nViewportHeight});

			buffers_++;
			index++;
			--buffersCount;
		} while (buffersCount);
	}

	field_0[0] = 0;
	field_0[1] = 0;
	field_0[2] = 0;
	field_0[3] = 0;

	//	TODO: implementation!
	//	Allocate textures list.
	(*(void(__thiscall*)(Renderer*))0x4210E0)(this);
	//	Allocate something
	(*(void(__thiscall*)(Renderer*))0x420390)(this);

	field_34 = 1;
	m_nUnkTime_1 = 0;
	m_nUnkTime_2 = __rdtsc();

#ifndef INCLUDE_FIXES
	//	For testing purposes - hide cursor.
	ShowCursor(0);
#endif
}

void Renderer::_41FDF0(Vector4<float>* size, int bufferIndex)
{
	(*(void(__thiscall*)(Renderer*, Vector4<float>*, int))0x41FDF0)(this, size, bufferIndex);
}

void Scene_Buffer276::Init(const Vector3<float>& vDimensions)
{
	m_vUnkDimensionsVec_1 = vDimensions;
	field_14 = 1;
	field_10 = 0;
	field_C = 0;

	//	Init matricies (MVP?)
	m_vRightVec = Builtin::m_RightVector;
	m_vUpVec = Builtin::m_UpVector;
	m_vInVec = Builtin::m_InVector;
	m_vUnkVec_1 = Builtin::m_Orientation;
	m_vRightVec_1 = Builtin::m_RightVector;
	m_vUpVec_1 = Builtin::m_UpVector;
	m_vInVec_1 = Builtin::m_InVector;
	m_vUnkVec_2 = Builtin::m_Orientation;
	m_vRightVec_2 = Builtin::m_RightVector;
	m_vUpVec_2 = Builtin::m_UpVector;
	m_vInVec_2 = Builtin::m_InVector;
	m_vUnkVec_3 = Builtin::m_Orientation;

	m_vUnkVec_4 = { 70.0f, 1.0f, 1.0f, 1000.0f };

	field_DC = 0;
	field_100 = 0;

	m_unkColor = Builtin::m_vUnkColor;

	m_vUnkVec_5 = { 0.0f, 0.0f };
}

void Scene_Buffer276::SetResolution(const Vector2<float>& vResolution)
{
	m_vResolution = vResolution;
}

void ScreenProperties::SetHudScreenSize(float width, float height, float unk1, float unk2)
{
	field_8 = unk1;
	field_C = unk2;
	m_fVirtualHudScreensizeWidth = width;
	m_fVirtualHudScreensizeHeight = height;

	AdjustWindowScalings();
}

//	TODO: implementation!
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