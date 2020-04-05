#include "Config.h"

#include "Types.h"
#include "Globals.h"

#include "MemoryAllocators.h"
#include "Blocks.h"
#include "ScriptTypes.h"

GameConfig::Config* g_Config = NULL;

namespace GameConfig {

	void Config::Init()
	{
		patch(0xA5D5AC, this, 4);
	}

	void Config::Process(LPSTR lpCmdLine, int unk, const char* szConfigFilename, signed int nIconResId)
	{
		CoInitialize(0);

		//	Set filename for configuration file.
		if (szConfigFilename && *szConfigFilename)
			m_sConfigFilePath = String(szConfigFilename);
		else
			m_sConfigFilePath = String("/configpc.txt");

		//	Set gamename.
		m_sGameName = String(CONFIG_GAMENAME);

		Session_Variables* pVariables;

		//	Try and look for configuration variables file.
		if (!Utils::FindFileEverywhere(m_sConfigFilePath.m_szString)) {
			if (!Allocators::Released) {
				pVariables = (Session_Variables*)Allocators::AllocatorsList->ALLOCATOR_DEFAULT->allocate(sizeof(Session_Variables));

				if (pVariables)
					pVariables->CreateBuffer(0);
			}
		}else{
			debug("Initialising engine with '%s'\n", m_sConfigFilePath.m_szString);

			if (!Allocators::Released) {
				pVariables = (Session_Variables*)Allocators::AllocatorsList->ALLOCATOR_DEFAULT->allocate(sizeof(Session_Variables));

				if (pVariables)
					pVariables->CreateBuffer(m_sConfigFilePath.m_szString, 1);
			}
		}

		m_pConfigurationVariables = pVariables;

		Session_Variables* pProfileVariables;

		//	Try and look for profile variables file.
		if (Utils::FindFileEverywhere("/profile.txt")) {
			if (!Allocators::Released) {
				pProfileVariables = (Session_Variables*)Allocators::AllocatorsList->ALLOCATOR_DEFAULT->allocate(sizeof(Session_Variables));

				if (pProfileVariables)
					pProfileVariables->CreateBuffer("/profile.txt", 0);
			}
		}

		if (m_pConfigurationVariables->IsVariableSet("filecheck"))
			Script::FileCheck = m_pConfigurationVariables->IsVariableValueTrue("filecheck") == 0;

		if (m_pConfigurationVariables->IsVariableSet("control_type"))
			Script::ControlType = m_pConfigurationVariables->GetParamValue<String>("control_type");

		if (m_pConfigurationVariables->IsVariableSet("forcefeedback"))
			Script::ForceFeedback = m_pConfigurationVariables->IsVariableValueTrue("forcefeedback");

		if (m_pConfigurationVariables->IsVariableSet("loadblocks"))
			Script::LoadBlocks = m_pConfigurationVariables->IsVariableValueTrue("loadblocks");

		//	Try and initialize Blocks class.
		if (!Allocators::Released)
			if (g_Blocks = (Blocks*)Allocators::AllocatorsList->ALLOCATOR_DEFAULT->allocate(sizeof(Blocks)))
				g_Blocks->Init(Script::LoadBlocks);

		//	Init script types.
		//	Init scratchpad.
		//	Init scene buffers, resource types.
		ScriptTypes::Init();

		//	TODO: finish.
	}

	Session_Variables* Session_Variables::CreateBuffer(int unk)
	{
		Session_Variables* (__thiscall * _CreateBuffer)(Session_Variables * _this, int unk) = (Session_Variables * (__thiscall*)(Session_Variables*, int))0x410680;

		return _CreateBuffer(this, unk);
	}

	Session_Variables* Session_Variables::CreateBuffer(const char* szPath, bool unk)
	{
		Session_Variables* (__thiscall * _CreateBuffer)(Session_Variables * _this, const char* _path, bool _unk) = (Session_Variables * (__thiscall*)(Session_Variables*, const char*, bool))0x4124D0;

		return _CreateBuffer(this, szPath, unk);
	}

	bool Session_Variables::IsVariableSet(const char* variableName)
	{
		bool(__thiscall * _IsVariableSet)(Session_Variables* _this, const char* _varname) = (bool (__thiscall*)(Session_Variables*, const char*))0x410080;
		
		return _IsVariableSet(this, variableName);
	}

	bool Session_Variables::IsVariableValueTrue(const char* variableName)
	{
		bool(__thiscall * _IsVariableValueTrue)(Session_Variables * _this, const char* _varname) = (bool(__thiscall*)(Session_Variables*, const char*))0x410900;
		
		return _IsVariableValueTrue(this, variableName);
	}

	template <typename T>
	T Session_Variables::GetParamValue(const char* variableName)
	{
		return 0;
	}

	template <>
	int Session_Variables::GetParamValue<int>(const char* variableName)
	{
		int(__thiscall * _GetParamInt)(Session_Variables * _this, const char* _varname) = (int(__thiscall*)(Session_Variables*, const char*))0x410A30;

		return _GetParamInt(this, variableName);
	}

