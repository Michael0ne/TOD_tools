#include "AssetBlockReader.h"

const char* AssetBlockReader::AssetTypeIndexString[] =
{
	"Texture",
	"#1",
	"#2",
	"Model",
	"Fragment",
	"#5",
	"#6",
	"#7",
	"StreamedSound",
	"Animation"
};

AssetBlockReader::AssetHeaderStruct_t::AssetHeaderStruct_1::AssetHeaderStruct_1()
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

void AssetBlockReader::AssetHeaderStruct_t::AssetHeaderStruct_1::_401450(char* key, char* keydata)
{
	_4010C0(key);

	for (unsigned int i = 0; i < strlen(keydata); i++)
	{
		*key = keydata[i];
		_4011A0(key);
		keydata[i] = *key;
	}
}

void AssetBlockReader::AssetHeaderStruct_t::AssetHeaderStruct_1::_4010C0(const char* key)
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

void AssetBlockReader::AssetHeaderStruct_t::AssetHeaderStruct_1::_4011A0(char* key)
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

AssetBlockReader::AssetBlockReader(LPCSTR filename)
{
	m_ResourceExtension = ".main";
	m_ResourceName = filename;
	m_ResourceName += m_ResourceExtension;
}

AssetBlockReader::~AssetBlockReader()
{
	delete[] m_AssetsInfoBuffer;
	for (int i = 0; i < m_SharedHeader.m_ResourcesTotal; ++i)
		delete[] m_AssetsDataBuffer[i];
	delete[] m_AssetsSizes;
}

void AssetBlockReader::ReadInfo()
{
	if (!m_FilePtr ||
		m_LastErrorCode)
		return;

	fread(&m_SharedHeader, sizeof(m_SharedHeader), 1, m_FilePtr);

	m_AssetsInfoBuffer = new char[m_SharedHeader.m_AssetsHeaderSize];
	fread(m_AssetsInfoBuffer, m_SharedHeader.m_AssetsHeaderSize, 1, m_FilePtr);
	
	m_AssetsSizes = new int[m_SharedHeader.m_ResourcesTotal];
	fread(m_AssetsSizes, sizeof(m_AssetsSizes), m_SharedHeader.m_ResourcesTotal, m_FilePtr);

	m_AssetsDataBuffer = (char**)new char[m_SharedHeader.m_ResourcesTotal * 4];
	m_AssetsNames = (char**)new char[m_SharedHeader.m_ResourcesTotal * 4];
	for (int i = 0; i < m_SharedHeader.m_ResourcesTotal; ++i)
	{
		m_AssetsDataBuffer[i] = new char[m_AssetsSizes[i]];
		fread(m_AssetsDataBuffer[i], m_AssetsSizes[i], 1, m_FilePtr);
	}
}

void AssetBlockReader::PrintInfo() const
{
	printf("\tEngine timestamp:\t%X\n", m_SharedHeader.m_EngineTimestamp);
	printf("\tProperties checksum:\t%X\n", m_SharedHeader.m_PropertyChecksum);
	printf("\tCommands checksum:\t%X\n", m_SharedHeader.m_CommandsChecksum);
	printf("\tResources in block:\t%d\n", m_SharedHeader.m_ResourcesTotal);
	printf("\tResources info header size:\t%d\n", m_SharedHeader.m_AssetsHeaderSize);
	printf("\tResources data buffer size:\t%d\n", m_SharedHeader.m_AssetsBufferSize);

	if (m_SharedHeader.m_ResourcesTotal == -1)
		return;

	printf("******************\n");
	for (int i = 0; i < m_SharedHeader.m_ResourcesTotal; ++i)
	{
		const int	asstypeind = *(int*)m_AssetsInfoBuffer;
		const char* asstypename = AssetTypeIndexString[asstypeind];
		const int	assnameoffset = *((int*)m_AssetsInfoBuffer + 1);

		if (assnameoffset == -1)
		{
			printf("\tAsset has no name! Skipped!\n");
			continue;
		}

		*(int*)&m_AssetsInfoBuffer = (int)(m_AssetsInfoBuffer + assnameoffset + 4);
		const char* assname = m_AssetsInfoBuffer;
		m_AssetsNames[i] = m_AssetsInfoBuffer;
		const size_t assnamelen = strlen(assname);

		printf("\tAsset index:\t%d\n", i);
		printf("\tType:\t%s (%d)\n", asstypename, asstypeind);
		printf("\tSize:\t%d bytes\n", m_AssetsSizes[i]);
		printf("\tPath:\t%s\n", assname);

		//	NOTE: skip reading name.
		*(int*)&m_AssetsInfoBuffer = (int)(m_AssetsInfoBuffer + assnamelen + 1);

		while (*m_AssetsInfoBuffer != NULL)
			*(int*)&m_AssetsInfoBuffer = (int)(m_AssetsInfoBuffer + 1);

		//	TODO: add proper handling for reading each type.
		if (asstypeind == AssetTypeIndex::FRAGMENT)
			continue;

		if (asstypeind == AssetTypeIndex::TEXTURE)
			*(int*)&m_AssetsInfoBuffer = (int)(m_AssetsInfoBuffer + 128);

		if (asstypeind == AssetTypeIndex::STREAMEDSOUND)
			*(int*)&m_AssetsInfoBuffer = (int)(m_AssetsInfoBuffer + 124);

		if (asstypeind == AssetTypeIndex::MODEL)
			*(int*)&m_AssetsInfoBuffer = (int)(m_AssetsInfoBuffer + 13068);

		if (asstypeind == AssetTypeIndex::ANIMATION)
			*(int*)&m_AssetsInfoBuffer = (int)(m_AssetsInfoBuffer + 627);
	}
}

void AssetBlockReader::DumpData() const
{
	if (m_SharedHeader.m_ResourcesTotal <= 0)
	{
		printf("\tSorry, but this asset block has no assets in it!\n");
		return;
	}

	for (int i = 0; i < m_SharedHeader.m_ResourcesTotal; ++i)
	{
		char assname[512] = {};
#ifdef KEEP_STRUCTURE
		strcpy(assname, m_AssetsNames[i]);
#else
		strcpy(assname, strrchr(m_AssetsNames[i], '/') + 1);
#endif
		strcat(assname, ".dump");

		//	TODO: restore asset header so it can be open as normal file.
		FILE* ofile = fopen(assname, "wb");
		fwrite(m_AssetsDataBuffer[i], m_AssetsSizes[i], 1, ofile);
		fclose(ofile);

		printf("Saved asset dump: %s\n", assname);
	}
}