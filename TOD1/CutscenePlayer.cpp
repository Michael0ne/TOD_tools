#include "CutscenePlayer.h"
#include "StringType.h"
#include "TruthType.h"
#include "IntegerType.h"
#include "NumberType.h"

EntityType* tCutscenePlayer;

int CutscenePlayer::InterpolationOnCommand = -1;
int CutscenePlayer::InterpolationOffCommand = -1;
int CutscenePlayer::InterpolationOffFrameCommand = -1;

CutscenePlayer::CutscenePlayer() : Node(NODE_MASK_EMPTY)
{
    MESSAGE_CLASS_CREATED(CutscenePlayer);

    field_50 = 0;
    field_54 = 1;

    m_StopEvent = {};
    m_ActiveCameraAnim = {};
    m_BeforeStartEvent = {};
    m_AfterStopEvent = {};
    m_String_5 = {};
}

CutscenePlayer::~CutscenePlayer()
{
    MESSAGE_CLASS_DESTROYED(CutscenePlayer);
}

const char* const CutscenePlayer::GetCutscene() const
{
    return nullptr;
}

void CutscenePlayer::SetCutscene(const char* const cutscene)
{
}

const bool CutscenePlayer::IsPlaying() const
{
    return m_Flags.Playing;
}

const bool CutscenePlayer::IsLooped() const
{
    return m_Flags.Looped;
}

void CutscenePlayer::SetIsLooped(const bool looped)
{
    m_Flags.Looped = looped;
}

const float CutscenePlayer::GetTimePos() const
{
    return m_Timepos;
}

void CutscenePlayer::SetTimePos(const float timepos)
{
    m_Timepos = timepos;
}

const int CutscenePlayer::GetFramePos() const
{
    return (int)((field_80 - 1.f) * m_Timepos);
}

void CutscenePlayer::SetFramePos(const int framepos)
{
    m_Timepos = (float)framepos / (field_80 - 1.f);
}

const float CutscenePlayer::GetSpeed() const
{
    return m_Speed;
}

void CutscenePlayer::SetSpeed(const float speed)
{
    m_Speed = speed;
}

const float CutscenePlayer::GetCamNearClip() const
{
    return m_CamNearclip;
}

void CutscenePlayer::SetCamNearClip(const float nearclip)
{
    m_CamNearclip = nearclip;
}

const float CutscenePlayer::GetCamDynLightsCullRange() const
{
    return m_CamDynlightsCullrange;
}

void CutscenePlayer::SetCamDynLightsCullRange(const float cullrange)
{
    m_CamDynlightsCullrange = cullrange;
}

const bool CutscenePlayer::OverrideCurrentCamera() const
{
    return m_Flags.OverrideCurrentCamera;
}

void CutscenePlayer::SetOverrideCurrentCamera(const bool overridecamera)
{
    m_Flags.OverrideCurrentCamera = overridecamera;
}

const char* const CutscenePlayer::GetActiveCameraAnim() const
{
    return m_ActiveCameraAnim.m_Str;
}

void CutscenePlayer::SetActiveCameraAnim(const char* const anim)
{
    m_ActiveCameraAnim = anim;
}

const char* const CutscenePlayer::GetBeforeStartEvent() const
{
    return m_BeforeStartEvent.m_Str;
}

void CutscenePlayer::SetBeforeStartEvent(const char* const eventname)
{
    m_BeforeStartEvent = eventname;
}

const char* const CutscenePlayer::GetAfterStopEvent() const
{
    return m_AfterStopEvent.m_Str;
}

void CutscenePlayer::SetAfterStopEvent(const char* const eventname)
{
    m_AfterStopEvent = eventname;
}

const bool CutscenePlayer::IsDefaultInherit() const
{
    return m_Flags.DefaultInherit;
}

void CutscenePlayer::SetDefaultInherit(const bool definherit)
{
    m_Flags.DefaultInherit = definherit;
}

void CutscenePlayer::Play(int* args)
{
}

void CutscenePlayer::PlayWithStopEvent(int* args)
{
    m_StopEvent = (const char*)args[0];
}