	template <>
	float Session_Variables::GetParamValue<float>(const char* variableName)
	{
		float(__thiscall * _GetParamFloat)(Session_Variables * _this, const char* _varname) = (float(__thiscall*)(Session_Variables*, const char*))0x410AC0;

		return _GetParamFloat(this, variableName);
	}

	template <>
	int* Session_Variables::GetParamValue<int*>(const char* variableName)
	{
		int*(__thiscall * _GetParamIntArr)(Session_Variables * _this, const char* _varname) = (int*(__thiscall*)(Session_Variables*, const char*))0x410B50;

		return _GetParamIntArr(this, variableName);
	}

	template <>
	const Vector2<float>& Session_Variables::GetParamValue<const Vector2<float>&>(const char* variableName)
	{
		const Vector2<float>&(__thiscall * _GetParamVec2f)(Session_Variables * _this, const char* _varname) = (const Vector2<float>&(__thiscall*)(Session_Variables*, const char*))0x410BE0;

		return _GetParamVec2f(this, variableName);
	}

	template <>
	const Vector4f& Session_Variables::GetParamValue<const Vector4f&>(const char* variableName)
	{
		const Vector4f&(__thiscall * _GetParamVec4)(Session_Variables * _this, const char* _varname) = (const Vector4f&(__thiscall*)(Session_Variables*, const char*))0x410C70;

		return _GetParamVec4(this, variableName);
	}

	template <>
	const ColorRGB& Session_Variables::GetParamValue<const ColorRGB&>(const char* variableName)
	{
		const ColorRGB&(__thiscall * _GetParamColor)(Session_Variables * _this, const char* _varname) = (const ColorRGB&(__thiscall*)(Session_Variables*, const char*))0x410D90;

		return _GetParamColor(this, variableName);
	}

	template <>
	const String& Session_Variables::GetParamValue<const String&>(const char* variableName)
	{
		const String& (__thiscall * _GetParamString)(Session_Variables * _this, const char* _varname) = (const String& (__thiscall*)(Session_Variables*, const char*))0x410E30;

		return _GetParamString(this, variableName);
	}
	/*
	 *	int @410A30,
	 *	float @410AC0,
	 *	array[2] @410B50,
	 *	vector2 @410BE0,
	 *	vector4 @410C70,
	 *	vector4(color?) @410D90,
	 *	string @410E30
	*/

	void Session_Variables::SetParamValue(const char* variableName, char* value)
	{
		void(__thiscall * _SetParamValue)(Session_Variables * _this, const char* _varname, char* _val) = (void(__thiscall*)(Session_Variables*, const char*, char*))0x4114E0;

		_SetParamValue(this, variableName, value);
	}

	void Session_Variables::SetParamValueBool(const char* variableName, int value)
	{
		void(__thiscall * _SetParamValueBool)(Session_Variables * _this, const char* _varname, int _val) = (void(__thiscall*)(Session_Variables*, const char*, int))0x4116D0;

		_SetParamValueBool(this, variableName, value);
	}

	void ReadZipDirectories(const char* szFileSystem)
	{
		void(__cdecl * _OpenZip)(const char* szName) = (void(__cdecl*)(const char*))0x419100;

		if (szFileSystem && *szFileSystem) {
			int nCurrentArchiveIndex = 0;
			char szZipName[255];

			memset(&szZipName, 0, sizeof(szZipName));

			while (true) {
				int nCharIndex = 0;
				do {
					char cCurrentChar = szFileSystem[nCurrentArchiveIndex];

					if (!cCurrentChar)
						break;
					if (cCurrentChar == ' ' || cCurrentChar == ',')
						break;
				} while (nCharIndex < 255);

				szZipName[nCharIndex] = 0;

				while (true) {
					char cCurrentChar = szFileSystem[nCurrentArchiveIndex];

					if (cCurrentChar != ' ' && cCurrentChar != ',')
						break;

					nCurrentArchiveIndex++;
				}

				_OpenZip(szZipName);

				String sZipNameLocalised = String(strlen(szZipName) + strlen("_") + strlen(Script::LanguageMode.m_szString));

				strcpy(sZipNameLocalised.m_szString, Script::LanguageMode.m_szString);
				strcat(sZipNameLocalised.m_szString, "_");
				strcat(sZipNameLocalised.m_szString, szZipName);

				_OpenZip(sZipNameLocalised.m_szString);

				if (!szFileSystem[nCurrentArchiveIndex])
					break;

				memset(&szZipName, 0, sizeof(szZipName));
			}
		}
	}
}

inline void PATCH_CONFIG()
{
	void* dwFunc;

	//	Override ReadZipDirectories function.
	hook(0x93EE5A, &GameConfig::ReadZipDirectories, PATCH_CALL);

	_asm	mov		eax, offset GameConfig::Config::Process
	_asm	mov		dwFunc, eax
	//	Override Config::Process function.
	hook(0x93F6C0, dwFunc, PATCH_NOTHING);
}