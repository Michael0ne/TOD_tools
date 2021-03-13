#pragma once
#include "Globals.h"
#include "LogDump.h"
#include <vector>
#include <map>

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
	int							m_PropertyId = 0;
	char*						m_PropertyName = nullptr;
	class ScriptType*			m_PropertyType = nullptr;

	void						GetNameAndType(String& outStr) const;	//	@8731C0

	GlobalProperty() {};
	GlobalProperty(const char* const propertyname, unsigned int ind);
	~GlobalProperty();
};

extern std::vector<GlobalProperty>		GlobalPropertiesList;	//	@A3CF20
extern std::map<String, unsigned int>	GlobalPropertiesMap;	//	@A3CF30

class GlobalCommand
{
protected:
	struct CommandArgument
	{
		String					m_PropertyName;
		class ScriptType*		m_ScriptType;
		unsigned int			m_TotalSizeBytes;

		CommandArgument(String& argname, const class ScriptType* argtype, unsigned int argsize);
	};

	struct CommandArguments
	{
		std::vector<CommandArgument>	m_ArgumentsList;
		int						field_C;
		unsigned int			m_TotalSizeBytes;
		unsigned int			m_TotalSize;
	};
	
	CommandArguments			m_Arguments;	//	NOTE: element at index 0 is always return scripttype.
	int							m_GlobalIndex;
	const char*					m_ArgumentsString;	//	NOTE: full name with arguments.
	const char*					m_CommandName;	//	NOTE: the name without arguments (only command name).

private:
	void						AddArgumentType(const ScriptType* argtype);	//	@862650

public:
	GlobalCommand(const char* const commandname, const unsigned int commandind);	//	@871FE0
	~GlobalCommand();

	void						GetReturnTypeString(String& outStr);	//	@871A90
};

extern std::vector<GlobalCommand>		GlobalCommandsList;	//	@A11470
extern std::map<String, unsigned int>	GlobalCommandsMap;	//	@A3CF08

//	NOTE: more appropriate name could be 'BuiltinScript' since they're baked in game. Essentially this is a representation of a class instance that can be used by scripts in game.
class GlobalScript
{
	struct GlobalScriptField
	{
		struct GlobalProperty
		{
			unsigned int		m_PropertyIndex;
			const char*			m_PropertyName;
			ScriptType*			m_PropertyType;
		}						m_GlobalProperty;
		unsigned int			m_FieldOffset;
		const char*				m_DefaultValue;
		unsigned int			field_C;
	};

	struct GlobalScriptMethod
	{
		unsigned int			field_0;
		unsigned int			field_4;
		unsigned int			field_8;
	};

	struct GlobalScriptParam
	{
		void					(* m_ProcPtr)(void*);
		ScriptType*				m_ParamType;
	};
protected:
	std::vector<GlobalScriptField>	m_FieldsList;
	int							m_FieldsDefaultValues[4];	//	[field_id] = field_val
	unsigned int				m_ScriptSizeBytes;
	std::vector<GlobalScriptMethod>	m_MethodsList;
	std::vector<GlobalScriptParam>		m_ParamsList;
	int							field_44;
	class ScriptType_Entity*	m_BaseEntity;
	String						m_Name;
	int							field_5C;
	void						(__cdecl* field_60)(int*);	//	NOTE: constructor (?)

public:
	GlobalScript(const char* const scriptName, const char* const parentEntity, bool, bool);	//	@48A530

	void						RegisterMember(unsigned int fieldId, const char* const defaultValue, unsigned int);	//	@48AF10

	class ScriptType_Entity*	AssignScriptToEntity(const ScriptType_Entity* parent);	//	@48A3F0

	static GlobalScript*		GetGlobalScriptByName(const char* name);	//	@48C590
	static std::vector<GlobalScript>	ScriptsList;	//	@A0B424
};

ASSERT_CLASS_SIZE(GlobalScript, 100);

struct ScriptObject
{
protected:
	GlobalScript*				m_ScriptInstance;
	ScriptType*					m_Script;	//	NOTE: cast to appropriate type.
};

static ScriptObject*			GlobalScriptsArray[205];	//	@A3B7A4

extern unsigned int	GlobalPropertyListChecksum;	//	@A3CF40
extern bool			GlobalPropertyListChecksumObtained;	//	@A3CF1C
extern unsigned int	GlobalCommandListChecksum;	//	@A3CF18
extern bool			GlobalCommandListChecksumObtained;	//	@A3CEF4

extern unsigned int GetGlobalPropertyListChecksum();	//	@873440
extern unsigned int GetGlobalCommandListChecksum();	//	@871DD0
extern int	GetPropertyIdByName(const char* const propertyname);	//	@8732C0
extern int	GetCommandByName_Impl(const char* const commandname);	//	@872360
extern int	GetCommandByName(const char* const commandname);	//	@872410
extern int	RegisterGlobalProperty(const char* const propertyname, bool existingProperty);	//	@8736B0
extern int	RegisterGlobalCommand(const char* const commandname, bool existingCommand);	//	@872590

extern void	ReadDatabaseFile(const char* path);	//	@48C400
extern void	LoadScripts();	//	@7A1F60

namespace Script
{
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
	static float LodFactor = 1.0f;	//	@A12090
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
	static unsigned int IconResourceId;

	extern String Filesystem;
	extern String ControlType;
	extern String Region;
	extern String VersionName;	//	@A1B9C8
	extern String StreamedSoundExt;	//	@A35EE4
	extern String LanguageMode;	//	@A086A8
	extern String ScriptsPath;	//	@A0B434
	extern std::vector<StringTuple> DirectoryMappings;	//	@A35DE4

	static bool IsRegionEurope() { return false; };	//	@420160
	static const char* GetCurrentCountryCode() { return CountryCodes[LanguageStringsOffset]; };	//	@42E500
	static void SetCountryCode(const char* const lang)	//	@42E530
	{
		unsigned int languageIndex = NULL;

		for (unsigned int i = 0; i < (sizeof(CountryCodes) / sizeof(char*)); i++)
			if (*CountryCodes[i] == *lang && CountryCodes[i][1] == lang[1])
				languageIndex = i;

		Script::LanguageStringsOffset = languageIndex;
	}
}