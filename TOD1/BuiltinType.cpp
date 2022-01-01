#include "BuiltinType.h"
#include "LogDump.h"
#include "Random.h"
#include "Timer.h"
#include "KapowEngineClass.h"
#include "StreamedSoundBuffers.h"
#include "InputKeyboard.h"
#include "InputMouse.h"
#include "GfxInternal.h"
#include "ScriptDatabase.h"
#include "Window.h"
#include "LoadScreenInfo.h"
#include "AssetManager.h"
#include "VirtualHud.h"
#include "DumpTable.h"
#include "SoundSlot.h"

BuiltinType* tBuiltin;

const Vector4f BuiltinType::ZeroVector = {};
const Vector4f BuiltinType::RightVector = { 1.f, 0.f, 0.f, 0.f };
const Vector4f BuiltinType::UpVector = { 0.f, 1.f, 0.f, 0.f };
const Vector4f BuiltinType::InVector = { 0.f, 0.f, 1.f, 0.f };
const Orientation BuiltinType::Orient = { 0.f, 0.f, 0.f, 1.f };
const Vector4f BuiltinType::LeftVector = { -1.f, 0.f, 0.f, 0.f };
const Vector4f BuiltinType::DownVector = { 0.f, -1.f, 0.f, 0.f };
const Vector4f BuiltinType::OutVector = { 0.f, 0.f, -1.f, 0.f };
const ColorRGB BuiltinType::ColorBlack = { 0.f, 0.f, 0.f, 1.f };
const ColorRGB BuiltinType::ColorRed = { 1.f, 0.f, 0.f, 1.f };
const ColorRGB BuiltinType::ColorGreen = { 0.f, 1.f, 0.f, 1.f };
const ColorRGB BuiltinType::ColorYellow = { 1.f, 1.f, 0.f, 1.f };
const ColorRGB BuiltinType::ColorDarkBlue = { 0.f, 0.f, 1.f, 1.f };
const ColorRGB BuiltinType::ColorPink = { 1.f, 0.f, 1.f, 1.f };
const ColorRGB BuiltinType::ColorBlue = { 0.f, 1.f, 1.f, 1.f };
const ColorRGB BuiltinType::ColorWhite = { 1.f, 1.f, 1.f, 1.f };

void BuiltinType::Sin(float* arg)
{
	*arg = (float)sin(arg[1]);
}

void BuiltinType::Cos(float* arg)
{
	*arg = (float)cos(arg[1]);
}

void BuiltinType::Tan(float* arg)
{
	*arg = (float)tan(arg[1]);
}

void BuiltinType::Asin(float* arg)
{
	*arg = (float)asin(arg[1]);
}

void BuiltinType::Acos(float* arg)
{
	*arg = (float)acos(arg[1]);
}

void BuiltinType::Atan(float* arg)
{
	*arg = (float)atan(arg[1]);
}

void BuiltinType::Abs(int* arg)
{
	*arg = abs(arg[1]);
}

void BuiltinType::Fabs(float* arg)
{
	*arg = (float)fabs(arg[1]);
}

void BuiltinType::Sqrt(float* arg)
{
	*arg = (float)sqrt(arg[1]);
}

void BuiltinType::Floor(float* arg)
{
	*arg = (float)floor(arg[1]);
}

void BuiltinType::Ceil(float* arg)
{
	*arg = (float)ceil(arg[1]);
}

void BuiltinType::Clamp(float* arg)
{
	if (arg[2] <= arg[1])
		*arg = arg[1] <= arg[3] ? arg[1] : arg[3];
	else
		*arg = arg[2];
}

void BuiltinType::Testbits(int* arg)
{
	*arg = (arg[1] & arg[2]) != NULL;
}

void BuiltinType::Setbit(int* arg)
{
	*arg = arg[1] | (1 << arg[2]);
}

void BuiltinType::Getbit(int* arg)
{
	*arg = (arg[1] & (1 << arg[2])) != NULL;
}

void BuiltinType::Rand_seed(int* arg)
{
	Random::Init(*arg);
}

void BuiltinType::Rand_integer(int* arg)
{
	*arg = Random::Integer(arg[1]);
}

void BuiltinType::Rand_number(float* arg)
{
	*arg = Random::Float();
}

void BuiltinType::Get_facecoll_MaterialID(int* arg)
{
	*arg = GetCollmatMaterialId((const char*)*arg);
}

