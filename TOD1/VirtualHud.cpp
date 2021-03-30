#include "VirtualHud.h"
#include "GfxInternal_Dx9.h"

VirtualHud VirtualHud::VirtualHudInstance;

void VirtualHud::SetVirtualHudScreenSize(const float width, const float height, const float a3, const float a4)
{
	field_8 = a3;
	field_C = a4;
	m_VirtualHudScreensizeWidth = width;
	m_VirtualHudScreensizeHeight = height;

	CalculateDimensions();
}

void VirtualHud::CalculateDimensions()
{
	float ratiowidth = m_ScreenWidth / m_VirtualHudScreensizeWidth;
	float ratioheight = (m_ScreenHeight * m_ScreenRatio) / (field_8 * m_VirtualHudScreensizeHeight);

	m_HudSizeRatio = ratioheight <= ratiowidth ? 1.f : ratioheight / ratiowidth;
	field_40 = 1.f;
	m_VirtualHudScreensizeHalfWidth = m_VirtualHudScreensizeWidth * 0.5f;
	m_VirtualHudScreensizeHalfHeight = m_VirtualHudScreensizeHeight * 0.5f;
	field_2C = (m_ScreenSafeArea / field_C) * (ratioheight <= ratiowidth ? ratioheight : ratiowidth);
	field_30 = (field_8 / m_ScreenRatio) * ( (m_ScreenSafeArea / field_C) * (ratioheight <= ratiowidth ? ratioheight : ratiowidth) );
	m_ScreenWidthHalf = m_ScreenWidth * 0.5f;
	m_ScreenHeightHalf = m_ScreenHeight * 0.5f;
}

Vector2f& VirtualHud::_420280(Vector2f& outvec, const float a2, const float a3) const
{
	outvec = { m_ScreenWidthHalf + (field_2C * (a2 - m_VirtualHudScreensizeHalfWidth)), m_ScreenHeightHalf + (field_30 * (a3 - m_VirtualHudScreensizeHalfHeight)) };
	return outvec;
}

Vector2f& VirtualHud::_4202D0(Vector2f& outvec, const float a2, const float a3) const
{
	outvec = { field_2C * a2, field_30 * a3 };
	return outvec;
}

float VirtualHud::GetScreenTopInVirtualUnits() const
{
	return ((1.f - m_ScreenSafeArea) * m_ScreenHeight * 0.5f - m_ScreenHeightHalf) / field_30 + m_VirtualHudScreensizeHalfHeight;
}

float VirtualHud::GetScreenBottomInVirtualUnits() const
{
	return ((m_ScreenSafeArea + 1.f) * m_ScreenHeight * 0.5f - m_ScreenHeightHalf) / field_30 + m_VirtualHudScreensizeHalfHeight;
}

float VirtualHud::GetScreenLeftInVirtualUnits() const
{
	return ((1.f - m_ScreenSafeArea) * m_ScreenWidth * 0.5f - m_ScreenWidthHalf) / field_2C + m_VirtualHudScreensizeHalfWidth;
}

float VirtualHud::GetScreenRightInVirtualUnits() const
{
	return ((m_ScreenSafeArea + 1.f) * m_ScreenWidth * 0.5f - m_ScreenWidthHalf) / field_2C + m_VirtualHudScreensizeHalfWidth;
}

float VirtualHud::_420380() const
{
	return field_2C;
}

void VirtualHud::SetSafeArea(const float area)
{
	m_SafeArea = true;
	m_ScreenSafeArea = area;

	CalculateDimensions();
}

Vector4f& VirtualHud::LimitCoordsToScreen(Vector4f& outvec, const Vector4f& invec) const
{
	float sc = tanf(0.017453292f * (atanf(tanf(0.017453292 * 35.0) * 0.75f) * 57.295776f + atanf(tanf(0.017453292 * 35.0) * 0.75f) * 57.295776f) * 0.5f);
	float px = m_ScreenWidthHalf + (field_2C * (invec.x - m_VirtualHudScreensizeHalfWidth));
	float py = m_ScreenHeightHalf + (field_30 * (invec.y - m_VirtualHudScreensizeHalfHeight));
	float hsc = (field_8 * m_VirtualHudScreensizeHeight) * 0.0016666667f;

	outvec =
	{
		((((((g_GfxInternal_Dx9->m_ViewportResolution.x / g_GfxInternal_Dx9->m_ViewportResolution.y) / m_ScreenRatio) * sc) * (((px / g_GfxInternal_Dx9->m_ViewportResolution.x) * 2.f) - 1.f))
		* (field_C / m_ScreenSafeArea))
		* hsc) * invec.z * m_HudSizeRatio,
		((((0.f - (((py / g_GfxInternal_Dx9->m_ViewportResolution.y) * 2.f) - 1.f)) * sc) * (field_C / m_ScreenSafeArea)) * hsc) * invec.z * m_HudSizeRatio,
		((field_C / m_ScreenSafeArea) * hsc) * invec.z * m_HudSizeRatio,
		0.f
	};
	return outvec;
}

void VirtualHud::SetHudProperties(const float width, const float height, const float ratio, const float safearea)
{
	m_ScreenWidth = width;
	m_ScreenHeight = height;
	m_ScreenRatio = ratio;

	if (!m_SafeArea)
		m_ScreenSafeArea = safearea;

	CalculateDimensions();
}