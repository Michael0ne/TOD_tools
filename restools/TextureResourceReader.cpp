#include "TextureResourceReader.h"

const std::string TextureResourceReader::PlatformExtension[] =
{
	"",
	".texture_pc",
	".texture_ps2",
	".texture_xbox"
};

void TextureResourceReader::ReadInfo()
{
	if (!m_FilePtr ||
		m_LastErrorCode)
		return;

}

void TextureResourceReader::PrintInfo() const
{

}

TextureResourceReader::TextureResourceReader(const char* const filename, PlatformDefinition platform)
{
	m_ResourceExtension = PlatformExtension[platform];
	m_ResourceName = filename;
	m_ResourceName += m_ResourceExtension;
}

TextureResourceReader::~TextureResourceReader()
{
}