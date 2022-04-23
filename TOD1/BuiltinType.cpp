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
#include "IntegerType.h"
#include "NumberType.h"
#include "VectorType.h"
#include "Scene.h"
#include "TruthType.h"
#include "ScriptThread.h"

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

BuiltinType::BuiltinType() : EntityType("builtin")
{
    MESSAGE_CLASS_CREATED(BuiltinType);
}

BuiltinType::~BuiltinType()
{
    MESSAGE_CLASS_DESTROYED(BuiltinType);

    tBuiltin = nullptr;
}

void BuiltinType::RegisterMemberFunction(DataType* returntype, const char* membername, Getter getter, Setter setter, const char* memberproto, const char* a6)
{
    char funcproto[128] = {};
    sprintf(funcproto, "%s:%s", membername, returntype->m_TypeName.m_Str);
    const int registeredtype = RegisterGlobalProperty(funcproto, true);

    BuiltinMember member(returntype, getter, setter, memberproto, a6);
    m_MembersMap.insert({registeredtype, member});
}

void BuiltinType::RegisterHandler(const char* handlerSignature, BuiltinTypeMemberFunction handlerPtr, const char* handlerHelperMessage)
{
    BuiltinHandler handler(handlerSignature, handlerPtr, handlerHelperMessage);
    m_HandlersList.push_back(handler);
}

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

void BuiltinType::PlayGlobalStreamedSound(int* arg)
{
    *arg = SoundSlot::PlayGlobalStreamedSound();
}

void BuiltinType::StopGlobalStreamedSound(int* arg)
{
    *arg = SoundSlot::StopGlobalStreamedSound();
}

