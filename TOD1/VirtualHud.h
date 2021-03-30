#pragma once
#include "stdafx.h"
#include "Types.h"

class VirtualHud
{
	friend class BuiltinType;
protected:
	float	m_VirtualHudScreensizeWidth = 0.f;
	float	m_VirtualHudScreensizeHeight = 0.f;
	float	field_8 = 1.f;	//	NOTE: scaleX?
	float	field_C = 1.f;	//	NOTE: scaleY?
	float	m_ScreenWidth = 0.f;
	float	m_ScreenHeight = 0.f;
	float	m_ScreenRatio = 1.f;
	float	m_ScreenSafeArea = 1.f;
	char	m_SafeArea = false;
	char	field_21 = NULL;
	float	m_VirtualHudScreensizeHalfWidth = 0.f;
	float	m_VirtualHudScreensizeHalfHeight = 0.f;
	float	field_2C = 0.f;
	float	field_30 = 0.f;
	float	m_ScreenWidthHalf = 0.f;
	float	m_ScreenHeightHalf = 0.f;
	float	m_HudSizeRatio = 0.f;
	float	field_40 = 0.f;

public:
	inline VirtualHud()	//	@9B12A0
	{
		m_VirtualHudScreensizeWidth = 640.f;
		m_VirtualHudScreensizeHeight = 480.f;
		m_ScreenWidth = 640.f;
		m_ScreenHeight = 480.f;
	}

	void	CalculateDimensions();	//	@420190
	Vector2f&	_420280(Vector2f& outvec, const float, const float) const;	//	@420280
	Vector2f&	_4202D0(Vector2f& outvec, const float, const float) const;	//	@4202D0
	float	GetScreenTopInVirtualUnits() const;	//	@420300
	float	GetScreenBottomInVirtualUnits() const;	//	@420320
	float	GetScreenLeftInVirtualUnits() const;	//	@420340
	float	GetScreenRightInVirtualUnits() const;	//	@420360
	float	_420380() const;	//	@420380
	void	SetVirtualHudScreenSize(const float width, const float height, const float, const float);	//	@420D60
	void	SetHudProperties(const float width, const float height, const float ratio, const float safearea);	//	@420D90
	void	SetSafeArea(const float area);	//	@420DD0
	Vector4f&	LimitCoordsToScreen(Vector4f& outvec, const Vector4f& invec) const;	//	@420DF0	//	NOTE: or simply 3d coords to 2d coords, but doesn't seems to be it.

	static VirtualHud	VirtualHudInstance;	//	@A08810
};

ASSERT_CLASS_SIZE(VirtualHud, 68);