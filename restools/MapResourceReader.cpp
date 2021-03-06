#include "MapResourceReader.h"

MapResourceReader::AssetHeaderStruct_t::AssetHeaderStruct_1::AssetHeaderStruct_1()
{
	field_20 = 0x13579BDF;
	field_24 = 0x2468ACE0;
	field_28 = 0xFDB97531;
	field_2C = 0x80000062;
	field_30 = 0x40000020;
	field_34 = 0x10000002;
	field_38 = 0x7FFFFFFF;
	field_3C = 0x3FFFFFFF;
	field_40 = 0xFFFFFFF;
	field_44 = 0x80000000;
	field_48 = 0xC0000000;
	field_4C = 0xF0000000;

	memset(m_OriginalKey, NULL, sizeof(m_OriginalKey));
}

void MapResourceReader::AssetHeaderStruct_t::AssetHeaderStruct_1::_401450(char* key, char* keydata)
{
	_4010C0(key);

	for (unsigned int i = 0; i < strlen(keydata); i++)
	{
		*key = keydata[i];
		_4011A0(key);
		keydata[i] = *key;
	}
}

void MapResourceReader::AssetHeaderStruct_t::AssetHeaderStruct_1::_4010C0(const char* key)
{
	strcpy(m_OriginalKey, key);

	field_20 = key[3] | ((key[2] | ((key[1] | (0x0000 | (key[0] << 8))) << 8)) << 8);
	if (!field_20)
		field_20 = 0x13579BDF;

	field_24 = key[7] | ((key[6] | ((key[5] | (key[4] << 8)) << 8)) << 8);
	if (!field_24)
		field_24 = 0x2468ACE0;

	field_28 = key[11] | ((key[10] | ((key[9] | (key[8] << 8)) << 8)) << 8);
	if (!field_28)
		field_28 = 0xFDB97531;
}

void MapResourceReader::AssetHeaderStruct_t::AssetHeaderStruct_1::_4011A0(char* key)
{
	unsigned int rounds = 2;
	unsigned char v3 = field_24 & 1,
		v7 = 0,
		v9 = 0,
		v12 = 0,
		v17 = 0,
		v19 = 0,
		v20 = 0,
		v21 = 0,
		v22 = 0,
		v23 = field_24 & 1,
		v24 = field_28 & 1,
		fld20 = field_20;

	do
	{
		if (fld20 & 1)
		{
			v7 = field_44 | ((fld20 ^ field_2C) >> 1);
			if (field_24 & 1)
			{
				v3 = v23 = 1;
				field_24 = field_48 | ((field_24 ^ field_30) >> 1);
			}
			else
			{
				v3 = v23 = 0;
				field_24 = field_3C & (field_24 >> 1);
			}
		}
		else
		{
			v7 = field_38 & (fld20 >> 1);
			if (field_28 & 1)
			{
				v24 = 1;
				field_28 = field_4C | ((field_28 ^ field_34) >> 1);
			}
			else
			{
				v24 = 0;
				field_28 = field_40 & (field_28 >> 1);
			}
		}

		v20 = (2 * v19) | v24 ^ v3;
		if (v7 & 1)
		{
			v9 = field_44 | ((v7 ^ field_2C) >> 1);
			if (field_24 & 1)
			{
				v23 = 1;
				field_24 = field_48 | ((field_24 ^ field_30) >> 1);
			}
			else
			{
				v23 = 0;
				field_24 = field_3C & (field_24 >> 1);
			}
		}
		else
		{
			v9 = field_38 & (v7 >> 1);
			if (field_28 & 1)
			{
				v24 = 1;
				field_28 = field_4C | ((field_28 ^ field_34) >> 1);
			}
			else
			{
				v24 = 0;
				field_28 = field_40 & (field_28 >> 1);
			}
		}

		v21 = (2 * v20) | v24 ^ v23;
		if (v9 & 1)
		{
			v12 = field_44 | ((v9 ^ field_2C) >> 1);
			if (field_24 & 1)
			{
				v23 = 1;
				field_24 = field_48 | ((field_24 ^ field_30) >> 1);
			}
		}
		else
		{
			v12 = field_38 & (v9 >> 1);
			if (field_28 & 1)
			{
				v24 = 1;
				field_28 = field_4C | ((field_28 ^ field_34) >> 1);
			}
			else
			{
				v24 = 0;
				field_28 = field_40 & (field_28 >> 1);
			}
		}

		v22 = (2 * v21) | v24 & v23;
		if (v12 & 1)
		{
			fld20 = field_44 | ((v12 ^ field_2C) >> 1);
			if (field_24 & 1)
			{
				v23 = 1;
				field_24 = field_48 | ((field_24 ^ field_30) >> 1);
			}
			else
			{
				v23 = 0;
				field_24 = field_3C & (field_24 >> 1);
			}
		}
		else
		{
			fld20 = field_38 & (v12 >> 1);
			if (field_28 & 1)
			{
				v24 = 1;
				field_28 = field_4C | ((field_28 ^ field_34) >> 1);
			}
			else
			{
				v24 = 0;
				field_28 = field_40 & (field_28 >> 1);
			}
		}

		v19 = v17 = (2 * v22) | v24 ^ v23;
		v3 = v23;

	} while (--rounds);

	field_20 = fld20;
	*key ^= v17;
	if (v17 == *key)
		*key = v17;
}