void BuiltinType::SetVolumePitchGlobalStreamedSound(int* arg)
{
    *arg = SoundSlot::SetVolumePitchGlobalStreamedSound((float)arg[1], (float)arg[2]);
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

void BuiltinType::SetScreenResolution(int* arg)
{
    g_GfxInternal_Dx9->SetScreenResolution(*arg, arg[1]);
    g_AssetManager->DestroySceneNodesFrameBuffers(0);
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

const unsigned int BuiltinType::GetGameTimeMs() const
{
    return Scene::GameTimeMs;
}

const unsigned int BuiltinType::GetRealTimeMs() const
{
    return Scene::RealTimeMs;
}

const float BuiltinType::GetGameTime() const
{
    return (float)Scene::GameTimeMs * 0.001f;
}

const float BuiltinType::GetRealTime() const
{
    return (float)Scene::RealTimeMs * 0.001f;
}

const float BuiltinType::GetTimePassed() const
{
    return Scene::TimePassed;
}

const float BuiltinType::GetRealTimePassed() const
{
    return Scene::RealTimePassed;
}

const float BuiltinType::GetPI() const
{
    return 3.1415927f;
}

void BuiltinType::GetInVector(Vector4f& outVec) const
{
    outVec = InVector;
}

void BuiltinType::GetOutVector(Vector4f& outVec) const
{
    outVec = OutVector;
}

void BuiltinType::GetRightVector(Vector4f& outVec) const
{
    outVec = RightVector;
}

void BuiltinType::GetLeftVector(Vector4f& outVec) const
{
    outVec = LeftVector;
}

void BuiltinType::GetUpVector(Vector4f& outVec) const
{
    outVec = UpVector;
}

void BuiltinType::GetDownVector(Vector4f& outVec) const
{
    outVec = DownVector;
}

void BuiltinType::GetZeroVector(Vector4f& outVec) const
{
    outVec = ZeroVector;
}

bool BuiltinType::GetWarnDelayedException() const
{
    return ScriptThread::WarnDelayedException;
}

void BuiltinType::SetWarnDelayedException(const bool warn)
{
    ScriptThread::WarnDelayedException = warn;
}

#pragma message(TODO_IMPLEMENTATION)
int BuiltinType::GetSoundRenderer() const
{
    return 0;
}

void BuiltinType::Register()
{
    tBuiltin = new BuiltinType();

    tBuiltin->RegisterHandler("sin(number):number", (BuiltinTypeMemberFunction)&Sin, "sin_handler");
    tBuiltin->RegisterHandler("cos(number):number", (BuiltinTypeMemberFunction)&Cos, "cos_handler");
    tBuiltin->RegisterHandler("tan(number):number", (BuiltinTypeMemberFunction)&Tan, "tan_handler");
    tBuiltin->RegisterHandler("asin(number):number", (BuiltinTypeMemberFunction)&Asin, "asin_handler");
    tBuiltin->RegisterHandler("acos(number):number", (BuiltinTypeMemberFunction)&Acos, "acos_handler");
    tBuiltin->RegisterHandler("atan(number):number", (BuiltinTypeMemberFunction)&Atan, "atan_handler");
    tBuiltin->RegisterHandler("abs(integer):integer", (BuiltinTypeMemberFunction)&Abs, "abs_handler");
    tBuiltin->RegisterHandler("fabs(number):number", (BuiltinTypeMemberFunction)&Fabs, "fabs_handler");
    tBuiltin->RegisterHandler("sqrt(number):number", (BuiltinTypeMemberFunction)&Sqrt, "sqrt_handler");
    tBuiltin->RegisterHandler("floor(number):number", (BuiltinTypeMemberFunction)&Floor, "floor_handler");
    tBuiltin->RegisterHandler("ceil(number):number", (BuiltinTypeMemberFunction)&Ceil, "ceil_handler");
    tBuiltin->RegisterHandler("clamp(number,number,number):number", (BuiltinTypeMemberFunction)&Clamp, "clamp_handler");
    tBuiltin->RegisterHandler("testbits(integer,integer):truth", (BuiltinTypeMemberFunction)&Testbits, "testbits_handler");
    tBuiltin->RegisterHandler("setbit(integer,integer):integer", (BuiltinTypeMemberFunction)&Setbit, "setbit_handler");
    tBuiltin->RegisterHandler("getbit(integer,integer):truth", (BuiltinTypeMemberFunction)&Getbit, "getbit_handler");
    tBuiltin->RegisterHandler("rand_seed(integer)", (BuiltinTypeMemberFunction)&Rand_seed, "rand_seed");
    tBuiltin->RegisterHandler("rand_integer(integer):integer", (BuiltinTypeMemberFunction)&Rand_integer, "rand_integer");
    tBuiltin->RegisterHandler("rand_number:number", (BuiltinTypeMemberFunction)&Rand_number, "rand_number");
    tBuiltin->RegisterHandler("get_facecoll_materialID(string):integer", (BuiltinTypeMemberFunction)&Get_facecoll_MaterialID, "GetFaceCollMaterialIDMSG");
    tBuiltin->RegisterHandler("gettime:number", (BuiltinTypeMemberFunction)&GetTime, "GetTime_handler");
    tBuiltin->RegisterHandler("print(string)", (BuiltinTypeMemberFunction)&Print, "print_handler");
    tBuiltin->RegisterHandler("iskeydown(integer):truth", (BuiltinTypeMemberFunction)&IsKeyDown, "IsKeyDown");
    tBuiltin->RegisterHandler("iskeypressed(integer):truth", (BuiltinTypeMemberFunction)&IsKeyPressed, "IsKeyPressed");
    tBuiltin->RegisterHandler("iskeyreleased(integer):truth", (BuiltinTypeMemberFunction)&IsKeyReleased, "IsKeyReleased");
    tBuiltin->RegisterHandler("drawpoint(vector,integer)", (BuiltinTypeMemberFunction)&DrawPoint, "DrawPoint");
    tBuiltin->RegisterHandler("drawline(vector,vector,integer)", (BuiltinTypeMemberFunction)&DrawLine, "DrawLine");
    tBuiltin->RegisterHandler("drawline2d(vector,vector,integer)", (BuiltinTypeMemberFunction) &DrawLine2D, "DrawLine2D");
    tBuiltin->RegisterHandler("drawsphere(vector,number,integer)", (BuiltinTypeMemberFunction) &DrawSphere, "DrawSphere");
    tBuiltin->RegisterHandler("profilebegin(string,integer)", (BuiltinTypeMemberFunction)&ProfileBegin, "ProfileBegin");
    tBuiltin->RegisterHandler("profileend(string,integer)", (BuiltinTypeMemberFunction)&ProfileEnd, "ProfileEnd");
    tBuiltin->RegisterHandler("c_int(number):integer", (BuiltinTypeMemberFunction)&NumberToInteger, "NumberToInteger");
    tBuiltin->RegisterHandler("c_num(integer):number", (BuiltinTypeMemberFunction)&IntegerToNumber, "IntegerToNumber");
    tBuiltin->RegisterHandler("printstack(entity)", (BuiltinTypeMemberFunction)&PrintStack, "PrintStack");
    tBuiltin->RegisterHandler("genericcall(entity)", (BuiltinTypeMemberFunction)&GenericCall, "GenericCallMSG");
    DataType::LoadScript("list(entity)");
    tBuiltin->RegisterHandler("quadtreequery(vector,number,integer,truth):list(entity)", (BuiltinTypeMemberFunction)&QuadTreeQuery, "QuadTreeQueryMSG");
    tBuiltin->RegisterHandler("auxquadtreequery(vector,number,integer,truth):list(entity)", (BuiltinTypeMemberFunction)&AuxQuadTreeQuery, "AuxQuadTreeQueryMSG");

    tBuiltin->RegisterMemberFunction(tINTEGER, "gametime_ms", (Getter)&GetGameTimeMs, 0, "iGameTime", 0);
    tBuiltin->RegisterMemberFunction(tINTEGER, "realtime_ms", (Getter)&GetRealTimeMs, 0, "iRealTime", 0);
    tBuiltin->RegisterMemberFunction(tNUMBER, "gametime", (Getter)&GetGameTime, 0, "(float)iGameTime/1000.f", 0);
    tBuiltin->RegisterMemberFunction(tNUMBER, "realtime", (Getter)&GetRealTime, 0, "(float)iRealTime/1000.f", 0);
    tBuiltin->RegisterMemberFunction(tNUMBER, "timepassed", (Getter)&GetTimePassed, 0, "fTimePassed", 0);
    tBuiltin->RegisterMemberFunction(tNUMBER, "gametimepassed", (Getter)&GetTimePassed, 0, "fTimePassed", 0);
    tBuiltin->RegisterMemberFunction(tNUMBER, "realtimepassed", (Getter)&GetRealTimePassed, 0, "fRealTimePassed", 0);
    tBuiltin->RegisterMemberFunction(tNUMBER, "pi", (Getter)&GetPI, 0, "MathLib::PI", 0);
    tBuiltin->RegisterMemberFunction(tVECTOR, "in_vector", (Getter)&GetInVector, 0, "pBuiltinModule->GetInVector()", 0);
    tBuiltin->RegisterMemberFunction(tVECTOR, "out_vector", (Getter)&GetOutVector, 0, "pBuiltinModule->GetOutVector()", 0);
    tBuiltin->RegisterMemberFunction(tVECTOR, "right_vector", (Getter)&GetRightVector, 0, "pBuiltinModule->GetRightVector()", 0);
    tBuiltin->RegisterMemberFunction(tVECTOR, "left_vector", (Getter)&GetLeftVector, 0, "pBuiltinModule->GetLeftVector()", 0);
    tBuiltin->RegisterMemberFunction(tVECTOR, "up_vector",(Getter)&GetUpVector, 0,"pBuiltinModule->GetUpVector()", 0);
    tBuiltin->RegisterMemberFunction(tVECTOR, "down_vector",(Getter)&GetDownVector, 0,"pBuiltinModule->GetDownVector()", 0);
    tBuiltin->RegisterMemberFunction(tVECTOR, "zero_vector",(Getter)&GetZeroVector, 0,"pBuiltinModule->GetZeroVector()", 0);
    tBuiltin->RegisterMemberFunction(tTRUTH, "WarnDelayedException", (Getter)&GetWarnDelayedException, (Setter)&SetWarnDelayedException, 0, 0);
    tBuiltin->RegisterMemberFunction(tINTEGER, "selected_soundrenderer", (Getter)&GetSoundRenderer, 0, "pBuiltinModule->GetSoundRenderer()", 0);

    tBuiltin->RegisterHandler("set_selected_soundrenderer(integer)", (BuiltinTypeMemberFunction)&SetSelectedSoundrenderer, "SetSoundRendererMSG");
    tBuiltin->RegisterHandler("sfx_muteall(truth)", (BuiltinTypeMemberFunction)&SfxMuteAll, "SfxMuteAll");
    tBuiltin->RegisterHandler("sfx_ismuteall:truth", (BuiltinTypeMemberFunction)&SfxIsMuteAll, "SfxIsMuteAll");
    tBuiltin->RegisterHandler("allocate_global_streamed_sound(string,truth,truth):truth", (BuiltinTypeMemberFunction)&AllocateGlobalStreamedSound, "AllocateGlobalStreamedSound");
    tBuiltin->RegisterHandler("deallocate_global_streamed_sound", (BuiltinTypeMemberFunction)&DeallocateGlobalStreamedSound,"DeallocateGlobalStreamedSound");
    tBuiltin->RegisterHandler("play_global_streamed_sound:truth", (BuiltinTypeMemberFunction)&PlayGlobalStreamedSound, "PlayGlobalStreamedSound");
    tBuiltin->RegisterHandler("stop_global_streamed_sound:truth", (BuiltinTypeMemberFunction)&StopGlobalStreamedSound, "StopGlobalStreamedSound");
    tBuiltin->RegisterHandler("set_volume_pitch_global_streamed_sound(number,number):truth", (BuiltinTypeMemberFunction)&SetVolumePitchGlobalStreamedSound, "SetVolumePitchGlobalStreamedSound");
    tBuiltin->RegisterHandler("get_default_fx_volume_var:number", (BuiltinTypeMemberFunction)&GetDefaultFxVolumeVar, "GetDefaultFXVolumeVar");
    tBuiltin->RegisterHandler("get_default_ambience_volume_var:number", (BuiltinTypeMemberFunction)&GetDefaultAmbienceVolumeVar, "GetDefaultAmbienceVolumeVar");
    tBuiltin->RegisterHandler("get_default_music_volume_var:number", (BuiltinTypeMemberFunction)&GetDefaultMusicVolumeVar, "GetDefaultMusicVolumeVar");
    tBuiltin->RegisterHandler("get_default_speaks_volume_var:number", (BuiltinTypeMemberFunction)&GetDefaultSpeaksVolumeVar, "GetDefaultSpeaksVolumeVar");
    tBuiltin->RegisterHandler("set_vol_modifier_on_group(integer,number)", (BuiltinTypeMemberFunction)&SetVolModifierOnGroup, "SetVolModifierOnGroup");
    tBuiltin->RegisterHandler("cutscene_disable_aware:truth", (BuiltinTypeMemberFunction)&CutsceneDisableAware, "CutsceneDisableAware");
    tBuiltin->RegisterHandler("dc_isactive:truth", (BuiltinTypeMemberFunction)&IsDebugConsoleActive, "IsDebugConsoleActive");
    tBuiltin->RegisterHandler("dc_textbox(integer,integer,integer,integer,integer)", (BuiltinTypeMemberFunction)&DebugConsoleTextBox, "DebugConsoleTextBox");
    tBuiltin->RegisterHandler("dc_print(string)", (BuiltinTypeMemberFunction)&DebugConsolePrint, "DebugConsolePrint");
    tBuiltin->RegisterHandler("global_kill_all_emitters", (BuiltinTypeMemberFunction)&GlobalKillAllEmmiters, "GlobalKillAllEmittersMSG");
    tBuiltin->RegisterHandler("getversionnumber:string", (BuiltinTypeMemberFunction)&GetVersionNumber, "GetVersionNumberMSG");
    tBuiltin->RegisterHandler("getconfigstring(string):string", (BuiltinTypeMemberFunction)&GetConfigString, "GetConfigStringMSG");
    tBuiltin->RegisterHandler("getconfigtruth(string):truth", (BuiltinTypeMemberFunction)&GetConfigTruth, "GetConfigTruthMSG");
    tBuiltin->RegisterHandler("get_session_variable_string(string):string", (BuiltinTypeMemberFunction)&GetSessionVariableString, "GetSessionVariableStringMSG");
    tBuiltin->RegisterHandler("set_session_variable_string(string,string)", (BuiltinTypeMemberFunction)&SetSessionVariableString, "SetSessionVariableStringMSG");
    tBuiltin->RegisterHandler("get_session_variable_truth(string):truth", (BuiltinTypeMemberFunction)&GetSessionVariableTruth, "GetSessionVariableTruthMSG");
    tBuiltin->RegisterHandler("set_session_variable_truth(string,truth)", (BuiltinTypeMemberFunction)&SetSessionVariableTruth, "SetSessionVariableTruthMSG");
    tBuiltin->RegisterHandler("set_current_country_code(string)", (BuiltinTypeMemberFunction)&SetCurrentCountryCode, "SetCurrentCountryCodeMSG");
    tBuiltin->RegisterHandler("get_current_country_code:string", (BuiltinTypeMemberFunction)&GetCurrentCountryCode, "GetCurrentCountryCodeMSG");
    tBuiltin->RegisterHandler("set_disc_error_message(entity,integer)", (BuiltinTypeMemberFunction)&SetDiscErrorMessage, "SetDiscErrorMessageMSG");
    tBuiltin->RegisterHandler("set_lod_factor(number)", (BuiltinTypeMemberFunction)&SetLodFactor, "SetLodFactorMSG");
    tBuiltin->RegisterHandler("loadscene(string)", (BuiltinTypeMemberFunction)&LoadScene, "LoadSceneMSG");
    tBuiltin->RegisterHandler("getsystemlanguage:integer", (BuiltinTypeMemberFunction)&GetSystemLanguage, "GetSystemLanguageMSG");
    tBuiltin->RegisterHandler("startcleanupdashboard(integer)", (BuiltinTypeMemberFunction)&StartCleanupDashboard, "StartCleanupDashboardMSG");
    tBuiltin->RegisterHandler("setscreenresolution(integer,integer)", (BuiltinTypeMemberFunction)&SetScreenResolution, "SetScreenResolutionMSG");
    tBuiltin->RegisterHandler("getscreenresolution:vector", (BuiltinTypeMemberFunction)&GetScreenResolution, "GetScreenResolutionMSG");
    tBuiltin->RegisterHandler("is_screen_resolution_available(integer,integer)", (BuiltinTypeMemberFunction)&IsScreenResolutionAvailable, "IsScreenResolutionAvailableMSG");
    tBuiltin->RegisterHandler("is_wide_screen:truth", (BuiltinTypeMemberFunction)&IsWideScreen, "IsWideScreenMSG");
    tBuiltin->RegisterHandler("set_virtual_hud_screensize(integer,integer)", (BuiltinTypeMemberFunction)&SetVirtualHudScreenSize, "SetVirtualHudScreenSizeMSG");
    tBuiltin->RegisterHandler("get_virtual_hud_screensize:vector", (BuiltinTypeMemberFunction)&GetVirtualHudScreenSize, "GetVirtualHudScreenSizeMSG");
    tBuiltin->RegisterHandler("get_screen_top_in_virtual_units:number", (BuiltinTypeMemberFunction)&GetScreenTopInVirtualUnits, "GetScreenTopInVirtualUnitsMSG");
    tBuiltin->RegisterHandler("get_screen_bottom_in_virtual_units:number", (BuiltinTypeMemberFunction)&GetScreenBottomInVirtualUnits, "GetScreenBottomInVirtualUnitsMSG");
    tBuiltin->RegisterHandler("get_screen_left_in_virtual_units:number", (BuiltinTypeMemberFunction)&GetScreenLeftInVirtualUnits, "GetScreenLeftInVirtualUnitsMSG");
    tBuiltin->RegisterHandler("get_screen_right_in_virtual_units:number", (BuiltinTypeMemberFunction)&GetScreenRightInVirtualUnits, "GetScreenRightInVirtualUnitsMSG");
    tBuiltin->RegisterHandler("disablecurrentloadscreen", (BuiltinTypeMemberFunction)&DisableCurrentLoadScreen, "DisableCurrentLoadScreenMSG");
    tBuiltin->RegisterHandler("geteditoractive:truth", (BuiltinTypeMemberFunction)&GetEditorActive, "GetEditorActiveMSG");
    tBuiltin->RegisterHandler("dumptable_create:integer", (BuiltinTypeMemberFunction)&DumptableCreate, "DumpTable_CreateMSG");
    tBuiltin->RegisterHandler("dumptable_createfromfile(string):integer", (BuiltinTypeMemberFunction)&DumptableCreateFromFile, "DumpTable_CreateFromFileMSG");
    tBuiltin->RegisterHandler("dumptable_addintegercolumn(integer,string)", (BuiltinTypeMemberFunction)&DumptableAddIntegerColumn, "DumpTable_AddIntegerColumnMSG");
    tBuiltin->RegisterHandler("dumptable_addnumbercolumn(integer,string)", (BuiltinTypeMemberFunction)&DumptableAddNumberColumn, "DumpTable_AddNumberColumnMSG");
    tBuiltin->RegisterHandler("dumptable_addstringcolumn(integer,string)", (BuiltinTypeMemberFunction)&DumptableAddStringColumn, "DumpTable_AddStringColumnMSG");
    tBuiltin->RegisterHandler("dumptable_setnumrows(integer,integer)", (BuiltinTypeMemberFunction)&DumptableSetNumRows, "DumpTable_SetNumRowsMSG");
    tBuiltin->RegisterHandler("dumptable_setintegervalue(integer,integer,integer,integer)", (BuiltinTypeMemberFunction)&DumptableSetIntegerValue, "DumpTable_SetIntegerValueMSG");
    tBuiltin->RegisterHandler("dumptable_setnumbervalue(integer,integer,integer,number)", (BuiltinTypeMemberFunction)&DumptableSetNumberValue, "DumpTable_SetNumberValueMSG");
    tBuiltin->RegisterHandler("dumptable_setstringvalue(integer,integer,integer,string)", (BuiltinTypeMemberFunction)&DumptableSetStringValue, "DumpTable_SetStringValueMSG");
    tBuiltin->RegisterHandler("dumptable_writetofile(integer,string)", (BuiltinTypeMemberFunction)&DumptableWriteToFile, "DumpTable_WriteToFileMSG");
    tBuiltin->RegisterHandler("dumptable_close(integer)", (BuiltinTypeMemberFunction)&DumptableClose, "DumpTable_CloseMSG");
    tBuiltin->RegisterHandler("editor_reloadallassets(string)", (BuiltinTypeMemberFunction)&EditorReloadAllAssets, "Editor_ReloadAllAssetsMSG");
    tBuiltin->RegisterHandler("editor_selectnode(entity)", (BuiltinTypeMemberFunction)&EditorSelectNode, "Editor_SelectNodeMSG");
    tBuiltin->RegisterHandler("getregion:integer", (BuiltinTypeMemberFunction)&GetRegion, "GetRegionMSG");
    tBuiltin->RegisterHandler("getmessageid(string):integer", (BuiltinTypeMemberFunction)&GetMessageId, "GetMessageIDMSG");
    tBuiltin->RegisterHandler("quitgame", (BuiltinTypeMemberFunction)&QuitGame, "QuitGameMSG");
    tBuiltin->RegisterHandler("getcoverdemoplaymode:integer", (BuiltinTypeMemberFunction)&GetCoverdemoPlayMode, "GetCoverdemoPlayModeMSG");
    tBuiltin->RegisterHandler("getcoverdemoinactivetimeoutsec:integer", (BuiltinTypeMemberFunction)&GetCoverdemoInactiveTimeoutSec, "GetCoverdemoInactiveTimeoutSecMSG");
    tBuiltin->RegisterHandler("getcoverdemogameplaytimeoutsec:integer", (BuiltinTypeMemberFunction)&GetCoverdemoGameplayTimeoutSec, "GetCoverdemoGameplayTimeoutSecMSG");
    tBuiltin->RegisterHandler("coverdemoexit(integer)", (BuiltinTypeMemberFunction)&CoverdemoExit, "CoverdemoExitMSG");
}

BuiltinType::BuiltinMember::BuiltinMember(const BuiltinMember& rhs)
{
    MESSAGE_CLASS_CREATED(BuiltinMember);

    m_RetType = rhs.m_RetType;
    m_Getter = rhs.m_Getter;
    m_Setter = rhs.m_Setter;
    m_Proto = rhs.m_Proto;
    m_Str_2 = rhs.m_Str_2;
}