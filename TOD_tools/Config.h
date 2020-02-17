#pragma once

#include "stdafx.h"
#include "StringsPool.h"

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
private:
	BYTE m_bInitialised;
	BYTE m_nUnkInt1a[3];
	String m_sGameName;
	String m_sConfigFilename;
	Config_Buffer* m_sCurrentString;
	Config_Buffer* m_sSessionVariable;
	String m_sUnkString1;
	String m_sUnkString2;
	int m_nUnkInt20;
	BYTE m_bStartEditor;
	void(__stdcall*** m_pUnkReleaseMethodPtr)(signed int);
	String m_sSceneName;
	int m_nUnkInt27;
	int m_nUnkInt28;
	int m_nUnkInt29;
	int m_nUnkInt30;
	BYTE _pad[126];
};