void BuiltinType::GetTime(float* arg)
{
	*arg = Timer::GetMilliseconds() * 0.001f;
}

void BuiltinType::Print(int* arg)
{
	LogDump::Print_Impl("%s", (const char*)*arg);
}

void BuiltinType::IsKeyDown(int* arg)
{
	*arg = g_InputKeyboard->m_Acquired ? g_InputKeyboard->m_ButtonStates_1[arg[1]] >> 7 : NULL;
}

void BuiltinType::IsKeyPressed(int* arg)
{
	*arg = g_InputKeyboard->m_Acquired ? g_InputKeyboard->m_ButtonStates_1[arg[1]] & 1 : NULL;
}

void BuiltinType::IsKeyReleased(int* arg)
{
	*arg = g_InputKeyboard->m_Acquired ? (g_InputKeyboard->m_ButtonStates_1[arg[1]] & 2) != NULL : NULL;
}

void BuiltinType::DrawPoint(int* arg)
{
#ifdef INCLUDE_FIXES
	LogDump::LogA("pBuiltinModule->DrawPoint: (%f, %f, %f) %i\n", (float)arg[1], (float)arg[2], (float)arg[3], arg[4]);
#endif
}

void BuiltinType::DrawLine(int* arg)
{
#ifdef INCLUDE_FIXES
	LogDump::LogA("pBuiltinModule->DrawLine: (%f, %f, %f) (%f, %f, %f) %i\n", (float)arg[1], (float)arg[2], (float)arg[3], (float)arg[4], (float)arg[5], (float)arg[6], arg[7]);
#endif
}

void BuiltinType::DrawLine2D(int* arg)
{
#ifdef INCLUDE_FIXES
	LogDump::LogA("pBuiltinModule->DrawLine2D: (%f, %f, %f) (%f, %f, %f) %i\n", (float)arg[1], (float)arg[2], (float)arg[3], (float)arg[4], (float)arg[5], (float)arg[6], arg[7]);
#endif
}

void BuiltinType::DrawSphere(int* arg)
{
#ifdef INCLUDE_FIXES
	LogDump::LogA("pBuiltinModule->DrawSphere: (%f, %f, %f) %f %i\n", (float)arg[1], (float)arg[2], (float)arg[3], (float)arg[4], arg[5]);
#endif
}

void BuiltinType::ProfileBegin(int* arg)
{
#ifdef INCLUDE_FIXES
	LogDump::LogA("pBuiltinModule->ProfileBegin: \"%s\" %i\n", (const char*)(arg[1]), arg[2]);
#endif
}

void BuiltinType::ProfileEnd(int* arg)
{
#ifdef INCLUDE_FIXES
	LogDump::LogA("pBuiltinModule->ProfileEnd: \"%s\" %i\n", (const char*)(arg[1]), arg[2]);
#endif
}

void BuiltinType::NumberToInteger(void* arg)
{
	*((int*)arg) = (int)(((float*)arg)[1]);
}

void BuiltinType::IntegerToNumber(void* arg)
{
	*((float*)arg) = (float)((int*)arg)[1];
}

void BuiltinType::PrintStack(int* arg)
{
#ifdef INCLUDE_FIXES
	LogDump::LogA("pBuiltinModule->PrintStack: %X\n", arg[1]); // NOTE: arg[1] is of type Entity.
#endif
}

void BuiltinType::GenericCall(int* arg)
{
#ifdef INCLUDE_FIXES
	LogDump::LogA("pBuiltinModule->GenericCall: %X\n", arg[1]); // NOTE: arg[1] is of type Entity.
#endif
}

#pragma message(TODO_IMPLEMENTATION)
void BuiltinType::QuadTreeQuery(int* arg)
{
}

#pragma message(TODO_IMPLEMENTATION)
void BuiltinType::AuxQuadTreeQuery(int* arg)
{
}

void BuiltinType::SetSelectedSoundrenderer(int* arg)
{
	LogDump::LogA("@@@@@@@@@ selected_sound: %d\n", *arg);

	switch (*arg)
	{
	case 1:
		StreamedSoundBuffers::RememberSoundRenderer(SOUND_SYSTEM_DIESELPOWER);
		break;
	case 2:
		StreamedSoundBuffers::RememberSoundRenderer(SOUND_SYSTEM_DSOUND);
		break;
	case 3:
		StreamedSoundBuffers::RememberSoundRenderer(SOUND_SYSTEM_UNDEFINED); // FIXME: this should be 0, but no enum for it right now.
		break;
	default:
		StreamedSoundBuffers::RememberSoundRenderer(SOUND_SYSTEM_AUTOSELECT);
		break;
	}
}