void CutscenePlayer::Register()
{
    tCutscenePlayer = new EntityType("CutscenePlayer");
    tCutscenePlayer->InheritFrom(tNode);
    tCutscenePlayer->SetCreator((CREATOR)Create);

    tCutscenePlayer->RegisterProperty(tSTRING, "cutscene", (EntityGetterFunction)&GetCutscene, (EntitySetterFunction)&SetCutscene, "control=resource|type=*.cutscene");
    tCutscenePlayer->RegisterProperty(tTRUTH, "playing", (EntityGetterFunction)&IsPlaying, nullptr, nullptr);
    tCutscenePlayer->RegisterProperty(tTRUTH, "looped", (EntityGetterFunction)&IsLooped, (EntitySetterFunction)&SetIsLooped, "control=truth");
    tCutscenePlayer->RegisterProperty(tNUMBER, "timepos", (EntityGetterFunction)&GetTimePos, (EntitySetterFunction)&SetTimePos, "control=slider|min=0.0|max=1.0");
    tCutscenePlayer->RegisterProperty(tINTEGER, "framepos", (EntityGetterFunction)&GetFramePos, (EntitySetterFunction)&SetFramePos, "control=slider|min=0|max=2000");
    tCutscenePlayer->RegisterProperty(tNUMBER, "speed", (EntityGetterFunction)&GetSpeed, (EntitySetterFunction)&SetSpeed, "control=slider|min=-2.0|max=2.0");
    tCutscenePlayer->RegisterProperty(tNUMBER, "cam_nearclip", (EntityGetterFunction)&GetCamNearClip, (EntitySetterFunction)&SetCamNearClip, "control=slider|min=0.01|max=1.0");
    tCutscenePlayer->RegisterProperty(tNUMBER, "cam_dynlights_cullrange", (EntityGetterFunction)&GetCamDynLightsCullRange, (EntitySetterFunction)&SetCamDynLightsCullRange, "control=slider|min=5|max=5000");
    tCutscenePlayer->RegisterProperty(tTRUTH, "override_current_camera", (EntityGetterFunction)&OverrideCurrentCamera, (EntitySetterFunction)&SetOverrideCurrentCamera, "control=truth");
    tCutscenePlayer->RegisterProperty(tSTRING, "active_camera_anim", (EntityGetterFunction)&GetActiveCameraAnim, (EntitySetterFunction)&SetActiveCameraAnim, "control=string");
    tCutscenePlayer->RegisterProperty(tSTRING, "before_start_event", (EntityGetterFunction)&GetBeforeStartEvent, (EntitySetterFunction)&SetBeforeStartEvent, "control=string");
    tCutscenePlayer->RegisterProperty(tSTRING, "after_stop_event", (EntityGetterFunction)&GetAfterStopEvent, (EntitySetterFunction)&SetAfterStopEvent, "control=string");
    tCutscenePlayer->RegisterProperty(tTRUTH, "default_inherit", (EntityGetterFunction)&IsDefaultInherit, (EntitySetterFunction)&SetDefaultInherit, nullptr);

    tCutscenePlayer->RegisterScript("play", (EntityFunctionMember)&Play, 0, 0, 0, "control=button|text=play");
    tCutscenePlayer->RegisterScript("playwithstopevent(string)", (EntityFunctionMember)&PlayWithStopEvent);
    tCutscenePlayer->RegisterScript("pause", (EntityFunctionMember)&Pause, 0, 0, 0, "control=button|text=pause");
    tCutscenePlayer->RegisterScript("stop", (EntityFunctionMember)&Stop, 0, 0, 0, "control=button|text=stop");
    tCutscenePlayer->RegisterScript("organizeanimationdata", (EntityFunctionMember)&OrganizeAnimationData);
    tCutscenePlayer->RegisterScript("playmode_stop", (EntityFunctionMember)&PlayModeStop);
    tCutscenePlayer->RegisterScript("getnumregistered_rootobjs:integer", (EntityFunctionMember)&GetNumRegisteredRootObjs);
    tCutscenePlayer->RegisterScript("getregistered_rootobj(integer):entity", (EntityFunctionMember)&GetRegisteredRootObj);
    tCutscenePlayer->RegisterScript("setobject_inherit(entity,truth)", (EntityFunctionMember)&SetObjectInherit);
    tCutscenePlayer->RegisterScript("getobject_inherit(entity):truth", (EntityFunctionMember)&GetObjectInherit);
    
    InterpolationOnCommand = GetMessage("interpol_on", true);
    InterpolationOffCommand = GetMessage("interpol_off", true);
    InterpolationOffFrameCommand = GetMessage("interpol_off_frame", true);

    tCutscenePlayer->PropagateProperties();
}

CutscenePlayer* CutscenePlayer::Create(AllocatorIndex)
{
    return new CutscenePlayer;
}

void CutscenePlayer::Pause(const int)
{
    m_Flags.Paused = !m_Flags.Paused;
}

void CutscenePlayer::Stop(int* args)
{
}

void CutscenePlayer::OrganizeAnimationData(int* args)
{
}

void CutscenePlayer::PlayModeStop(int* args)
{
}

void CutscenePlayer::GetNumRegisteredRootObjs(int* args) const
{
    args[0] = m_RegisteredRootObjects.size();
}

void CutscenePlayer::GetRegisteredRootObj(int* args) const
{
    if (field_50)
        args[0] = m_RegisteredRootObjects[args[1]].field_20;
    else
        args[0] = 0;
}

void CutscenePlayer::SetObjectInherit(int* args)
{
    SetObjectInherit_Impl((Node*)args[0], args[1] != false);
}

void CutscenePlayer::SetObjectInherit_Impl(Node* node, const bool inherit)
{
    if (!m_CutsceneObjects.size())
        return;

    unsigned int j = 0;
    for (unsigned int i = 0; i < m_CutsceneObjects.size(); ++i, j++)
    {
        m_CutsceneObjects[i].m_NodePtr = node;
        if (node)
            break;
    }

    m_CutsceneObjects[j].m_Inherit = inherit;
}

void CutscenePlayer::GetObjectInherit(int* args)
{
    if (!m_CutsceneObjects.size())
    {
        args[0] = 0;
        return;
    }

    unsigned int i = 0;
    for (; i < m_CutsceneObjects.size(); ++i)
    {
        if (m_CutsceneObjects[i].m_NodePtr == (Node*)args[1])
            break;
    }

    args[0] = m_CutsceneObjects[i].m_Inherit;
}