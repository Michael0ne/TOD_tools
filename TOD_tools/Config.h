#pragma once

#include "Types.h"
#include "Globals.h"
#include "File.h"

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
#endif

	struct Session_Variables
	{
	protected:
		int field_0;
		int field_4;
		int* field_8;
		int field_C;

		int field_10;
		int field_14;
		int* field_18;
		int field_1C;

		int field_20;
		int field_24;
		int* field_28;
		int field_2C;

		int field_30;
		int field_34;
		int* field_38;
		int field_3C;

		int field_40;
		int field_44;
		int* field_48;
		int field_4C;

		int field_50;
		int field_54;
		int* field_58;
		int field_5C;

		int m_TotalVariables;
		char field_64;

		void	LoadVariablesFile(const char* file, int unk);	//	@412110
		void	ParseVariablesFile(File* file, char unk);	//	@411A30
	public:
		Session_Variables(int);	//	@410680
		Session_Variables(const char* file, int unk);	//	@4124D0
		~Session_Variables();	//	@4107B0

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
	private:
		bool				m_Initialized;
		String				m_GameName;
		String				m_ConfigFilePath;
		Session_Variables*	m_ConfigurationVariables;
		Session_Variables*	m_SessionVariables;
		String				m_sUnkString_1;
		String				m_sUnkString_2;
		int					field_4C;
		bool				m_ShouldStartGame;
		ConfigCallback*		m_UninitialiseCallback;
		String				m_SceneName;
		Vector4f			m_vBackgroundSize;	//	TODO: better name?

		int					m_nGlobalPropertiesListCRC;
		int					m_nGlobalCommandsListCRC;
		int					m_nTypesListCRC;

		int					m_TotalGlobalProperties;
		int					m_TotalGlobalCommands;
		int					m_TotalTypes;

		int					m_GlobalPropertiesListCRC;
		int					m_GlobalCommandsListCRC;
		int					m_TypesListCRC;

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
		}

		void				Process(LPSTR lpCmdLine, int unk, const char* szConfigFilename, signed int nIconResId);	//	@93D480
		void				InitEntitiesDatabase();	//	@93C950
		void				UninitialiseGame();	//	@93CBC0
		bool				OpenScene(const char* scene);	//	@93CE00
		void				CreateUnknownMatricies();	//	@93D360
	};

	void					InitialiseGame(LPSTR cmdline);	//	@93F680

	static void				SetCountryCode(const char* szCode);	//	@42E530
	static CountryCodes		GetRegionId(String* regionStr);	//	@875450
	static void				EnumMaterialsInCollmat();	//	@87D330
	static void				EnumFaceColMaterials();	//	@87D100

	static void				_GetDeveloperPath(String& outStr);	//	@4098D0

	static List<String>&	FaceColList = *(List<String>*)0xA3D7EC;	//	@A3D7EC
	static File&			ColMatFilePtr = *(File*)0xA3D7E8;	//	@A3D7E8

	extern Config* g_Config;
}

static_assert(sizeof(GameConfig::Config) == CONFIG_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Config));