void BuiltinType::SfxMuteAll(int* arg)
{
	g_StreamedSoundBuffers->m_Muted = *arg == 1;
}

void BuiltinType::SfxIsMuteAll(int* arg)
{
	*arg = (int)g_StreamedSoundBuffers->m_Muted;
}

void BuiltinType::AllocateGlobalStreamedSound(int* arg)
{
	*arg = SoundSlot::AllocateGlobalStreamedSound((const char*)(arg[1]), arg[2] != NULL, arg[3] != NULL);
}

void BuiltinType::DeallocateGlobalStreamedSound(int* arg)
{
	SoundSlot::DeallocateGlobalStreamedSound();
}

#pragma message(TODO_IMPLEMENTATION)
void BuiltinType::PlayGlobalStreamedSound(int* arg)
{
	//*arg = Audio::PlayGlobalStreamedSound();
}

#pragma message(TODO_IMPLEMENTATION)
void BuiltinType::StopGlobalStreamedSound(int* arg)
{
	//*arg = Audio::StopGlobalStreamedSound();
}

#pragma message(TODO_IMPLEMENTATION)
void BuiltinType::SetVolumePitchGlobalStreamedSound(int* arg)
{
	//*arg = Audio::SetVolumePitchGlobalStreamedSound((float)arg[1], (float)arg[2]);
}

void BuiltinType::GetDefaultFxVolumeVar(float* arg)
{
	*arg = 1.0f;
}

void BuiltinType::GetDefaultAmbienceVolumeVar(float* arg)
{
	*arg = 1.0f;
}

void BuiltinType::GetDefaultMusicVolumeVar(float* arg)
{
	*arg = 1.0f;
}

void BuiltinType::GetDefaultSpeaksVolumeVar(float* arg)
{
	*arg = 1.0f;
}

#pragma message(TODO_IMPLEMENTATION)
void BuiltinType::SetVolModifierOnGroup(int* arg)
{
}

void BuiltinType::CutsceneDisableAware(int* arg)
{
	*arg = Script::CutsceneDisableAware;
}

void BuiltinType::IsDebugConsoleActive(int* arg)
{
#ifdef INCLUDE_FIXES
	LogDump::LogA("pBuiltinModule->IsDebugConsoleActive\n");
#else
	* arg = NULL;
#endif
}

void BuiltinType::DebugConsoleTextBox(int* arg)
{
#ifdef INCLUDE_FIXES
	LogDump::LogA("pBuiltinModule->DebugConsoleTextBox: %i %i %i %i %i\n", arg[1], arg[2], arg[3], arg[4], arg[5]);
#endif
}

void BuiltinType::DebugConsolePrint(int* arg)
{
#ifdef INCLUDE_FIXES
	LogDump::LogA("pBuiltinModule->DebugConsolePrint: \"%s\"\n", (const char*)arg[1]);
#endif
}

#pragma message(TODO_IMPLEMENTATION)
void BuiltinType::GlobalKillAllEmmiters(int* arg)
{
	//ParticleSystem::KillEmmiters(false, false);
}

void BuiltinType::GetVersionNumber(char** arg)
{
	char procname[64] = {};
	HANDLE prochnd = NULL;

	sprintf(procname, "v7_%04d", GetCurrentProcessId() ^ 0x19EA3FD3);
	prochnd = OpenEvent(EVENT_MODIFY_STATE, FALSE, procname);
	*arg = new char[6]; // TODO: who deletes this?

	if (prochnd)
	{
		CloseHandle(prochnd);
		strcpy(*arg, "v1.00");
	}
	else
		strcpy(*arg, "v1,00");
}

void BuiltinType::GetConfigString(int* arg)
{
	if (g_KapowEngine->m_ConfigurationVariables->IsVariableSet((const char*)arg[1]))
	{
		String configstr;
		g_KapowEngine->m_ConfigurationVariables->GetParamValueString(configstr, (const char*)arg[1]);
		*arg = (int)_strdup(configstr.m_Str);
	}
	else
		*arg = NULL;
}

