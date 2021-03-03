#pragma once

#include "Types.h"
#include "Globals.h"
#include "File.h"
#include "KeyValueList.h"

namespace GameConfig {

	#define CONFIG_CLASS_SIZE 156

	#define CONFIG_GAMENAME "Total Overdose"
	#define CONFIG_SAVEDIR	"/Total Overdose/"
	#define CONFIG_CONFIGFILE "/configpc.txt"

#ifdef INCLUDE_FIXES
	#define CONFIG_MENU_RESOURCEID 103
#else
	#define CONFIG_MENU_RESOURCEID 0
#endif

	struct ConfigVariables
	{
		struct VariableNameInfo
		{
			unsigned int	m_Index;
			String			m_Name;
		};

		struct VariableUnknownInfo
		{
			unsigned int	m_Index;
			char			field_4;
		};
	protected:
		KeyValueList<KeyValueListElement<StringTuple>, KeyValueListElement<StringTuple>> m_PlainValues;	//	NOTE: format is "varname=varvalue".
		KeyValueList<KeyValueListElement<VariableNameInfo>, KeyValueListElement<VariableNameInfo>> m_Keys;	//	NOTE: only variables names.
		KeyValueList<int, int> field_20;	//	NOTE: appears to be always empty.
		KeyValueList<KeyValueListElement<VariableUnknownInfo>, KeyValueListElement<VariableUnknownInfo>> field_30;
		KeyValueList<int, int> field_40;	//	NOTE: this and one below appears to be unused.
		KeyValueList<int, int> field_50;
		int		m_TotalVariables;
		char	field_64;

		void	LoadVariablesFile(const char* file, bool configvariables);	//	@412110
		void	ParseVariablesFile(File* file, bool configvariables);	//	@411A30
	public:
		ConfigVariables(int);	//	@410680
		ConfigVariables(const char* file, bool configvariables);	//	@4124D0
		~ConfigVariables();	//	@4107B0

		void* operator new(size_t size)
		{
			return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
		}
		void operator delete(void* ptr)
		{
			if (ptr)
				Allocators::ReleaseMemory(ptr, false);
			ptr = nullptr;
		}

		bool IsVariableSet(const char* const variableName) const;	//	@410080

		const bool GetParamValueBool(const char* const variableName) const;	//	@410900
		const int GetParamValueInt(const char* const variableName) const;	//	@410A30
		const float GetParamValueFloat(const char* const variableName) const;	//	@410AC0
		Vector2<int>& GetParamValueVector2i(Vector2<int>& outvec, const char* const variableName, const char delimiter) const;	//	@410B50
		Vector2<float>& GetParamValueVector2f(Vector2<float>& outvec, const char* const variableName, const char delimiter) const;	//	@410BE0
		Vector3<float>& GetParamValueVector3(Vector3<float>& outvec, const char* const variableName, const char delimiter) const;	//	@410C70
		Vector4f& GetParamValueVector4(Vector4f& outvec, const char* const variableName, const char delimiter) const;	//	@410D90
		String& GetParamValueString(String& outstr, const char* const variableName) const;	//	@410E30

		void SetParamValue(const char* const variableName, const char* const value);	//	@4114E0
		void SetParamValueBool(const char* const variableName, const bool value);	//	@4116D0
	};

	enum CountryCodes
	{
		COUNTRY_UNKNOWN = -1,
		COUNTRY_EUROPE = 0,
		COUNTRY_USA,
		COUNTRY_ASIA
	};

	struct ConfigCallback
	{
	public:
		virtual void		UninitialiseGameCallback(int);	//	NOTE: not actually in EXE.
	};

	class Config
	{
		friend class ScriptType_Builtin;
	protected:
		bool				m_Initialized;
		String				m_GameName;
		String				m_ConfigFilePath;
		ConfigVariables*	m_ConfigurationVariables;
		ConfigVariables*	m_SessionVariables;
		String				m_String_1;
		String				m_String_2;
		int					field_4C;
		bool				m_ShouldStartGame;
		ConfigCallback*		m_UninitialiseCallback;
		String				m_SceneName;
		Vector4f			m_Background;	//	TODO: better name?

		int					m_PropertiesBuiltinChecksum;
		int					m_CommandsBuiltinChecksum;
		int					m_TypesBuiltinChecksum;

		int					m_PropertiesTotal;
		int					m_CommandsTotal;
		int					m_TypesTotal;

		int					m_PropertiesLoadedChecksum;
		int					m_CommandsLoadedChecksum;
		int					m_TypesLoadedChecksum;

	public:
		Config();	//	@93CB60
		~Config();	//	@93CCF0

		void* operator new(size_t size)
		{
			return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
		}
		void operator delete(void* ptr)
		{
			if (ptr)
				Allocators::ReleaseMemory(ptr, 0);
			ptr = nullptr;
		}

		void				Process(LPSTR lpCmdLine, int unk, const char* szConfigFilename, signed int nIconResId);	//	@93D480
		void				InitEntitiesDatabase();	//	@93C950
		void				UninitialiseGame();	//	@93CBC0
		bool				OpenScene(const char* scene);	//	@93CE00
		void				CreateUnknownMatricies();	//	@93D360
	};

	void					InitialiseGame(LPSTR cmdline);	//	@93F680

	static CountryCodes		GetRegionId(String* regionStr);	//	@875450
	static void				EnumMaterialsInCollmat();	//	@87D330
	static void				EnumFaceColMaterials();	//	@87D100

	static void				GetInternalGameName(String& outStr);	//	@4098D0

	struct CollmatListEntry
	{
		String				m_MaterialName;
		unsigned int		m_MaterialIndex;	//	NOTE: some index into another array? Uinque id?
	};

	static List<String>&	FaceColList = *(List<String>*)0xA3D7EC;	//	@A3D7EC
	static File&			ColMatFilePtr = *(File*)0xA3D7E8;	//	@A3D7E8
	static KeyValueList<CollmatListEntry, int>	ColMatList = KeyValueList<CollmatListEntry, int>(0);	//	@A11704	//	TODO: this more looks like double-linked list.

	extern Config* g_Config;
}

ASSERT_CLASS_SIZE(GameConfig::Config, 156);
ASSERT_CLASS_SIZE(GameConfig::ConfigVariables, 104);