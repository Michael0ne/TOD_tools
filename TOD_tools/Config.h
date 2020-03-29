#pragma once

#include "stdafx.h"
#include "StringsPool.h"

namespace GameConfig {

	#define CONFIG_CLASS_SIZE 0x9C	//	156 bytes

	struct Config_Buffer
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
	};

	class Config
	{
		/*
	private:
		int	m_bInitialised;
		String	m_sGameName;
		String	m_sConfigFilename;
		Config_Buffer* m_pCurrentString;
		Config_Buffer* m_pSessionVariable;
		String m_sUnkString_1;
		String m_sUnkString_2;
		int	field_4C;
		int	m_bStartEditor;
		void(__stdcall*** m_pUnkReleaseMethodPtr)(signed int);
		String m_sSceneName;
		int	field_68;
		int	field_6C;
		int	field_70;
		int	field_74;
		int	field_78;
		int	field_7C;
		int	field_80;
		int	field_84;
		int	field_88;
		int	field_8C;
		int	field_90;	//	Could be CRC of scripts types, same goes for fields below.
		int	field_94;
		int	field_98;
		*/
		int	field_0;
		int	field_4;
		int	field_8;
		int	field_C;
		int	field_10;
		int	field_14;
		int	field_18;
		int	field_1C;
		int	field_20;
		int	field_24;
		int	field_28;
		int	field_2C;
		int	field_30;
		int	field_34;
		int	field_38;
		int	field_3C;
		int	field_40;
		int	field_44;
		int	field_48;
		int	field_4C;
		int	field_50;
		int	field_54;
		int	field_58;
		int	field_5C;
		int	field_60;
		int	field_64;
		int	field_68;
		int	field_6C;
		int	field_70;
		int	field_74;
		int	field_78;
		int	field_7C;
		int	field_80;
		int	field_84;
		int	field_88;
		int	field_8C;
		int	field_90;
		int	field_94;
		int	field_98;


	public:
		Config() {
			debug("Config constructor\n");			
		}

		~Config() {
			debug("Config destructor\n");
		}

		void Process(LPSTR lpCmdLine, int unk, const char* szGameDir, signed int nIconResId);	//	@93D480
		void Init();	//	@93CB60
	};

	static void ReadZipDirectories(char* szFileSystem);	//	@419550
}

extern GameConfig::Config* g_Config;

static_assert(sizeof(GameConfig::Config) == CONFIG_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE("Config"));