void BuiltinType::GetConfigTruth(int* arg)
{
	*arg = g_KapowEngine->m_ConfigurationVariables->IsVariableSet((const char*)arg[1]) ? g_KapowEngine->m_ConfigurationVariables->GetParamValueBool((const char*)arg[1]) : 0;
}

void BuiltinType::GetSessionVariableString(int* arg)
{
	if (g_KapowEngine->m_SessionVariables->IsVariableSet((const char*)arg[1]))
	{
		String sessionstr;
		g_KapowEngine->m_SessionVariables->GetParamValueString(sessionstr, (const char*)arg[1]);
		*arg = (int)_strdup(sessionstr.m_Str);
	}
	else
		*arg = NULL;
}

void BuiltinType::SetSessionVariableString(int* arg)
{
	g_KapowEngine->m_SessionVariables->SetParamValue((const char*)arg[0], (const char*)arg[1]);
}

void BuiltinType::GetSessionVariableTruth(char* arg)
{
	*arg = g_KapowEngine->m_SessionVariables->IsVariableSet((const char*)arg[1]) ? g_KapowEngine->m_SessionVariables->GetParamValueBool((const char*)arg[1]) : *arg = false;
}

void BuiltinType::SetSessionVariableTruth(int* arg)
{
	g_KapowEngine->m_SessionVariables->SetParamValueBool((const char*)arg[0], (bool)arg[1]);
}

void BuiltinType::SetCurrentCountryCode(char* arg)
{
	Script::SetCountryCode((char*)*arg);
}

void BuiltinType::GetCurrentCountryCode(char* arg)
{
	*(char**)(&(*arg)) = new char[4];
	strcpy(*(char**)(&(*arg)), Script::GetCurrentCountryCode());
}

void BuiltinType::SetDiscErrorMessage(int* arg)
{
#ifdef INCLUDE_FIXES
	LogDump::LogA("pBuiltinModule->SetDiscErrorMessage: %p %i\n", *arg, arg[1]);
#endif
}

void BuiltinType::SetLodFactor(float* arg)
{
	Script::LodFactor = *arg;
}

void BuiltinType::LoadScene(char** arg)
{
	if (!*arg || !**arg || !strlen(*arg))
		return;

	g_KapowEngine->m_SceneName = *arg;
}

void BuiltinType::GetSystemLanguage(int* arg)
{
	*arg = GetSystemLanguageCode();
}

void BuiltinType::StartCleanupDashboard(int* arg)
{
#ifdef INCLUDE_FIXES
	LogDump::LogA("pBuiltinModule->StartCleanupDashboard: %i\n", *arg);
#endif
}

#pragma message(TODO_IMPLEMENTATION)
void BuiltinType::SetScreenResolution(int* arg)
{
	g_GfxInternal_Dx9->SetScreenResolution(*arg, arg[1]);
	//g_Blocks->ResetSceneChildrenNodes(NULL);
}

void BuiltinType::GetScreenResolution(float* arg)
{
	ScreenResolution res;
	g_GfxInternal->GetScreenResolution(res);

	*arg = (float)res.x;
	arg[1] = (float)res.y;
	arg[2] = 0.f; // NOTE: why?
}

void BuiltinType::IsScreenResolutionAvailable(int* arg)
{
	*arg = g_GfxInternal->IsScreenResolutionAvailable(arg[1], arg[2]);
}

void BuiltinType::IsWideScreen(int* arg)
{
	*arg = GfxInternal::IsWideScreen();
}

void BuiltinType::SetVirtualHudScreenSize(float* arg)
{
	VirtualHud::VirtualHudInstance.SetVirtualHudScreenSize(*arg, arg[1], 1.f, 1.f);
}

void BuiltinType::GetVirtualHudScreenSize(float* arg)
{
	*arg = VirtualHud::VirtualHudInstance.m_VirtualHudScreensizeWidth;
	arg[1] = VirtualHud::VirtualHudInstance.m_VirtualHudScreensizeHeight;
	arg[2] = 0.f;
}

void BuiltinType::GetScreenTopInVirtualUnits(float* arg)
{
	*arg = VirtualHud::VirtualHudInstance.GetScreenTopInVirtualUnits();
}

void BuiltinType::GetScreenBottomInVirtualUnits(float* arg)
{
	*arg = VirtualHud::VirtualHudInstance.GetScreenBottomInVirtualUnits();
}

