#pragma once

#include "ScriptTypes.h"

#ifdef PLATFORM_PS2
	#define SCRIPT_PROPERTIES_TOTAL 5940
	#define SCRIPT_PROPERTIES_BUILTIN_TOTAL 711

	#define SCRIPT_PROPERTIES_BUILTIN_CRC 0xE99606BA
	#define SCRIPT_PROPERTIES_DATABASE_CRC 0x24A37B21
#else
	#define SCRIPT_PROPERTIES_TOTAL 5994
	#define SCRIPT_PROPERTIES_BUILTIN_TOTAL 711

	#define SCRIPT_PROPERTIES_BUILTIN_CRC 0xE99606BA
	#define SCRIPT_PROPERTIES_LOADED_CRC 0x65C37710
#endif

struct GlobalProperty
{
	int							m_PropertyId;
	const char*					m_PropertyName;
	ScriptType*					m_PropertyType;

	void						GetNameAndType(String& outStr);	//	@8731C0
};

static List<GlobalProperty>&	GlobalPropertiesList = *(List<GlobalProperty>*)0xA3CF20;	//	@A3CF20

struct ScriptNodeProperties
{
	String						m_PropertyName;
	ScriptType*					m_ScriptType;
	int							m_TotalSizeBytes;
};

struct GlobalCommand
{
	struct
	{
		List<ScriptNodeProperties>	m_PropertiesList;
		int						m_TotalSizeBytes;
		int						m_TotalSize;
	}							m_ArgumentsList;	//	NOTE: element at index 0 is always return scripttype.
	int							m_GlobalIndex;
	const char*					m_ArgumentsString;
	const char*					m_CommandName;

	void						GetReturnTypeString(String& outStr);	//	@871A90
};

static List<GlobalCommand>&		GlobalCommandsList = *(List<GlobalCommand>*)0xA11470;	//	@A11470

class GlobalScript
{
protected:
	List<GlobalScriptProperty>	m_PropertiesList;
	int							m_PropertiesMap[4];
	int							field_20;
	List<int>					m_List_1;
	List<GlobalScriptParam>		m_ParamsList;
	int							field_44;
	ScriptType_Entity*			m_BaseEntity;
	String						m_Name;
	int							field_5C;
	void						(*field_60)(int*);

public:
	GlobalScript();
	~GlobalScript();

	ScriptType_Entity*			AssignScriptToEntity(const ScriptType_Entity& parent);	//	@48A3F0

	static GlobalScript*		GetGlobalScriptByName(const char* name);	//	@48C590
	static List<GlobalScript>	ScriptsList;	//	@A0B424
};

static ScriptType_Entity*	GlobalScriptsArray[410];	//	@A3B7A4

enum GlobalScriptIndex
{
	SCRIPT_CROSSBLEND_OVERRIDE = 1,
	SCRIPT_AIPASSENGER = 251,
	LIST_SCRIPT_AIPASSENGER = 372,
	SCRIPT_TRAFFIC_SECTION = 152,
	LIST_SCRIPT_TRAFFIC_SECTION = 159,
	SCRIPT_TUTORIALMENU = 92,
	//	TODO: much much more.
};

namespace Script
{
	static ScriptType*	GetScriptType(const char*);	//	@863070
	static void	ReadDatabaseFile(const char* path);	//	@48C400
	extern void	LoadScripts();	//	@7A1F60

	static bool FileCheck = false;	//	@A35DE0
	static bool ForceFeedback = false;	//	@A35E70
	static bool SavePlatformPS2 = true;	//	@A090C8
	static unsigned int Ps2MaxTextureSize = 1024;	//	@A10FF0
	static const char* const CountryCodes[] = {
		"uk", "fr", "it", "de", "es", "dk"
	};	//	@A089BC
	static bool _A1B98D = true;	//	@A1B98D
	static bool CheckOriginalAsset = true;	//	@A11540
	static bool WarningShow = true;	//	@A082FE
	static bool FrameConsoleMarker = true;	//	@A1B98C
	static float MinFadeDist = 15.f;	//	@A11C8C
	static bool LodAndFade = true;	//	@A11C88
	static bool FixDanglingRefs = true;	//	@A1207D
	static unsigned int LanguageStringsOffset = 0;	//	@A35E28
	static bool RelaxBuildVersionCheck;	//	@A5D5B0
	static bool CutsceneDisableAware;	//	@A3D892
	static bool CutsceneForceCompleteLodUpdates;	//	@A5D5A8
	static bool ShowHud;	//	@A3E0C8
	static bool CheckDataSanity;	//	@A5D5A9
	static bool CheckDivisionByZero;	//	@A5D5AA
	static bool Ps2PlayCtrl;	//	@A5D5B8
	static bool CheckDanglingRefs;	//	@A3DC4C
	static bool SimulateReleaseBuild;	//	@A3B584

	static bool LoadBlocks;
	static bool Fullscreen;
	static int IconResourceId;

	extern String Filesystem;
	extern String ControlType;
	extern String Region;
	extern String VersionName;	//	@A1B9C8
	extern String StreamedSoundExt;	//	@A35EE4
	extern String LanguageMode;	//	@A086A8
	extern String ScriptsPath;	//	@A0B434
	extern List<StringTuple> DirectoryMappings;	//	@A35DE4

	static bool IsRegionEurope() { return false; };	//	@420160
	static const char* GetCurrentCountryCode() { return CountryCodes[LanguageStringsOffset]; };	//	@42E500
	static unsigned int GetGlobalPropertyListCRC();	//	@873440
	static unsigned int GetGlobalCommandListCRC();	//	@871DD0

	static unsigned int	GlobalPropertyListChecksum;	//	@A3CF40
	static bool			GlobalPropertyListChecksumObtained;	//	@A3CF1C
	static unsigned int	GlobalCommandListChecksum;	//	@A3CF18
	static bool			GlobalCommandListChecksumObtained;	//	@A3CEF4
}