MapResourceReader::MapResourceReader(LPCSTR filename)
{
	m_ResourceExtension = ".map";
	m_ResourceName = filename;
	m_ResourceName += m_ResourceExtension;
}

MapResourceReader::~MapResourceReader()
{
	delete[] m_ResBuffer;
	delete[] m_ResTable;
}

void MapResourceReader::ReadInfo()
{
	if (!m_FilePtr ||
		m_LastErrorCode)
		return;

	char keybuf[255] = {};

	fread(m_HeaderStruct.m_AssetId, sizeof(m_HeaderStruct.m_AssetId), 1, m_FilePtr);
	fread(keybuf, sizeof(keybuf), 1, m_FilePtr);

	strcpy(m_CypherKey, "1E564E3B-D243-4ec5-AFB7");
	m_HeaderStruct.field_38._401450(m_CypherKey, keybuf);
	strcpy(m_DataDeciphered, keybuf);

	fread(&field_1A8, sizeof(field_1A8), 1, m_FilePtr);
	fread(&m_TotalResources, sizeof(m_TotalResources), 1, m_FilePtr);
	fread(&m_EngineTimestamp, sizeof(m_EngineTimestamp), 1, m_FilePtr);
	fread(&m_PropertyCRC, sizeof(m_PropertyCRC), 1, m_FilePtr);
	fread(&m_CommandsCRC, sizeof(m_CommandsCRC), 1, m_FilePtr);

	fread(&m_ResInFile, sizeof(m_ResInFile), 1, m_FilePtr);
	fread(&m_ResElemSize, sizeof(m_ResElemSize), 1, m_FilePtr);
	fread(&m_ResBuffSize, sizeof(m_ResBuffSize), 1, m_FilePtr);

	fseek(m_FilePtr, ~(1 - 1) & (1 + ftell(m_FilePtr) - 1), SEEK_SET);
	m_ResBuffer = new CHAR[m_ResElemSize];
	fread(m_ResBuffer, m_ResElemSize, 1, m_FilePtr);

	fseek(m_FilePtr, ~(1 - 1) & (1 + ftell(m_FilePtr) - 1), SEEK_CUR);
	m_ResTable = new CHAR[16 * m_ResInFile];
	fread(m_ResTable, 4 * m_ResInFile, 1, m_FilePtr);
}

void MapResourceReader::PrintInfo() const
{
	printf("\tArch. header id:\t%s\n", m_HeaderStruct.m_AssetId);
	printf("\tDecypher key:\t%s\n", m_CypherKey);
	printf("\tDeciphered data dump:\n\t\t");

	UINT nempt = NULL;
	for (UINT i = NULL; i < sizeof(m_DataDeciphered); i++)
	{
		if (m_DataDeciphered[i])
			++nempt;
		printf("%X ", m_DataDeciphered[i] & 0x000000FF);
	}

	printf("\n\t%u non-empty out of %u total\n", nempt, sizeof(m_DataDeciphered));
	printf("\tfield_1A8:\t%x\n", field_1A8);
	printf("\tTotal resources:\t%u\n", m_TotalResources);
	printf("\tEngine timestamp:\t%u\n", m_EngineTimestamp);
	printf("\tProperty CRC:\t%X\n", m_PropertyCRC);
	printf("\tCommands CRC:\t%X\n", m_CommandsCRC);
	printf("\tResources in file:\t%u\n", m_ResInFile);
	printf("\tResource element size:\t%u\n", m_ResElemSize);
	printf("\tResource buffer size:\t%u\n", m_ResBuffSize);

	printf("\tDump first 64 bytes out of %u for resource buffer:\n\t\t", m_ResElemSize);
	for (UINT i = NULL; i < 64; i += 4)
	{
		printf("%X ", *(int*)&m_ResBuffer[i]);
	}

	printf("\n\tResource table %u bytes:\n\t\t", 4 * m_ResInFile);
	for (UINT i = NULL; i < (m_ResInFile * 4); i++)
	{
		printf("%X ", m_ResTable[i] & 0x000000FF);
		if (i % 4 == 3)
			printf("\n\t\t");
	}
}