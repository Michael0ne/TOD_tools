#pragma once
#include "GenericResourceReader.h"

class AssetBlockReader : public GenericResourceReader
{
private:
	struct AssetHeaderStruct_t
	{
		unsigned char m_AssetId[36] = {};
		int field_24 = NULL;
		int field_28 = NULL;
		int field_2C = NULL;
		int field_30 = NULL;
		int field_34 = NULL;

		struct AssetHeaderStruct_1
		{
			char m_OriginalKey[32];

			int field_20;
			int field_24;
			int field_28;

			int field_2C;
			int field_30;
			int field_34;
			int field_38;
			int field_3C;
			int field_40;
			int field_44;
			int field_48;
			int field_4C;

			AssetHeaderStruct_1();	//	@401050
			void	_401450(char* key, char* keydata);	//	@401450
			void	_4010C0(const char* key);	//	@4010C0
			void	_4011A0(char* key);	//	@4011A0
		} field_38;
	};

	//	TODO: fill rest of types. Can't be bothered right now...
	enum AssetTypeIndex
	{
		TEXTURE = 0,
		TYPE_1,
		TYPE_2,
		MODEL,
		FRAGMENT,
		TYPE_5,
		TYPE_6,
		TYPE_7,
		STREAMEDSOUND,
		ANIMATION
	};

	static const char*	AssetTypeIndexString[];

	struct AssetHeaderShared
	{
		unsigned int	m_EngineTimestamp;
		unsigned int	m_PropertyChecksum;
		unsigned int	m_CommandsChecksum;
		int				m_ResourcesTotal;
		unsigned int	m_AssetsHeaderSize;
		unsigned int	m_AssetsBufferSize;
	};

	AssetHeaderShared	m_SharedHeader;
	char*				m_AssetsInfoBuffer;
	char**				m_AssetsDataBuffer;
	char**				m_AssetsNames;
	int*				m_AssetsSizes;

public:
	AssetBlockReader(LPCSTR filename);
	~AssetBlockReader();

	virtual void	ReadInfo() override;
	virtual void	PrintInfo() const override;
	virtual void	DumpData() const override;
};