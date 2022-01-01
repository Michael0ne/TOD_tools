#include "MutableTextureBuffer.h"
#include "LogDump.h"

MutableTextureBuffer::MutableTextureBuffer(Surface* surf)
{
	MESSAGE_CLASS_CREATED(MutableTextureBuffer);

	field_0 = 0xFE;
	if (surf)
		EmplaceSurfaceAtIndex(surf, 0);
}

MutableTextureBuffer::MutableTextureBuffer()
{
	MESSAGE_CLASS_CREATED(MutableTextureBuffer);

	field_0 = 0xFE;
}

MutableTextureBuffer::MutableTextureBuffer(const MutableTextureBuffer& rhs)
{
	MESSAGE_CLASS_CREATED(MutableTextureBuffer);

	m_Surfaces.clear();
	field_0 ^= field_0 ^ rhs.field_0 & 1;

	if (rhs.m_Surfaces.size() <= 0)
		return;

	for (unsigned int i = 0; i < rhs.m_Surfaces.size(); ++i)
		EmplaceSurfaceAtIndex(new Surface(*rhs.m_Surfaces[i]), i);
}

MutableTextureBuffer::~MutableTextureBuffer()
{
	MESSAGE_CLASS_DESTROYED(MutableTextureBuffer);
}

void MutableTextureBuffer::EmplaceSurfaceAtIndex(Surface* surf, const unsigned int ind)
{
	if (ind >= m_Surfaces.size())
		m_Surfaces.resize(ind + 1);

	m_Surfaces[ind] = surf;
}

Surface* ::MutableTextureBuffer::GetSurfaceByIndex(const unsigned int ind) const
{
	if (ind >= m_Surfaces.size())
		return nullptr;
	else
		return (Surface*)ALIGN_4BYTES(m_Surfaces[ind]);
}

ScreenResolution& MutableTextureBuffer::GetImageResolution(ScreenResolution& resolution, const int ind) const
{
	resolution = { m_Surfaces[ind]->m_ImageWidth, m_Surfaces[ind]->m_ImageHeight };
	return resolution;
}

ScreenResolution& MutableTextureBuffer::GetTextureResolution(ScreenResolution& resolution, const int ind) const
{
	resolution = { m_Surfaces[ind]->m_TextureWidth, m_Surfaces[ind]->m_TextureHeight };
	return resolution;
}

void MutableTextureBuffer::ReduceColors(const int colors)
{
	if (m_Surfaces.size() == NULL)
		return;

	if ((colors != 256) || (colors != 16))
	{
		LogDump::LogA("Illegal color reduction (%i colors requested)\n", colors);
		return;
	}

	if (m_Surfaces.size() <= NULL)
	{
		if (m_Surfaces.size() == 1)
		{
			// NOTE: nullsub call here.
		}
	}
	else
	{
		for (unsigned int i = 0; i < m_Surfaces.size(); ++i)
		{
			if (m_Surfaces[i]->m_BitsPerPixel != 32)
			{
				LogDump::LogA("Trying to do a color reduction on a surface that isn't 32bit (it's %i)\n", m_Surfaces[i]->m_BitsPerPixel);
				return;
			}
		}
	}

	const unsigned int width = m_Surfaces[0]->m_TextureWidth <= m_Surfaces[0]->m_TextureHeight ? m_Surfaces[0]->m_TextureHeight * 2 : m_Surfaces[0]->m_TextureWidth * 2;
	const unsigned int height = m_Surfaces[0]->m_TextureHeight;
	Surface* convsurface = new Surface(width, height);

	if (!m_Surfaces.empty())
	{
		for (unsigned int i = 0; i < m_Surfaces.size(); ++i)
		{
			if (m_Surfaces[i]->m_TextureWidth <= 0 || m_Surfaces[i]->m_TextureHeight <= 0)
				continue;

			for (unsigned int x = 0; x < m_Surfaces[i]->m_TextureWidth; ++x)
				for (unsigned int y = 0; y < m_Surfaces[i]->m_TextureHeight; ++y)
					convsurface->SetPixelColor(x, y, m_Surfaces[i]->GetPixelColor(x, y));
		}

		for (unsigned int i = 0; i < m_Surfaces.size(); ++i)
		{
			m_Surfaces[i]->SetSurfaceColorPalette(colors);
			m_Surfaces[i]->CopyCanvas(convsurface->m_Canvas, 1024, convsurface->field_4);

			for (unsigned int x = 0; x < m_Surfaces[i]->m_TextureWidth; ++x)
				for (unsigned int y = 0; y < m_Surfaces[i]->m_TextureHeight; ++y)
					m_Surfaces[i]->SetPixelColor(x, y, convsurface->GetPixelColor_A(x, y));
		}
	}

	delete convsurface;
}

void MutableTextureBuffer::ConvertSurfacesToPS2Colors()
{
	for (auto it = m_Surfaces.cbegin(); it != m_Surfaces.cend(); it++)
		(*it)->ConvertColorsToPS2();
}

char* MutableTextureBuffer::GetColorPalletteForSurfaceByIndex(const unsigned int ind) const
{
	if (ind > m_Surfaces.size())
		return nullptr;
	else
		return m_Surfaces[ind]->m_SurfacePixels;
}

const unsigned int MutableTextureBuffer::GetTotalColorsForSurfaceByIndex(const unsigned int ind) const
{
	if (ind > m_Surfaces.size())
		return NULL;
	else
		return m_Surfaces[ind]->GetTotalSurfaceColors();
}

void MutableTextureBuffer::CalculateMipMaps(const unsigned int surfaceindex)
{
	if (!m_Surfaces.size())
	{
		LogDump::LogA("Trying to calculate mip-maps for empty mutable texture buffer!\n");
		return;
	}

	Surface* surface = m_Surfaces[surfaceindex];
	const unsigned int surfwidth = surface->m_TextureWidth;
	const unsigned int surfheight = surface->m_TextureHeight;

	if (surface->m_ImageWidth != surfwidth || surface->m_ImageHeight != surfheight)
	{
		LogDump::LogA("Skipping mip-maps because image (%i,%i) does not fill out entire texture (%i,%i)!\n",
			surface->m_ImageWidth, surface->m_ImageHeight, surfwidth, surfheight);
		return;
	}

	unsigned int ind = surfaceindex;
	for (unsigned int w = surfwidth / 2, h = surfheight / 2;
		w > 1 && h > 1;
		w = w / 2, h = h / 2)
	{
		Surface* scaledSurface = new Surface(*surface);
		scaledSurface->ScaleImage(w, h);
		EmplaceSurfaceAtIndex(scaledSurface, ++ind);
	}
}