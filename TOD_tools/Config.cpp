#include "Config.h"

GameConfig::Config* g_Config = NULL;

namespace GameConfig {

	void Config::Init()
	{
		//	Replace exe's pointer to Config class object to point to this class instance.
		g_Config = this;
		patch(0xA5D5AC, this, 4);
	}

	//	NOTE: for future releases - separate into different inline'd subroutines and put here.
	void Config::Process(LPSTR lpCmdLine, int unk, const char* szGameDir, signed int nIconResId)
	{
		void(__thiscall * _Process)(Config* _this, LPSTR cmdline, int _unk, const char* gamedir, signed int icon) = (void(__thiscall*)(Config*, LPSTR, int, const char*, signed int))0x93D480;

		_Process(this, lpCmdLine, unk, szGameDir, nIconResId);
	}

	//	FIXME: this currently crashes the game!
	//	TODO: implementation! Nothing complicated here really.
	void ReadZipDirectories(char* szFileSystem)
	{
		debug("Call to ReadZipDirectories(\"%s\")\n", szFileSystem);

		void(__cdecl * _ReadZipDirectories)(const char* filesystem) = (void (__cdecl*)(const char*))0x419550;

		_ReadZipDirectories(szFileSystem);
	}
}

inline void PATCH_CONFIG()
{
	//	Override ReadZipDirectories function.
	hook(0x93EE5A, &GameConfig::ReadZipDirectories, PATCH_CALL);
}