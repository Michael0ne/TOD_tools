#pragma once

#include "Types.h"
#include "Globals.h"
#include "File.h"
#include <map>

namespace GameConfig
{
	#define CONFIG_CLASS_SIZE 156

	#define CONFIG_GAMENAME "Total Overdose"
	#define CONFIG_SAVEDIR	"/Total Overdose/"
	#define CONFIG_CONFIGFILE "/configpc.txt"
	#define CONFIG_PROFILEFILE "/profile.txt"
	#define CONFIG_SCRIPTS_PATH_STABLE "/data/scripts/stable/"

#ifdef INCLUDE_FIXES
	#define CONFIG_MENU_RESOURCEID 103
#else
	#define CONFIG_MENU_RESOURCEID 0
#endif

	struct ConfigVariables
	{
	protected:
		mutable std::map<String, String>m_KeyValueMap;
		int								field_C;
		std::map<unsigned int, String>	m_Keys;
		int								field_1C;
		std::map<unsigned int, String>	m_UnrecognizedKeys;
		int								field_2C;
		std::map<unsigned int, bool>	m_StringVariables;	//	NOTE: if variable at index is a string then it's set to true.
		int								field_3C;
		std::map<unsigned int, String>	field_40;
		int								field_4C;
		std::map<int, int>				field_50;
		int								field_5C;

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
			return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
		}
		void operator delete(void* ptr)
		{
			if (ptr)
				MemoryManager::ReleaseMemory(ptr, false);
			ptr = nullptr;
		}

		bool IsVariableSet(const char* const variableName) const;	//	@410080

		const bool GetParamValueBool(const char* const variableName) const;	//	@410900
		const int GetParamValueInt(const char* const variableName) const;	//	@410A30
		const float GetParamValueFloat(const char* const variableName) const;	//	@410AC0
		Vector2<int>& GetParamValueVector2i(Vector2<int>& outvec, const char* const variableName, const char delimiter) const;	//	@410B50
		Vector2<unsigned int>& GetParamValueVector2i(Vector2<unsigned int>& outvec, const char* const variableName, const char delimiter) const;	//	@410B50
		Vector2<float>& GetParamValueVector2f(Vector2<float>& outvec, const char* const variableName, const char delimiter) const;	//	@410BE0
		Vector3<float>& GetParamValueVector3(Vector3<float>& outvec, const char* const variableName, const char delimiter) const;	//	@410C70
		Vector4f& GetParamValueVector4(Vector4f& outvec, const char* const variableName, const char delimiter) const;	//	@410D90
		String& GetParamValueString(String& outstr, const char* const variableName) const;	//	@410E30

		void SetParamValue(const char* const variableName, const char* const value);	//	@4114E0
		void SetParamValueBool(const char* const variableName, const bool value);	//	@4116D0
	};

	ASSERT_CLASS_SIZE(ConfigVariables, 104);

	struct ConfigCallback
	{
	public:
		virtual void		UninitialiseGameCallback(int);	//	NOTE: not actually in EXE.
	};

	class Config
	{
		friend class BuiltinType;
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
			return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
		}
		void operator delete(void* ptr)
		{
			if (ptr)
				MemoryManager::ReleaseMemory(ptr, 0);
			ptr = nullptr;
		}

		void				Process(LPSTR cmdline, int, const char* configFileName, signed int iconResId);	//	@93D480
		void				InitEntitiesDatabase();	//	@93C950
		void				UninitialiseGame();	//	@93CBC0
		bool				OpenScene(const char* scene);	//	@93CE00
		void				CreateUnknownMatricies();	//	@93D360

		static bool			CheckAssetChecksum(File& file, const unsigned int propertyChecksum, const unsigned int commandChecksum);	//	@93CB00
		static bool			UpdateGame();	//	@93CEB0
		static void			_93CDA0(const char* const str);	//	@93CDA0

		static String		_A1B9F8;	//	@A1B9F8	//	NOTE: unused, max length is 10'000 symbols.
	};

	ASSERT_CLASS_SIZE(Config, 156);

	void					InitialiseGame(LPSTR cmdline);	//	@93F680

	static void				EnumMaterialsInCollmat();	//	@87D330
	static void				EnumFaceColMaterials();	//	@87D100

	static void				GetInternalGameName(String& outStr);	//	@4098D0

	enum FaceColl
	{
		FACECOLL_METAL = 1,
		FACECOLL_WOOD = 2,
		FACECOLL_DIRT = 3,
		FACECOLL_STONE = 4,
		FACECOLL_BUSH = 5,
		FACECOLL_GLASS = 6,
		FACECOLL_WIREFENCE = 7,
		FACECOLL_GRASS = 8,
		FACECOLL_ASPHALT = 9,
		FACECOLL_WATER = 10,
		FACECOLL_CLOTH = 11,
		FACECOLL_ALLROUND = 12,

		FACECOLL_CONCRETE,
		FACECOLL_SOLIDGLASS,
		FACECOLL_BROKENWINDOW_ELVEZ,
		FACECOLL_BROKENWINDOW_BURGERBAR,
		FACECOLL_BROKENWINDOW_SHOPPINGMALL,
		FACECOLL_BROKENWINDOW_VIRGILLO,
		FACECOLL_DEFAULT_NO_DECAL
	};

	static bool				OpenCollMatFile(const char* const fileName, String& materialName, int& materialProperties);	//	@87CE50
	static bool				ReadAndParseCollMatMaterial(String& materialName, int& outMaterialProperties);	//	@87CC80

	extern std::list<String>	FaceCollList;	//	@A3D7EC
	extern File*			CollMatFile;	//	@A3D7E8
	extern std::map<String, unsigned int>	CollMatProperties;	//	@A11704
	extern std::map<String, FaceColl>	CollMatMaterialsTypes;	//	@A3D7FC

	extern Config* g_Config;
}