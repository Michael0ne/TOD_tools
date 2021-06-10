#pragma once
#include "GenericResourceReader.h"

class TextureResourceReader : public GenericResourceReader
{
private:


	static const std::string	PlatformExtension[];
public:
	TextureResourceReader(const char* const filename, PlatformDefinition platform = PC);
	~TextureResourceReader();

	virtual void	ReadInfo() override;
	virtual void	PrintInfo() const override;
};