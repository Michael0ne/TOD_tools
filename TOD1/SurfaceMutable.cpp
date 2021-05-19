#include "SurfaceMutable.h"

SurfaceMutable::SurfaceMutable(Surface* surf)
{
	MESSAGE_CLASS_CREATED(SurfaceMutable);

	field_0 = 0xFE;
	if (surf)
		EmplaceSurfaceAtIndex(surf, 0);
}

SurfaceMutable::SurfaceMutable()
{
	MESSAGE_CLASS_CREATED(SurfaceMutable);

	field_0 = 0xFE;
}

SurfaceMutable::SurfaceMutable(const SurfaceMutable& rhs)
{
	MESSAGE_CLASS_CREATED(SurfaceMutable);

	m_Surfaces.clear();
	field_0 ^= field_0 ^ rhs.field_0 & 1;

	if (rhs.m_Surfaces.size() <= 0)
		return;

	for (unsigned int i = 0; i < rhs.m_Surfaces.size(); ++i)
		EmplaceSurfaceAtIndex(new Surface(*rhs.m_Surfaces[i]), i);
}

SurfaceMutable::~SurfaceMutable()
{
	MESSAGE_CLASS_DESTROYED(SurfaceMutable);
}

void SurfaceMutable::EmplaceSurfaceAtIndex(Surface* surf, int ind)
{
	if (ind >= m_Surfaces.size())
		m_Surfaces.resize(ind + 1);

	m_Surfaces[ind] = surf;
}

Surface* ::SurfaceMutable::GetSurfaceByIndex(const int ind) const
{
	if (ind >= m_Surfaces.size())
		return nullptr;
	else
		return (Surface*)ALIGN_4BYTES(m_Surfaces[ind]);
}

ScreenResolution& SurfaceMutable::GetSurfaceResolution(ScreenResolution& resolution, const int ind) const
{
	resolution = { m_Surfaces[ind]->m_Width, m_Surfaces[ind]->m_Height };
	return resolution;
}

ScreenResolution& SurfaceMutable::GetSurfaceResolutionNormalised(ScreenResolution& resolution, const int ind) const
{
	resolution = { m_Surfaces[ind]->m_WidthPowerOf2, m_Surfaces[ind]->m_HeightPowerOf2 };
	return resolution;
}

#pragma message(TODO_IMPLEMENTATION)
void SurfaceMutable::ReduceColors(const int colors)
{
}

void SurfaceMutable::ConvertSurfacesToPS2Colors()
{
	for (auto it = m_Surfaces.cbegin(); it != m_Surfaces.cend(); it++)
		(*it)->ConvertColorsToPS2();
}

char* SurfaceMutable::GetColorPalletteForSurfaceByIndex(const unsigned int ind) const
{
	if (ind > m_Surfaces.size())
		return nullptr;
	else
		return m_Surfaces[ind]->m_ColorPallette;
}

const unsigned int SurfaceMutable::GetTotalColorsForSurfaceByIndex(const unsigned int ind) const
{
	if (ind > m_Surfaces.size())
		return NULL;
	else
		return m_Surfaces[ind]->GetTotalSurfaceColors();
}