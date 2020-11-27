#pragma once

#include "stdafx.h"
#include "time.h"

#include "Globals.h"
#include "Types.h"
#include "List.h"
#include "MemoryAllocators.h"
#include "FileInternal.h"
#include "StringsPool.h"
#include "Progress.h"

namespace GameConfig {

	#define CONFIG_CLASS_SIZE 156

	#define CONFIG_GAMENAME "Total Overdose"
	#define CONFIG_SAVEDIR	"/Total Overdose/"
	#define CONFIG_CONFIGFILE "/configpc.txt"
	#define CONFIG_LANGUAGES 6

#ifdef INCLUDE_FIXES
	#define CONFIG_MENU_RESOURCEID 103
#else
	#define CONFIG_MENU_RESOURCEID 0
#endif // INCLUDE_FIXES

	struct Session_Variables_Entry
	{
		String	m_sVariableName;
		String	m_sVariableValue;
	};

	struct Session_Variables
	{
		int field_0;
		int field_4;
		String* field_8;
		int field_C;

		int* field_10;
		int field_14;
		int field_18;
		int field_1C;

		int field_20;
		int field_24;
		int field_28;
		int field_2C;

		int* field_30;
		int field_34;
		int field_38;
		int field_3C;

		int* field_40;
		int field_44;
		int* field_48;
		int field_4C;

		int field_50;
		int field_54;
		int* field_58;
		int field_5C;

		int m_nTotalVariables;
		int field_64;

	public:
		Session_Variables(int unk)
		{
			MESSAGE_CLASS_CREATED(Session_Variables);

			(*(Session_Variables * (__thiscall*)(Session_Variables*, int))0x410680)(this, unk);
		}
		Session_Variables(const char* file, int unk)
		{
			MESSAGE_CLASS_CREATED(Session_Variables);

			(*(Session_Variables * (__thiscall*)(Session_Variables*, const char*, int))0x4124D0)(this, file, unk);
		}
		~Session_Variables()
		{
			MESSAGE_CLASS_DESTROYED(Session_Variables);

			(*(void(__thiscall*)(Session_Variables*))0x4107B0)(this);
		}

		void* operator new(size_t size)
		{
			return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
		}
		void operator delete(void* ptr)
		{
			if (ptr)
				Allocators::ReleaseMemory(ptr, 0);
		}

		bool IsVariableSet(const char* variableName);	//	@410080

		bool GetParamValueBool(const char* variableName);	//	@410900
		int GetParamValueInt(const char* variableName);	//	@410A30
		float GetParamValueFloat(const char* variableName);	//	@410AC0
		const Vector2<int>& GetParamValueVector2(const char* variableName, Vector2<int>& outVec, char delimiter);	//	@410B50
		const Vector2<float>& GetParamValueVector2(const char* variableName, Vector2<float>& outVec, char delimiter);	//	@410BE0
		const Vector3<float>& GetParamValueVector3(const char* variableName, Vector3<float>& outVec, char delimiter);	//	@410C70
		const Vector4f& GetParamValueVector4(const char* variableName, Vector4f& outVec, char delimiter);	//	@410D90
		void GetParamValueString(const char* variableName, String& outStr);	//	@410E30
		const String& GetParamValueString(const char* variableName);

		void SetParamValue(const char* variableName, char* value);	//	@4114E0
		void SetParamValueBool(const char* variableName, int value);	//	@4116D0
	};

	class Config
	{
	private:
		char field_0;
		String m_sGameName;
		String m_sConfigFilePath;
		Session_Variables* m_pConfigurationVariables;
		Session_Variables* m_pSessionVariables;
		String m_sUnkString_1;
		String m_sUnkString_2;
		int field_4C;
		int field_50;
		void* field_54;
		String m_sSceneName;
		Vector4f m_vBackgroundSize;	//	TODO: better name?

		int m_nGlobalPropertiesListCRC;
		int m_nGlobalCommandsListCRC;
		int m_nTypesListCRC;

		int m_nTotalGlobalProperties;
		int m_nTotalGlobalCommands;
		int m_nTotalTypes;

		int m_nCRCForScriptsListUnk;
		int m_nCRCForScriptsGlobalList;
		int m_nCRCForTypesList;

	public:
		Config();	//	@93CB60
		~Config()
		{
			MESSAGE_CLASS_DESTROYED(Config);

			if (field_0)
				UninitialiseGame();

			delete &m_sUnkString_1;
			delete &m_sUnkString_2;
			delete &m_sSceneName;
			delete &m_sConfigFilePath;
			delete &m_sGameName;
		}

		void* operator new(size_t size)
		{
			return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
		}
		void operator delete(void* ptr)
		{
			if (ptr)
				Allocators::ReleaseMemory(ptr, 0);
		}

		void Process(LPSTR lpCmdLine, int unk, const char* szConfigFilename, signed int nIconResId);	//	@93D480
		void InitEntitiesDatabase();	//	@93C950
		void UninitialiseGame();	//	@93CBC0
		bool OpenScene(const char* scene);	//	@93CE00
		void CreateUnknownMatricies();	//	@93D360
	};

	static void ReadZipDirectories(const char* szFileSystem);	//	@419550
	static void OpenZip(const char* szZipPath);	//	@419100
	static void AddDirectoryMappingsListEntry(const char* szDir, const char* szDirTo); //	@418F90
	static void SetCountryCode(const char* szCode);	//	@42E530
	static signed int GetRegionId(String* regionStr);	//	@875450
	static void EnumMaterialsInCollmat();	//	@87D330
	static void EnumFaceColMaterials();	//	@87D100

	static void _GetDeveloperPath(String* outStr);	//	@4098D0

	static List<String>& FaceColList = *(List<String>*)0xA3D7EC;	//	@A3D7EC
	static FileInternal& ColMatFilePtr = *(FileInternal*)0xA3D7E8;	//	@A3D7E8
}

extern GameConfig::Config* g_Config;

static_assert(sizeof(GameConfig::Config) == CONFIG_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Config));