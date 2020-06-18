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
		Session_Variables()
		{
			field_0 = 0;
			field_4 = 0;
			field_8 = 0;
			field_C = 0;

			field_10 = 0;
			field_14 = 0;
			field_18 = 0;
			field_1C = 0;

			field_20 = 0;
			field_24 = 0;
			field_28 = 0;
			field_2C = 0;

			field_30 = 0;
			field_34 = 0;
			field_38 = 0;
			field_3C = 0;

			field_40 = 0;
			field_44 = 0;
			field_48 = 0;
			field_4C = 0;

			field_50 = 0;
			field_54 = 0;
			field_58 = 0;
			field_5C = 0;

			m_nTotalVariables = 0;
			field_64 = 0;

			debug("GameConfig::Session_Variables created at %X\n", this);
		}

		~Session_Variables()
		{
			(*(void(__thiscall*)(Session_Variables*))0x4107B0)(this);

			debug("GameConfig::Session_Variables destroyed!\n");
		}

		void* operator new(size_t size)
		{
			return Allocators::AllocatorsList[Allocators::ALLOCATOR_DEFAULT]->allocate(size);
		}

		void operator delete(void* ptr)
		{
			if (ptr)
				Allocators::MemoryAllocators::ReleaseMemory(ptr, 0);
		}

		Session_Variables* CreateBuffer(int unk);	//	@410680
		Session_Variables* CreateBuffer(const char* szPath, bool unk);	//	@4124D0

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
		int field_0;
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
		int _pad[3];
		int m_nScriptsListUnkCurrentIndex;
		int m_nScriptsListGlobalCurrentIndex;
		int m_nTypesListCurrentIndex;
		int m_nCRCForScriptsListUnk;
		int m_nCRCForScriptsGlobalList;
		int m_nCRCForTypesList;

	public:
		Config() {
			debug("GameConfig::Config created at %X\n", this);

			field_0 = 0;
			m_sGameName = String();
			m_sConfigFilePath = String();
			m_sUnkString_1 = String();
			m_sUnkString_2 = String();
			m_sSceneName = String();
			m_pConfigurationVariables = nullptr;
			m_pSessionVariables = nullptr;
			field_4C = 0;
			field_50 = 0;
			m_vBackgroundSize = Vector4<float>();
			m_nCRCForScriptsListUnk = 0;
			m_nCRCForScriptsGlobalList = 0;
			m_nCRCForTypesList = 0;
			m_nScriptsListUnkCurrentIndex = 0;
			m_nScriptsListGlobalCurrentIndex = 0;
			m_nTypesListCurrentIndex = 0;
			_pad[0] = 0;
			_pad[1] = 0;
			_pad[2] = 0;
		}

		~Config() {
			if (field_0)
				UninitialiseGame();

			delete &m_sUnkString_1;
			delete &m_sUnkString_2;
			delete &m_sSceneName;
			delete &m_sConfigFilePath;
			delete &m_sGameName;

			debug("GameConfig::Config destroyed!\n");
		}

		void* operator new(size_t size)
		{
			return Allocators::AllocatorsList[Allocators::ALLOCATOR_DEFAULT]->allocate(size);
		}
		void operator delete(void* ptr)
		{
			if (ptr)
				Allocators::MemoryAllocators::ReleaseMemory(ptr, 0);
		}

		void Process(LPSTR lpCmdLine, int unk, const char* szConfigFilename, signed int nIconResId);	//	@93D480
		void Init();	//	@93CB60
		void InitEntitiesDatabase();	//	@93C950
		void UninitialiseGame();	//	@93CBC0
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

namespace Script
{
	static String& LanguageMode = *(String*)0xA086A8;
	static bool& FileCheck = *(bool*)0xA35DE0;
	static bool& ForceFeedback = *(bool*)0xA35E70;	
	static bool LoadBlocks;
	static int& Ps2MaxTextureSize = *(int*)0xA10FF0;
	static bool Fullscreen;
	static List<String>& DirectoryMappings = *(List<String>*)0xA35DE4;
	static String& ScriptsPath = *(String*)0xA0B434;
	static bool& RelaxBuildVersionCheck = *(bool*)0xA5D5B0;
	static int IconResourceId;
	static int& LanguageStringsOffset = *(int*)0xA35E28;
	/*static const char** CountryCodes[6] = {
		(const char**)0xA089BC,	//	UK
		(const char**)0xA089C0,	//	FR
		(const char**)0xA089C4,	//	IT
		(const char**)0xA089C8,	//	DE
		(const char**)0xA089CC,	//	ES
		(const char**)0xA089D0	//	DK
	};*/
	static const char* CountryCodes[CONFIG_LANGUAGES] = {
		"uk",
		"fr",
		"it",
		"de",
		"es",
		"dk"
	};
	static bool& SavePlatformPS2 = *(bool*)0xA090C8;
	static bool& CutsceneDisableAware = *(bool*)0xA3D892;
	static bool& CutsceneForceCompleteLodUpdates = *(bool*)0xA5D5A8;
	static String& StreamedSoundExt = *(String*)0xA35EE4;
	static String& VersionName = *(String*)0xA1B9C8;
	static char& _A1B98D = *(char*)0xA1B98D;
	static bool& ShowHud = *(bool*)0xA3E0C8;
	static bool& CheckOriginalAsset = *(bool*)0xA11540;
	static bool& WarningShow = *(bool*)0xA082FE;
	static bool& FrameConsoleMarker = *(bool*)0xA1B98C;
	static bool& CheckDataSanity = *(bool*)0xA5D5A9;
	static bool& CheckDivisionByZero = *(bool*)0xA5D5AA;
	static bool& Ps2PlayCtrl = *(bool*)0xA5D5B8;
	static float& MinFadeDist = *(float*)0xA11C8C;
	static bool& LodAndFade = *(bool*)0xA11C88;
	static bool& CheckDanglingRefs = *(bool*)0xA11C88;
	static bool& FixDanglingRefs = *(bool*)0xA1207D;
	static bool& SimulateReleaseBuild = *(bool*)0xA3B584;

	extern String Filesystem;
	extern String ControlType;
	extern String Region;
}

extern GameConfig::Config* g_Config;

static_assert(sizeof(GameConfig::Config) == CONFIG_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Config));