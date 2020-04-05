#pragma once

#include "stdafx.h"

#include "StringsPool.h"

namespace GameConfig {

	#define CONFIG_CLASS_SIZE 0x9C	//	156 bytes

	#define CONFIG_GAMENAME "Total Overdose"

	struct Session_Variables
	{
		int field_0;
		int field_4;
		int field_8;
		int field_C;
		int field_10;
		int field_14;
		int field_18;
		int field_1C;
		int field_20;
		int field_24;
		int field_28;
		int field_2C;
		int field_30;
		int field_34;
		int field_38;
		int field_3C;
		int field_40;
		int field_44;
		int field_48;
		int field_4C;
		int field_50;
		int field_54;
		int field_58;
		int field_5C;
		int field_60;
		int field_64;

	public:
		Session_Variables* CreateBuffer(int unk);	//	@410680
		Session_Variables* CreateBuffer(const char* szPath, bool unk);	//	@4124D0

		bool IsVariableSet(const char* variableName);	//	@410080
		bool IsVariableValueTrue(const char* variableName);	//	@410900

		template <typename T>
		T GetParamValue(const char* variableName);
		/*
		 *	int @410A30,
		 *	float @410AC0,
		 *	array[2] @410B50,
		 *	vector2 @410BE0,
		 *	vector4 @410C70,
		 *	vector4(color?) @410D90,
		 *	string @410E30
		*/

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
		int field_2C;
		int field_30;
		int field_34;
		int field_38;
		int field_3C;
		int field_40;
		int field_44;
		int field_48;
		int field_4C;
		int field_50;
		int field_54;
		int field_58;
		int field_5C;
		int field_60;
		int field_64;
		int field_68;
		int field_6C;
		int field_70;
		int field_74;
		int m_nCRCForScriptsListUnk;
		int m_nCRCForScriptsGlobalList;
		int m_nCRCForTypesList;
		int m_nScriptsListUnkCurrentIndex;
		int m_nScriptsListGlobalCurrentIndex;
		int m_nTypesListCurrentIndex;
		int field_90;
		int field_94;
		int field_98;

	public:
		Config() {
			debug("GameConfig::Config created at %X\n", this);

			field_0 = 0;
			m_sGameName = String();
			m_sConfigFilePath = String();
			m_pConfigurationVariables = nullptr;
			m_pSessionVariables = nullptr;
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
			field_60 = 0;
			field_64 = 0;
			field_68 = 0;
			field_6C = 0;
			field_70 = 0;
			field_74 = 0;
			m_nCRCForScriptsListUnk = 0;
			m_nCRCForScriptsGlobalList = 0;
			m_nCRCForTypesList = 0;
			m_nScriptsListUnkCurrentIndex = 0;
			m_nScriptsListGlobalCurrentIndex = 0;
			m_nTypesListCurrentIndex = 0;
			field_90 = 0;
			field_94 = 0;
			field_98 = 0;
		}

		~Config() {
			debug("GameConfig::Config destroyed!\n");
		}

		void Process(LPSTR lpCmdLine, int unk, const char* szConfigFilename, signed int nIconResId);	//	@93D480
		void Init();	//	@93CB60
		void InitEntitiesDatabase();	//	@93C950
	};

	static void ReadZipDirectories(const char szFileSystem[]);	//	@419550
}

namespace Script
{
	static const String& LanguageMode = *(String*)0xA086A8;
	static bool& FileCheck = *(bool*)0xA35DE0;
	static bool& ForceFeedback = *(bool*)0xA35E70;
	static String ControlType;
	static bool LoadBlocks;
}

extern GameConfig::Config* g_Config;

static_assert(sizeof(GameConfig::Config) == CONFIG_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE("Config"));