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

static List<GlobalProperty>		GlobalPropertiesList;	//	@A3CF20

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
		List<ScriptNodeProperties>	m_PropertiesList;	//	NOTE: 0th element is always return type.
		int						m_TotalSizeBytes;	//	NOTE: these 2 seem to be always equal.
		int						m_TotalSize;
	}							m_ArgumentsList;	//	NOTE: element at index 0 is always return scripttype.
	int							m_GlobalIndex;
	const char*					m_ArgumentsString;	//	NOTE: full name with arguments.
	const char*					m_CommandName;	//	NOTE: the name without arguments (only command name).

	void						GetReturnTypeString(String& outStr);	//	@871A90
};

struct CommandListEntry
{
	String						m_Command;
	unsigned int				m_IndexInGlobalList;
	CommandListEntry*			m_Next;
	CommandListEntry*			m_Previous;
};

static List<GlobalCommand>		GlobalCommandsList;		//	@A11470	//	NOTE: this is an array with contents allocated as single space with size and put consequently.
static KeyValueList<CommandListEntry, void>	GlobalCommandsStringsList;	//	@A3CF08

#define GLOBALSCRIPT_CLASS_SIZE 100

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
	List<GlobalScriptField>		m_FieldsList;
	int							m_FieldsDefaultValues[4];	//	[field_id] = field_val
	unsigned int				m_ScriptSizeBytes;
	List<GlobalScriptMethod>	m_MethodsList;
	List<GlobalScriptParam>		m_ParamsList;
	int							field_44;
	ScriptType_Entity*			m_BaseEntity;
	String						m_Name;
	int							field_5C;
	void						(__cdecl* field_60)(int*);	//	NOTE: constructor (?)

public:
	GlobalScript(const char* const scriptName, const char* const parentEntity, bool, bool);	//	@48A530

	void						RegisterMember(unsigned int fieldId, const char* const defaultValue, unsigned int);	//	@48AF10

	ScriptType_Entity*			AssignScriptToEntity(const ScriptType_Entity& parent);	//	@48A3F0

	static GlobalScript*		GetGlobalScriptByName(const char* name);	//	@48C590
	static List<GlobalScript>	ScriptsList;	//	@A0B424
};

static_assert(sizeof(GlobalScript) == GLOBALSCRIPT_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(GlobalScript));

struct ScriptObject
{
protected:
	GlobalScript*				m_ScriptInstance;
	ScriptType*					m_Script;	//	NOTE: cast to appropriate type.
};

static ScriptObject*			GlobalScriptsArray[205];	//	@A3B7A4

class IScriptThread
{
public:
	virtual ~IScriptThread();
	virtual int stub1(void*);
};

class ScriptThread : public IScriptThread
{
	struct ThreadList_1
	{
		unsigned int			field_0;
		int*					field_4;	//	NOTE: pointer to another list. Contents vary.
		unsigned int			field_8;
	};

	struct CallStackElement
	{
		class Node*				m_NodePtr;
		void					(*m_FuncPtr)(ScriptThread*);
		unsigned int			field_8;
		unsigned int			field_C;
		unsigned int			field_10;
	};

	struct ThreadCallStack
	{
		unsigned int			field_0;
		int*					field_4;
		unsigned int			field_8;
	};

	struct MethodStruct
	{
		void					(* field_0)(ScriptThread*);
		int						field_4;
		void					(* field_8)(ScriptThread*);
		Node*					field_C;
		GlobalScript*			field_10;
		unsigned int			field_14;
	};
protected:
	Defragmentator*				m_Defragmentator;
	List<ThreadList_1>			m_List_1;	//	NOTE: associated with defragmentator?
	Defragmentator*				m_Defragmentator_1;
	List<ThreadCallStack>		m_CallStack;
	int							m_SleepUntil;
	int							m_WaitForFrame;
	int							m_ThreadFlags;
	Node*						m_ScriptNode;
	MethodStruct*				field_3C;
	CallStackElement*			field_40;
public:
	virtual ~ScriptThread();	//	@48ED10
	virtual int					stub1(void*);

	ScriptThread(class Node*);	//	@48EC70

	void						DumpState(String&);	//	@48D690
};

namespace Script
{
	static ScriptType* GetScriptType(const char*);	//	@863070
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
	static unsigned int	GetCommandId(const char* commandName, bool alreadyRegistered);	//	@872590

	static unsigned int	GlobalPropertyListChecksum;	//	@A3CF40
	static bool			GlobalPropertyListChecksumObtained;	//	@A3CF1C
	static unsigned int	GlobalCommandListChecksum;	//	@A3CF18
	static bool			GlobalCommandListChecksumObtained;	//	@A3CEF4
}