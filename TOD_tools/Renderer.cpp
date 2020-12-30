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
Scene_Buffer2& Renderer::Buffer_A08704 = *(Scene_Buffer2*)0xA08704;
ScreenProperties& Renderer::g_ScreenProperties = *(ScreenProperties*)0xA08810;

#pragma message(TODO_IMPLEMENTATION)
Renderer::Renderer(const Vector2<int>* resolution, unsigned int unused1, unsigned int unused2, unsigned int FSAA, unsigned int buffersCount, unsigned int unk1,const Vector3<float>* buffersDimens)
{
	MESSAGE_CLASS_CREATED(Renderer);

	m_TexturesList = List<GfxInternal_Dx9_Texture>();
	m_fTimeDelta = 0.0f;
	field_30 = 0;
	m_nTimeMilliseconds = Performance::GetMilliseconds();
	field_34 = 0;

	g_RendererDx = new GfxInternal_Dx9(resolution, unused1, unused2, FSAA, NULL);

	field_20 = unk1;
	m_BuffersCount = buffersCount;

	m_pBuffersArray = new Scene_Buffer276[buffersCount];

	if (buffersCount > 0) {
		int index = 0;

		do {
			m_pBuffersArray[index].Init(*(buffersDimens++));
			m_pBuffersArray[index].SetResolution(Vector2<float>{g_RendererDx->m_nViewportWidth, g_RendererDx->m_nViewportHeight});

			++index;
			--buffersCount;
		} while (buffersCount);
	}

	m_RenderBufferEmpty = false;

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

Renderer::~Renderer()
{
	MESSAGE_CLASS_DESTROYED(Renderer);

	if (m_BuffersCount > 0)
		while (m_BuffersCount) {
			if (&m_pBuffersArray[m_BuffersCount])
				delete &m_pBuffersArray[m_BuffersCount];
			m_BuffersCount--;
		}
}

void Renderer::_41FDF0(Vector4<float>* size, int bufferIndex)
{
	if (bufferIndex == -1)
		if (m_BuffersCount > 0) {
			unsigned char ind_ = 0;
			while (++ind_ < m_BuffersCount)
				m_pBuffersArray[ind_].m_vPos_1 = *size;
		}
	else
		m_pBuffersArray[bufferIndex].m_vPos_1 = *size;
}

#pragma message(TODO_IMPLEMENTATION)
void Renderer::_SetBufferStateByIndex(int state, int index)
{
	(*(void(__thiscall*)(Renderer*, int, int))0x41FD90)(this, state, index);
}

void Renderer::SetRenderBufferIsEmpty(bool _empty)
{
	m_RenderBufferEmpty = _empty;
}

void Scene_Buffer276::Init(const Vector3<float>& vDimensions)
{
	m_vRes_1 = vDimensions;

	field_10 = 0;
	field_14 = 1;
	field_C = 0;
	field_DC = 0;
	field_100 = 0;

	m_vDimens_1 = { 70.0f, 1.0f, 1.0f, 1000.0f };

	m_vRightVec_1 = Builtin::RightVector;
	m_vUpVec_1 = Builtin::UpVector;
	m_vInVec_1 = Builtin::InVector;
	m_vOrient_1 = Builtin::Orientation;

	m_vRightVec_2 = Builtin::RightVector;
	m_vUpVec_2 = Builtin::UpVector;
	m_vInVec_2 = Builtin::InVector;
	m_vOrient_2 = Builtin::Orientation;

	m_vRightVec_3 = Builtin::RightVector;
	m_vUpVec_3 = Builtin::UpVector;
	m_vInVec_3 = Builtin::InVector;
	m_vOrient_3 = Builtin::Orientation;

	m_vPos_1 = Builtin::UnkColor;
	memset(&m_vRes_2, 0, sizeof(m_vRes_2));
}

void ScreenProperties::SetHudScreenSize(float width, float height, float unk1, float unk2)
{
	field_8 = unk1;
	field_C = unk2;
	m_fVirtualHudScreensizeWidth = width;
	m_fVirtualHudScreensizeHeight = height;

	AdjustWindowScalings();
}

#pragma message(TODO_IMPLEMENTATION)
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

void ScreenProperties::SetWindowProperties(float width, float height, float ratio, float safearea)
{
	m_fScreenWidth = width;
	m_fScreenHeight = height;
	m_fScreenRatio = ratio;
	
	if (!m_bSafeArea)
		m_fScreenSafeArea = safearea;

	AdjustWindowScalings();
}

#pragma message(TODO_IMPLEMENTATION)
void Scene_Buffer108::Init(unsigned int unk1, unsigned char unk2, unsigned int unk3)
{

}

#pragma message(TODO_IMPLEMENTATION)
void Scene_Buffer68::Init(const Scene_Buffer108& buf, unsigned int unk)
{

}