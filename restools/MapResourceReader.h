#pragma once
#include "GenericResourceReader.h"

class MapResourceReader : public GenericResourceReader
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

	AssetHeaderStruct_t		m_HeaderStruct;
	char					m_DataDeciphered[256] = {};
	char					m_CypherKey[32] = {};
	UINT					field_1A8 = NULL;
	UINT					m_TotalResources = NULL;
	UINT					m_EngineTimestamp = NULL;
	UINT					m_PropertyCRC = NULL;
	UINT					m_CommandsCRC = NULL;

	UINT					m_ResInFile = NULL;
	UINT					m_ResElemSize = NULL;
	UINT					m_ResBuffSize = NULL;
	PCHAR					m_ResBuffer = nullptr;
	PCHAR					m_ResTable = nullptr;

public:
	MapResourceReader(LPCSTR filename);
	~MapResourceReader();

	virtual void	ReadInfo() override;
	virtual void	PrintInfo() const override;
};