void BuiltinType::GetScreenLeftInVirtualUnits(float* arg)
{
	*arg = VirtualHud::VirtualHudInstance.GetScreenLeftInVirtualUnits();
}

void BuiltinType::GetScreenRightInVirtualUnits(float* arg)
{
	*arg = VirtualHud::VirtualHudInstance.GetScreenRightInVirtualUnits();
}

void BuiltinType::DisableCurrentLoadScreen(int* arg)
{
	g_LoadScreenInfo->Deactivate();
}

void BuiltinType::GetEditorActive(bool* arg)
{
#ifdef INCLUDE_FIXES
	debug("pBuiltinModule->GetEditorActive: %c\n", false);
#endif

	*arg = false;
}

void BuiltinType::DumptableCreate(int* arg)
{
	*arg = DumpTable.size();

	DumpTable.push_back(new DumpTableDescription);
}

#pragma message(TODO_IMPLEMENTATION)
void BuiltinType::DumptableCreateFromFile(int* arg)
{
	/*
	String buff;
	File dtfile((const char*)*arg, 1, true);

	while (!dtfile.ReadIfNotEOF())
	{
	 char buf[5] = {};
	 dtfile.Read(buf, sizeof(buf) - 1);
	 buff.Append(buf);
	}

	DumpTable_Element dtel(buff.m_szString);
	DumpTable.AddElement(&dtel);

	*arg = DumpTable.m_CurrIndex;
	*/
}

void BuiltinType::DumptableAddIntegerColumn(int* arg)
{
	DumpTable[*arg]->AddIntegerColumn((const char*)arg[1], nullptr, 10, 0, 0);
}

void BuiltinType::DumptableAddNumberColumn(int* arg)
{
	DumpTable[*arg]->AddNumberColumn((const char*)arg[1], nullptr, 10, -1, 0);
}

void BuiltinType::DumptableAddStringColumn(int* arg)
{
	DumpTable[*arg]->AddStringColumn((const char*)arg[1], nullptr, 10, 0);
}

void BuiltinType::DumptableSetNumRows(int* arg)
{
	DumpTable[*arg]->SetNumRows(arg[1]);
}

void BuiltinType::DumptableSetIntegerValue(int* arg)
{
	DumpTable[*arg]->SetIntegerValue(arg[1], arg[2], arg[3]);
}

void BuiltinType::DumptableSetNumberValue(int* arg)
{
	DumpTable[*arg]->SetNumberValue(arg[1], arg[2], (float)arg[3]);
}

void BuiltinType::DumptableSetStringValue(int* arg)
{
	DumpTable[*arg]->SetStringValue(arg[1], arg[2], (const char*)arg[3]);
}

void BuiltinType::DumptableWriteToFile(int* arg)
{
	String dumpstr;
	DumpTable[*arg]->DumpContents(dumpstr, -1, -1, -1, 0);

	File dumpfile((const char*)arg[1], 2, true);
	dumpfile.WriteBuffer(dumpstr.m_Str);
}

void BuiltinType::DumptableClose(int* arg)
{
	if (DumpTable[*arg])
		delete DumpTable[*arg];
}

void BuiltinType::EditorReloadAllAssets(int* arg)
{
#ifdef INCLUDE_FIXES
	LogDump::LogA("pBuiltinModule->EditorReloadAllAssets: %s\n", (const char*)*arg);
#endif
}

void BuiltinType::EditorSelectNode(int* arg)
{
#ifdef INCLUDE_FIXES
	LogDump::LogA("pBuiltinModule->EditorSelectNode: %p\n", *arg);
#endif
}

void BuiltinType::GetRegion(int* arg)
{
	*arg = g_AssetManager->GetRegion();
}

void BuiltinType::GetMessageId(int* arg)
{
	*arg = GetCommandByName((const char*)arg[1]);
}

void BuiltinType::QuitGame(int* arg)
{
	g_Window->QuitGame();
}

void BuiltinType::GetCoverdemoPlayMode(int* arg)
{
	*arg = g_Window->GetCoverdemoPlayMode();
}

void BuiltinType::GetCoverdemoInactiveTimeoutSec(int* arg)
{
	*arg = g_Window->GetCoverdemoInactiveTimeoutSec();
}

void BuiltinType::GetCoverdemoGameplayTimeoutSec(int* arg)
{
	*arg = g_Window->GetCoverdemoGameplayTimeoutSec();
}

void BuiltinType::CoverdemoExit(int*)
{
	exit(0);
}