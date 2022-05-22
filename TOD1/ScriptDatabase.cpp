#include "ScriptDatabase.h"
#include "LogDump.h"
#include "Timer.h"
#include "Globals.h"
#include "EntityType.h"
#include "AssetManager.h"
#include "NothingType.h"
#include "StructType.h"
#include "Entity.h"
#include "Scene.h"
#include "ScriptThread.h"

//  NOTE: actual source path "/data_baked/tnt/production/_c++_/scriptresource.cpp".
std::vector<GlobalProperty>  GlobalPropertiesList;
std::map<String, unsigned int> GlobalPropertiesMap;
std::vector<GlobalCommand>  GlobalCommandsList;
std::map<String, unsigned int> GlobalCommandsMap;
std::vector<Scriptbaked*> Scriptbaked::ScriptsList;
std::vector<Node*> Scriptbaked::SceneScriptEntitiesList;

unsigned int GlobalPropertyListChecksum;
bool   GlobalPropertyListChecksumObtained;
unsigned int GlobalCommandListChecksum;
bool   GlobalCommandListChecksumObtained;
void* GlobalScriptsArray[410];
Node* CacheScriptNode;
Node* CommonScriptNode;
Node* CommonAnimSlotScriptNode;
Node* CommonTriggerScriptNode;
Node* IntroCommonScriptNode;

unsigned int GetGlobalPropertyListChecksum()
{
    if (GlobalPropertyListChecksumObtained)
        return GlobalPropertyListChecksum;

    String checksum_str;
    if (GlobalPropertiesList.size() > 0)
    {
        for (std::vector<GlobalProperty>::const_iterator it = GlobalPropertiesList.cbegin(); it != GlobalPropertiesList.cend(); ++it)
        {
            String tempstr;
            it->GetNameAndType(tempstr);

            if (checksum_str.m_Length >= 10000)
                break;
            checksum_str.Append(tempstr.m_Str);
        }
    }

    GlobalPropertyListChecksum = Utils::CalcCRC32(checksum_str.m_Str, checksum_str.m_Length);
    GlobalPropertyListChecksumObtained = true;
    return GlobalPropertyListChecksum;
}

unsigned int GetGlobalCommandListChecksum()
{
    if (GlobalCommandListChecksumObtained)
        return GlobalCommandListChecksum;

    String checksum_str;
    if (GlobalCommandsList.size() > 0)
    {
        for (std::vector<GlobalCommand>::iterator it = GlobalCommandsList.begin(); it != GlobalCommandsList.end(); ++it)
        {
            String tempstr;
            it->GetReturnTypeString(tempstr);

            checksum_str.Append(tempstr.m_Str);
        }
    }

    GlobalCommandListChecksum = Utils::CalcCRC32(checksum_str.m_Str, checksum_str.m_Length);
    GlobalCommandListChecksumObtained = true;
    return GlobalCommandListChecksum;
}

extern unsigned int GetGlobalPropertyListSize()
{
    return GlobalPropertiesList.size();
}

extern unsigned int GetGlobalCommandListSize()
{
    return GlobalCommandsList.size();
}

int GetPropertyIdByName(const char* const propertyname)
{
    char propname[256] = {};
    strncpy_s(propname, sizeof(propname), propertyname, strchr(propertyname, ':') - propertyname);

    const std::map<String, unsigned int>::const_iterator& it = GlobalPropertiesMap.find(propname);
    if (it == GlobalPropertiesMap.cend())
        return -1;
    else
        return it->second;
}

int GetCommandByName_Impl(const char* const commandname)
{
    String cmdname = commandname;
    cmdname.ToLowerCase();

    const std::map<String, unsigned int>::const_iterator& it = GlobalCommandsMap.find(cmdname);
    if (it == GlobalCommandsMap.cend())
        return -1;
    else
        return it->second;
}

int GetCommandByName(const char* const commandname)
{
    const char* const ddotpos = strchr(commandname, ':');
    if (ddotpos)
    {
        char cmdname[256] = {};
        strncpy(cmdname, commandname, ddotpos - commandname);

        return GetCommandByName_Impl(cmdname);
    }
    else
        return GetCommandByName_Impl(commandname);
}

short GetProperty(const char* const propertyname, bool existing)
{
    if (!GlobalPropertiesList.size())
        GlobalPropertiesList.reserve(6100);

    const char* ddotpos = strchr(propertyname, ':');
    if (!ddotpos)
    {
        LogDump::LogA("unable to register property '%s' - it does not specify a type", propertyname);
        return -1;
    }

    int propid = GetPropertyIdByName(propertyname);
    const char* proptype = ddotpos + 1;
    if (existing && propid >= 0)
    {
        if (strncmp(GlobalPropertiesList[propid].m_PropertyType->m_TypeName.m_Str, proptype, strlen(proptype)) == NULL)
        {
#if defined INCLUDE_FIXES && defined VERBOSELOG
            LogDump::LogA("Property \"%s\" was already registered with id=%d!\n", propertyname, propid);
#endif
            return propid;
        }
        else
            return -1;
    }
    else
    {
        DataType::LoadScript(proptype);

        unsigned int propind = GlobalPropertiesList.size();
        GlobalPropertiesList.emplace_back(propertyname, propind);

        char propname[256] = {};
        strncpy(propname, propertyname, ddotpos - propertyname);
        String::ToLowerCase(propname);

        GlobalPropertiesMap[propname] = propind;
        GlobalPropertyListChecksumObtained = false;

#if defined INCLUDE_FIXES && defined VERBOSELOG
        LogDump::LogA("Property \"%s\" has been registered with id=%d\n", propertyname, propind);
#endif

        return propind;
    }
}

int GetMessage(const char* const commandname, bool existing)
{
    if (!GlobalCommandsList.size())
        GlobalCommandsList.reserve(3000);

    int cmdid = GetCommandByName(commandname);
    const char* const ddotpos = strchr(commandname, ':');
    if (existing && cmdid >= 0)
    {
        if (ddotpos)
            DataType::GetTypeByName(ddotpos + 1);

#if defined INCLUDE_FIXES && defined VERBOSELOG
        LogDump::LogA("Command \"%s\" already exists with id=%d!\n", commandname, cmdid);
#endif

        return cmdid;
    }
    else
    {
        int commandind = GlobalCommandsList.size();
        GlobalCommandsList.emplace_back(commandname, commandind);

        if (ddotpos)
        {
            char cmdname[256] = {};
            strncpy(cmdname, commandname, ddotpos - commandname);
            GlobalCommandsMap[cmdname] = commandind;
        }
        else
            GlobalCommandsMap[commandname] = commandind;

#if defined INCLUDE_FIXES && defined VERBOSELOG
        LogDump::LogA("Command \"%s\" has been registered with id=%d!\n", commandname, commandind);
#endif

        return commandind;
    }
}

void ReadDatabaseFile(const char* path)
{
    LogDump::LogA("Loading script database\n");
    FileBuffer dbfile(path, 0x21, true);

    if (!dbfile.IsFileOpen())
        return;

    DWORD timeStart = Timer::GetMilliseconds();
    unsigned int totalProperties = NULL;
    unsigned int totalMessages = NULL;

    dbfile.Read(&totalProperties, sizeof(totalProperties));
    GlobalPropertiesList.reserve(totalProperties);

    for (unsigned int i = 0; i < totalProperties; i++)
    {
        char propertyName[128] = {};
        unsigned int propertyNameLen = NULL;

        dbfile.Read(&propertyNameLen, sizeof(propertyNameLen));
        dbfile.Read(propertyName, propertyNameLen);

        GetProperty(propertyName, false);
    }

    dbfile.Read(&totalMessages, sizeof(totalMessages));
    GlobalCommandsList.reserve(totalMessages);

    for (unsigned int i = 0; i < totalMessages; i++)
    {
        char messageName[128] = {};
        unsigned int messageNameLen = NULL;

        dbfile.Read(&messageNameLen, sizeof(messageNameLen));
        dbfile.Read(messageName, messageNameLen);

        GetMessage(messageName, false);
    }

    LogDump::LogA("Done loading script database (%dms)\n", Timer::GetMilliseconds() - timeStart);
#ifdef INCLUDE_FIXES
    LogDump::LogA("Read %d properties and %d commands\n", totalProperties, totalMessages);
#endif
}

bool FindScript(const char* const scriptname, String& zipname)
{
    if (Script::ScriptsPath.m_Length == NULL)
        return false;

    char scriptnamefull[256] = {};
    sprintf(scriptnamefull, "%s.script", scriptname);

    if (FileBuffer::SearchScriptFile(Script::ScriptsPath.m_Str, scriptnamefull, zipname))
        return true;

    LogDump::LogA("Script '%s.script' not found (search path is '%s')\n", scriptname, Script::ScriptsPath.m_Str);
    return false;
}

#pragma message(TODO_IMPLEMENTATION)
void LoadScripts()
{
    // NOTE: register built-in scripts.
    StructType::ScriptFieldsList fields;
    fields.Add("RecievingEntity", DataType::LoadScript("Entity"), 0);
    fields.Add("Damage", DataType::LoadScript("number"), 0);
    fields.Add("Owner", DataType::LoadScript("Entity"), 0);
    fields.Add("inflictor_node", DataType::LoadScript("Entity"), 0);
    fields.Add("RecievingCollVolume", DataType::LoadScript("Entity"), 0);
    fields.Add("CollisionImpulse", DataType::LoadScript("vector"), 0);
    fields.Add("Peercontactpoint", DataType::LoadScript("vector"), 0);
    fields.Add("Damage_type", DataType::LoadScript("integer"), 0);

    new StructType("Take_a_hit_info", fields);
    fields.Clear();

    fields.Add("pBlock", DataType::LoadScript("Entity"), 0);
    fields.Add("sTeleportName", DataType::LoadScript("string"), 0);

    new StructType("s_BlockLoad", fields);
    fields.Clear();

    fields.Add("pResource", DataType::LoadScript("Entity"), 0);
    fields.Add("pPlaceholder", DataType::LoadScript("Entity"), 0);

    new StructType("s_GenericRes", fields);
    fields.Clear();

    fields.Add("pPlaceholder", DataType::LoadScript("Entity"), 0);
    fields.Add("RangeSqr", DataType::LoadScript("number"), 0);
    fields.Add("bInView", DataType::LoadScript("truth"), 0);
    fields.Add("bIsVisible", DataType::LoadScript("truth"), 0);

    new StructType("s_PlaceholderPriority", fields);
    fields.Clear();

    fields.Add("RightHandGunList", DataType::LoadScript("list(truth)"), 0);
    fields.Add("LeftHandGunList", DataType::LoadScript("list(truth)"), 0);
    fields.Add("AmmoList", DataType::LoadScript("list(integer)"), 0);
    fields.Add("MaxAmmoList", DataType::LoadScript("list(integer)"), 0);
    fields.Add("AmmoInPrimWeapon", DataType::LoadScript("list(integer)"), 0);
    fields.Add("AmmoInSecWeapon", DataType::LoadScript("list(integer)"), 0);

    new StructType("s_Inventory", fields);
    fields.Clear();

    fields.Add("pPlaceHolder", DataType::LoadScript("Entity"), 0);
    fields.Add("vLocalPosition", DataType::LoadScript("vector"), 0);

    new StructType("s_AIPassenger", fields);
    fields.Clear();

    fields.Add("PowerUpList", DataType::LoadScript("list(integer)"), 0);
    fields.Add("MaxPowerUpList", DataType::LoadScript("list(integer)"), 0);

    new StructType("s_PowerUpInventory", fields);
    fields.Clear();

    fields.Add("Type", DataType::LoadScript("integer"), 0);
    fields.Add("Sprite", DataType::LoadScript("Entity"), 0);
    fields.Add("BGSprite", DataType::LoadScript("Entity"), 0);
    fields.Add("OrgSize", DataType::LoadScript("vector"), 0);
    fields.Add("NormalSize", DataType::LoadScript("vector"), 0);
    fields.Add("NormalPos", DataType::LoadScript("vector"), 0);
    fields.Add("SelectedSize", DataType::LoadScript("vector"), 0);
    fields.Add("SelectedPos", DataType::LoadScript("vector"), 0);
    fields.Add("AmountTB", DataType::LoadScript("Entity"), 0);
    fields.Add("AmountTB_BG", DataType::LoadScript("Entity"), 0);

    new StructType("s_PowerUpSprite", fields);
    fields.Clear();

    fields.Add("BlipSprite", DataType::LoadScript("Entity"), 0);
    fields.Add("LocationSprite", DataType::LoadScript("Entity"), 0);
    fields.Add("atchedEntity", DataType::LoadScript("Entity"), 0);
    fields.Add("EntityType", DataType::LoadScript("integer"), 0);
    fields.Add("SubMapIndex", DataType::LoadScript("integer"), 0);

    new StructType("s_RadarBlip", fields);
    fields.Clear();

    fields.Add("OptionName", DataType::LoadScript("integer"), 0);
    fields.Add("TextField", DataType::LoadScript("Entity"), 0);
    fields.Add("TextFieldBG", DataType::LoadScript("Entity"), 0);
    fields.Add("OptionAction", DataType::LoadScript("integer"), 0);

    new StructType("s_MenuOption", fields);
    fields.Clear();

    fields.Add("TaxiZapEntry", DataType::LoadScript("Entity"), 0);
    fields.Add("EntryIcon", DataType::LoadScript("Entity"), 0);
    fields.Add("ShowIcon", DataType::LoadScript("truth"), 0);
    fields.Add("EntryText", DataType::LoadScript("integer"), 0);
    fields.Add("EntryType", DataType::LoadScript("integer"), 0);
    fields.Add("EntryDetailTextType", DataType::LoadScript("integer"), 0);
    fields.Add("EntryDetailTextIndex", DataType::LoadScript("integer"), 0);
    fields.Add("EntrySpacing", DataType::LoadScript("truth"), 0);
    fields.Add("IsActiveEntry", DataType::LoadScript("truth"), 0);

    new StructType("s_TaxiZapOptions", fields);
    fields.Clear();

    fields.Add("IsAchieved", DataType::LoadScript("truth"), 0);
    fields.Add("NameTextField", DataType::LoadScript("Entity"), 0);
    fields.Add("NameTextFieldBG", DataType::LoadScript("Entity"), 0);
    fields.Add("ScoreTextField", DataType::LoadScript("Entity"), 0);
    fields.Add("Stroke3", DataType::LoadScript("Entity"), 0);
    fields.Add("Stroke4", DataType::LoadScript("Entity"), 0);
    fields.Add("FlashEffect", DataType::LoadScript("Entity"), 0);
    fields.Add("AwardType", DataType::LoadScript("integer"), 0);
    fields.Add("AwardAP", DataType::LoadScript("integer"), 0);
    fields.Add("AwardModelList", DataType::LoadScript("list(Entity)"), 0);
    fields.Add("AwardModelListBg", DataType::LoadScript("list(Entity)"), 0);
    fields.Add("NumOfAwards", DataType::LoadScript("integer"), 0);

    new StructType("s_ScoreBar", fields);
    fields.Clear();

    fields.Add("TextBox", DataType::LoadScript("Entity"), 0);
    fields.Add("TextBoxBG", DataType::LoadScript("Entity"), 0);
    fields.Add("BounceSpeed", DataType::LoadScript("number"), 0);
    fields.Add("OrgScale", DataType::LoadScript("number"), 0);
    fields.Add("MaxScale", DataType::LoadScript("number"), 0);
    fields.Add("Expand", DataType::LoadScript("truth"), 0);

    new StructType("s_BounceText", fields);
    fields.Clear();

    fields.Add("Placeholder", DataType::LoadScript("Entity"), 0);
    fields.Add("StartPoint", DataType::LoadScript("vector"), 0);
    fields.Add("NewPoint", DataType::LoadScript("Entity"), 0);
    fields.Add("LastPoint", DataType::LoadScript("Entity"), 0);
    fields.Add("currenttime", DataType::LoadScript("number"), 0);
    fields.Add("at_pos", DataType::LoadScript("truth"), 0);

    new StructType("s_ped_resource", fields);
    fields.Clear();

    fields.Add("pBone", DataType::LoadScript("Entity"), 0);
    fields.Add("vCurrentAngle", DataType::LoadScript("vector"), 0);
    fields.Add("vTargetAngle", DataType::LoadScript("vector"), 0);
    fields.Add("Speed", DataType::LoadScript("number"), 0);
    fields.Add("eRotateSpace", DataType::LoadScript("integer"), 0);
    fields.Add("bActive", DataType::LoadScript("truth"), 0);

    new StructType("s_CtrlBone", fields);
    fields.Clear();

    fields.Add("Objective", DataType::LoadScript("Entity"), 0);
    fields.Add("Show", DataType::LoadScript("truth"), 0);
    fields.Add("Status", DataType::LoadScript("integer"), 0);

    new StructType("s_Mission_Objective", fields);
    fields.Clear();

    fields.Add("Mission_Valid", DataType::LoadScript("truth"), 0);
    fields.Add("Mission", DataType::LoadScript("integer"), 0);
    fields.Add("Mission_type", DataType::LoadScript("integer"), 0);
    fields.Add("Award_achived", DataType::LoadScript("integer"), 0);
    fields.Add("Mission_Entity", DataType::LoadScript("Entity"), 0);
    fields.Add("Mission_complete", DataType::LoadScript("truth"), 0);
    fields.Add("Mission_open", DataType::LoadScript("truth"), 0);
    fields.Add("Mission_name", DataType::LoadScript("integer"), 0);
    fields.Add("Mission_start_submap", DataType::LoadScript("integer"), 0);
    fields.Add("Highest_Score", DataType::LoadScript("integer"), 0);
    fields.Add("Current_Score", DataType::LoadScript("integer"), 0);
    fields.Add("L_Combo", DataType::LoadScript("number"), 0);
    fields.Add("Best_Move_string", DataType::LoadScript("string"), 0);
    fields.Add("Best_Move", DataType::LoadScript("integer"), 0);
    fields.Add("FollowUpMission", DataType::LoadScript("integer"), 0);
    fields.Add("MissionConditions", DataType::LoadScript("list(Entity)"), 0);
    fields.Add("ObjectiveList", DataType::LoadScript("list(Entity)"), 0);

    new StructType("s_Mission_data", fields);
    fields.Clear();

    fields.Add("TextBox", DataType::LoadScript("Entity"), 0);
    fields.Add("TextBox_BG", DataType::LoadScript("Entity"), 0);
    fields.Add("SlideUpTime", DataType::LoadScript("number"), 0);
    fields.Add("StartSlideTime", DataType::LoadScript("number"), 0);
    fields.Add("SlideX", DataType::LoadScript("number"), 0);
    fields.Add("SlideY", DataType::LoadScript("number"), 0);
    fields.Add("FromPos", DataType::LoadScript("vector"), 0);
    fields.Add("TargetPos", DataType::LoadScript("vector"), 0);
    fields.Add("FromOpacity", DataType::LoadScript("number"), 0);
    fields.Add("TargetOpacity", DataType::LoadScript("number"), 0);
    fields.Add("StayTime", DataType::LoadScript("number"), 0);
    fields.Add("InUse", DataType::LoadScript("truth"), 0);

    new StructType("s_ComboAwardString", fields);
    fields.Clear();

    fields.Add("MoveName", DataType::LoadScript("integer"), 0);
    fields.Add("Bonus", DataType::LoadScript("integer"), 0);
    fields.Add("GoodieAward", DataType::LoadScript("integer"), 0);
    fields.Add("MoveBitName", DataType::LoadScript("integer"), 0);
    fields.Add("SoundIndex", DataType::LoadScript("integer"), 0);
    fields.Add("SoundFrequency", DataType::LoadScript("number"), 0);
    fields.Add("CCmove", DataType::LoadScript("integer"), 0);

    new StructType("s_UniqueComboMove", fields);
    fields.Clear();

    fields.Add("KillsNeeded", DataType::LoadScript("integer"), 0);
    fields.Add("ComboBonus", DataType::LoadScript("integer"), 0);
    fields.Add("GoodieAwards", DataType::LoadScript("list(integer)"), 0);
    fields.Add("AwardString", DataType::LoadScript("integer"), 0);

    new StructType("s_ComboAward", fields);
    fields.Clear();

    fields.Add("Position", DataType::LoadScript("vector"), 0);
    fields.Add("CurrentHealth", DataType::LoadScript("number"), 0);
    fields.Add("CurrentWeapons", DataType::LoadScript("list(string)"), 0);
    fields.Add("AmmoActiveWeapon", DataType::LoadScript("number"), 0);

    new StructType("s_StatusPoint", fields);
    fields.Clear();

    fields.Add("ActionsPoints", DataType::LoadScript("integer"), 0);
    fields.Add("EnemyKills", DataType::LoadScript("integer"), 0);
    fields.Add("NeutralKills", DataType::LoadScript("integer"), 0);
    fields.Add("BarrelsKilled", DataType::LoadScript("integer"), 0);
    fields.Add("AvarageHealth", DataType::LoadScript("number"), 0);
    fields.Add("UsedKillTypes", DataType::LoadScript("list(string)"), 0);
    fields.Add("MostOftenKill", DataType::LoadScript("list(integer)"), 0);
    fields.Add("LargestCombo", DataType::LoadScript("string"), 0);
    fields.Add("DrivingCar", DataType::LoadScript("integer"), 0);
    fields.Add("MenuOpened", DataType::LoadScript("integer"), 0);
    fields.Add("MaxNumOfWeapon", DataType::LoadScript("integer"), 0);
    fields.Add("MostUsedWeapon", DataType::LoadScript("integer"), 0);
    fields.Add("Medals", DataType::LoadScript("integer"), 0);

    new StructType("s_Statistics", fields);
    fields.Clear();

    fields.Add("pCallBack", DataType::LoadScript("Entity"), 0);
    fields.Add("eMessage", DataType::LoadScript("integer"), 0);
    fields.Add("nTimer", DataType::LoadScript("number"), 0);
    fields.Add("bRealTime", DataType::LoadScript("truth"), 0);

    new StructType("s_DelayedMessage", fields);
    fields.Clear();

    fields.Add("HasCollided", DataType::LoadScript("truth"), 0);
    fields.Add("ColEntity", DataType::LoadScript("Entity"), 0);
    fields.Add("ColVolume", DataType::LoadScript("Entity"), 0);
    fields.Add("ColMaterial", DataType::LoadScript("integer"), 0);
    fields.Add("ColFaceProperty", DataType::LoadScript("integer"), 0);
    fields.Add("ColNormal", DataType::LoadScript("vector"), 0);
    fields.Add("ColPos", DataType::LoadScript("vector"), 0);
    fields.Add("SetDecal", DataType::LoadScript("truth"), 0);

    new StructType("s_CollisionInfo", fields);
    fields.Clear();

    fields.Add("CollisionEntity", DataType::LoadScript("Entity"), 0);
    fields.Add("RealNode", DataType::LoadScript("Entity"), 0);
    fields.Add("CollisionPos", DataType::LoadScript("vector"), 0);
    fields.Add("ColliderEntity", DataType::LoadScript("Entity"), 0);

    new StructType("s_VehicleCollision", fields);
    fields.Clear();

    fields.Add("Sound", DataType::LoadScript("list(Entity)"), 0);
    fields.Add("Volume", DataType::LoadScript("list(number)"), 0);
    fields.Add("Min_Pause", DataType::LoadScript("list(number)"), 0);
    fields.Add("Max_Pause", DataType::LoadScript("list(number)"), 0);
    fields.Add("Position_Type", DataType::LoadScript("list(integer)"), 0);
    fields.Add("Position_Entity", DataType::LoadScript("list(Entity)"), 0);

    new StructType("Random_sound", fields);
    fields.Clear();

    fields.Add("ent_1", DataType::LoadScript("Entity"), 0);
    fields.Add("ent_2", DataType::LoadScript("Entity"), 0);

    new StructType("s_Entity_pair", fields);
    fields.Clear();

    fields.Add("Watched", DataType::LoadScript("Entity"), 0);
    fields.Add("WatchedName", DataType::LoadScript("integer"), 0);
    fields.Add("HealthPercentage", DataType::LoadScript("number"), 0);
    fields.Add("InFocus", DataType::LoadScript("truth"), 0);
    fields.Add("LocalShowTime", DataType::LoadScript("number"), 0);

    new StructType("s_HealthWatchedEntity", fields);
    fields.Clear();

    fields.Add("GamePulse", DataType::LoadScript("integer"), 0);
    fields.Add("UnderPulse", DataType::LoadScript("integer"), 0);
    fields.Add("UberPulse", DataType::LoadScript("integer"), 0);
    fields.Add("StayTime", DataType::LoadScript("number"), 0);
    fields.Add("toHighPulses", DataType::LoadScript("integer"), 0);

    new StructType("s_pulse", fields);
    fields.Clear();

    fields.Add("at_entity", DataType::LoadScript("Entity"), 0);
    fields.Add("pos_offset", DataType::LoadScript("vector"), 0);
    fields.Add("ParticleOwner", DataType::LoadScript("Entity"), 0);
    fields.Add("ParticleSys", DataType::LoadScript("Entity"), 0);
    fields.Add("at_pos", DataType::LoadScript("vector"), 0);
    fields.Add("LifeTime", DataType::LoadScript("number"), 0);

    new StructType("s_particel_dam", fields);
    fields.Clear();

    fields.Add("list_index", DataType::LoadScript("integer"), 0);
    fields.Add("dist", DataType::LoadScript("number"), 0);

    new StructType("s_list_dist", fields);
    fields.Clear();

    fields.Add("PedestrianGroup", DataType::LoadScript("integer"), 0);
    fields.Add("Minimum", DataType::LoadScript("integer"), 0);
    fields.Add("Maximum", DataType::LoadScript("integer"), 0);

    new StructType("s_neighborhood_config", fields);
    fields.Clear();

    fields.Add("PathPoint", DataType::LoadScript("Entity"), 0);
    fields.Add("Neighborhood", DataType::LoadScript("Entity"), 0);

    new StructType("s_pedestrian_points", fields);
    fields.Clear();

    fields.Add("Object", DataType::LoadScript("Entity"), 0);
    fields.Add("StartOpacity", DataType::LoadScript("number"), 0);
    fields.Add("paPerSec", DataType::LoadScript("number"), 0);
    fields.Add("DelayTimer", DataType::LoadScript("number"), 0);

    new StructType("s_fadeEntity", fields);
    fields.Clear();

    fields.Add("Valid", DataType::LoadScript("truth"), 0);
    fields.Add("Type", DataType::LoadScript("integer"), 0);
    fields.Add("Relation", DataType::LoadScript("integer"), 0);

    new StructType("s_target_cache_info", fields);
    fields.Clear();

    fields.Add("TargetScore", DataType::LoadScript("integer"), 0);
    fields.Add("OverallAwardType", DataType::LoadScript("integer"), 0);
    fields.Add("AwardString", DataType::LoadScript("integer"), 0);
    fields.Add("TutAwardString", DataType::LoadScript("integer"), 0);

    new StructType("s_Overall_Award", fields);
    fields.Clear();

    fields.Add("NotifyThis", DataType::LoadScript("Entity"), 0);
    fields.Add("RequiredScore", DataType::LoadScript("integer"), 0);

    new StructType("s_ScoreNotification", fields);
    fields.Clear();

    fields.Add("position", DataType::LoadScript("vector"), 0);
    fields.Add("normal", DataType::LoadScript("vector"), 0);
    fields.Add("type", DataType::LoadScript("integer"), 0);

    new StructType("s_waiting_decal", fields);
    fields.Clear();

    fields.Add("particle", DataType::LoadScript("Entity"), 0);
    fields.Add("target", DataType::LoadScript("Entity"), 0);
    fields.Add("impact_pos", DataType::LoadScript("vector"), 0);
    fields.Add("impact_orient", DataType::LoadScript("quaternion"), 0);

    new StructType("s_waiting_particle", fields);
    fields.Clear();

    fields.Add("position", DataType::LoadScript("vector"), 0);
    fields.Add("normal", DataType::LoadScript("vector"), 0);
    fields.Add("impact_dir", DataType::LoadScript("vector"), 0);
    fields.Add("size", DataType::LoadScript("number"), 0);
    fields.Add("type", DataType::LoadScript("integer"), 0);
    fields.Add("probe", DataType::LoadScript("Entity"), 0);
    fields.Add("ignore", DataType::LoadScript("Entity"), 0);

    new StructType("s_waiting_blood_decal", fields);
    fields.Clear();

    fields.Add("dist", DataType::LoadScript("number"), 0);
    fields.Add("FromPoint", DataType::LoadScript("Entity"), 0);
    fields.Add("ToPoint", DataType::LoadScript("Entity"), 0);
    fields.Add("OneOverLength", DataType::LoadScript("number"), 0);
    fields.Add("Status", DataType::LoadScript("integer"), 0);
    fields.Add("EndStatus", DataType::LoadScript("integer"), 0);
    fields.Add("Type", DataType::LoadScript("integer"), 0);
    fields.Add("Queue", DataType::LoadScript("list(Entity)"), 0);

    new StructType("s_traffic_section", fields);
    fields.Clear();

    fields.Add("CaptionIndex", DataType::LoadScript("integer"), 0);
    fields.Add("Tutorials", DataType::LoadScript("list(Entity)"), 0);

    new StructType("s_TutorialMenu", fields);
    fields.Clear();

    fields.Add("obj_text", DataType::LoadScript("integer"), 0);
    fields.Add("obj_text_alt", DataType::LoadScript("integer"), 0);
    fields.Add("use_pause_mode", DataType::LoadScript("truth"), 0);
    fields.Add("delay", DataType::LoadScript("number"), 0);
    fields.Add("justify", DataType::LoadScript("truth"), 0);

    new StructType("s_TutorialPopUp", fields);
    fields.Clear();

    fields.Add("logic_condition", DataType::LoadScript("integer"), 0);
    fields.Add("actor", DataType::LoadScript("Entity"), 0);

    new StructType("s_PendingLogicCondition", fields);
    fields.Clear();

    fields.Add("TutorialText", DataType::LoadScript("integer"), 0);
    fields.Add("index_list", DataType::LoadScript("list(integer)"), 0);

    new StructType("s_TutorialIndex", fields);
    fields.Clear();

    fields.Add("LanguageName", DataType::LoadScript("string"), 0);
    fields.Add("Language", DataType::LoadScript("integer"), 0);
    fields.Add("Flag", DataType::LoadScript("Entity"), 0);
    fields.Add("TextB", DataType::LoadScript("Entity"), 0);
    fields.Add("LanguageCode", DataType::LoadScript("string"), 0);

    new StructType("s_Language", fields);
    fields.Clear();

    fields.Add("unit", DataType::LoadScript("Entity"), 0);
    fields.Add("unit_pos", DataType::LoadScript("vector"), 0);
    fields.Add("unit_orient", DataType::LoadScript("quaternion"), 0);
    fields.Add("unit_velocity", DataType::LoadScript("vector"), 0);

    new StructType("element", fields);
    fields.Clear();

    fields.Add("unit", DataType::LoadScript("Entity"), 0);
    fields.Add("hinge", DataType::LoadScript("vector"), 0);
    fields.Add("unit_orient", DataType::LoadScript("quaternion"), 0);
    fields.Add("unit_angle_velocity", DataType::LoadScript("number"), 0);
    fields.Add("unit_angle", DataType::LoadScript("number"), 0);
    fields.Add("unit_max_angle", DataType::LoadScript("number"), 0);
    fields.Add("unit_zones", DataType::LoadScript("integer"), 0);
    fields.Add("unit_in_use", DataType::LoadScript("integer"), 0);
    fields.Add("unit_rotation_inverse", DataType::LoadScript("number"), 0);
    fields.Add("unit_bent_step", DataType::LoadScript("integer"), 0);
    fields.Add("unit_grav_direction", DataType::LoadScript("number"), 0);

    new StructType("hinge_element", fields);
    fields.Clear();

    fields.Add("parameter_name", DataType::LoadScript("string"), 0);
    fields.Add("parameter_value", DataType::LoadScript("number"), 0);

    new StructType("parameter", fields);
    fields.Clear();

    fields.Add("crossblend_speed", DataType::LoadScript("number"), 0);
    fields.Add("anim_slot", DataType::LoadScript("Entity"), 0);

    new StructType("crossblend_override", fields);
    fields.Clear();

    if (GlobalPropertiesList.size() == SCRIPT_PROPERTIES_BUILTIN_TOTAL)
#ifdef INCLUDE_FIXES
        LogDump::LogA("%s built-in properties initialized.\n", GlobalCommandsList.size());
#else
        GlobalCommandsList.size(); // NOTE: why?
#endif

    if (GetGlobalPropertyListChecksum() == SCRIPT_PROPERTIES_BUILTIN_CRC)
        GetGlobalCommandListChecksum();

#ifdef INCLUDE_FIXES
    char path[1024] = {};
    g_AssetManager->GetPlatformSpecificPath(path, "/data/scripts/stable/Database.bin", "bin", AssetManager::PlatformId::PC);
    ReadDatabaseFile(path);
#else
    String tmpstr;
    g_AssetManager->GetPlatformSpecificPath(tmpstr, "/data/scripts/stable/Database.bin", "bin", AssetManager::PlatformId::PC);
    ReadDatabaseFile(tmpstr.m_Str);
#endif

    if (GlobalPropertiesList.size() == SCRIPT_PROPERTIES_TOTAL)
#ifdef INCLUDE_FIXES
        LogDump::LogA("%s more properties loaded.\n", GlobalCommandsList.size());
#else
        GlobalCommandsList.size(); // NOTE: why?
#endif

    if (GetGlobalPropertyListChecksum() == SCRIPT_PROPERTIES_LOADED_CRC)
        GetGlobalCommandListChecksum();

    // NOTE: global variables?
    GlobalScriptsArray[65] = DataType::LoadScript("list(entity)");
    GlobalScriptsArray[50] = DataType::LoadScript("list(string)");
    GlobalScriptsArray[334] = DataType::LoadScript("list(truth)");
    GlobalScriptsArray[203] = DataType::LoadScript("list(list(entity))");
    GlobalScriptsArray[18] = DataType::LoadScript("list(number)");
    GlobalScriptsArray[380] = DataType::LoadScript("list(vector)");
    GlobalScriptsArray[25] = DataType::LoadScript("list(quaternion)");
    GlobalScriptsArray[372] = DataType::LoadScript("list(s_aipassenger)");
    GlobalScriptsArray[251] = DataType::LoadScript("s_aipassenger");
    GlobalScriptsArray[58] = DataType::LoadScript("list(list(integer))");
    GlobalScriptsArray[216] = DataType::LoadScript("list(integer)");
    GlobalScriptsArray[159] = DataType::LoadScript("list(s_traffic_section)");
    GlobalScriptsArray[152] = DataType::LoadScript("s_traffic_section");
    GlobalScriptsArray[28] = DataType::LoadScript("list(element)");
    GlobalScriptsArray[253] = DataType::LoadScript("list(hinge_element)");
    GlobalScriptsArray[273] = DataType::LoadScript("element");
    GlobalScriptsArray[279] = DataType::LoadScript("hinge_element");
    GlobalScriptsArray[49] = DataType::LoadScript("list(parameter)");
    GlobalScriptsArray[369] = DataType::LoadScript("list(take_a_hit_info)");
    GlobalScriptsArray[381] = DataType::LoadScript("parameter");
    GlobalScriptsArray[365] = DataType::LoadScript("list(s_tutorialmenu)");
    GlobalScriptsArray[290] = DataType::LoadScript("list(s_mission_data)");
    GlobalScriptsArray[92] = DataType::LoadScript("s_tutorialmenu");
    GlobalScriptsArray[218] = DataType::LoadScript("dict(integer)");
    GlobalScriptsArray[277] = DataType::LoadScript("s_mission_data");
    GlobalScriptsArray[336] = DataType::LoadScript("list(s_genericres)");
    GlobalScriptsArray[255] = DataType::LoadScript("s_genericres");
    GlobalScriptsArray[403] = DataType::LoadScript("list(list(s_genericres))");
    GlobalScriptsArray[148] = DataType::LoadScript("list(s_vehiclecollision)");
    GlobalScriptsArray[187] = DataType::LoadScript("s_vehiclecollision");
    GlobalScriptsArray[345] = DataType::LoadScript("list(s_target_cache_info)");
    GlobalScriptsArray[107] = DataType::LoadScript("s_target_cache_info");
    GlobalScriptsArray[179] = DataType::LoadScript("list(s_ctrlbone)");
    GlobalScriptsArray[370] = DataType::LoadScript("list(s_entity_pair)");
    GlobalScriptsArray[296] = DataType::LoadScript("s_entity_pair");
    GlobalScriptsArray[248] = DataType::LoadScript("list(list(truth))");
    GlobalScriptsArray[52] = DataType::LoadScript("list(s_neighborhood_config)");
    GlobalScriptsArray[134] = DataType::LoadScript("s_neighborhood_config");
    GlobalScriptsArray[69] = DataType::LoadScript("list(s_pendinglogiccondition)");
    GlobalScriptsArray[136] = DataType::LoadScript("s_pendinglogiccondition");
    GlobalScriptsArray[243] = DataType::LoadScript("list(s_pedestrian_points)");
    GlobalScriptsArray[43] = DataType::LoadScript("s_pedestrian_points");
    GlobalScriptsArray[306] = DataType::LoadScript("list(s_particel_dam)");
    GlobalScriptsArray[326] = DataType::LoadScript("s_particel_dam");
    GlobalScriptsArray[356] = DataType::LoadScript("list(s_list_dist)");
    GlobalScriptsArray[401] = DataType::LoadScript("s_list_dist");
    GlobalScriptsArray[182] = DataType::LoadScript("list(s_fadeentity)");
    GlobalScriptsArray[20] = DataType::LoadScript("s_fadeentity");
    GlobalScriptsArray[83] = DataType::LoadScript("list(s_blockload)");
    GlobalScriptsArray[95] = DataType::LoadScript("s_blockload");
    GlobalScriptsArray[119] = DataType::LoadScript("list(s_delayedmessage)");
    GlobalScriptsArray[165] = DataType::LoadScript("s_delayedmessage");
    GlobalScriptsArray[5] = DataType::LoadScript("list(s_waiting_blood_decal)");
    GlobalScriptsArray[294] = DataType::LoadScript("s_waiting_blood_decal");
    GlobalScriptsArray[147] = DataType::LoadScript("take_a_hit_info");
    GlobalScriptsArray[368] = DataType::LoadScript("list(s_overall_award)");
    GlobalScriptsArray[146] = DataType::LoadScript("list(s_waiting_decal)");
    GlobalScriptsArray[242] = DataType::LoadScript("list(s_waiting_particle)");
    GlobalScriptsArray[56] = DataType::LoadScript("s_waiting_decal");
    GlobalScriptsArray[68] = DataType::LoadScript("s_waiting_particle");
    GlobalScriptsArray[81] = DataType::LoadScript("list(s_language)");
    GlobalScriptsArray[247] = DataType::LoadScript("s_language");
    GlobalScriptsArray[105] = DataType::LoadScript("list(s_tutorialindex)");
    GlobalScriptsArray[104] = DataType::LoadScript("s_tutorialpopup");
    GlobalScriptsArray[94] = DataType::LoadScript("s_tutorialindex");
    GlobalScriptsArray[36] = DataType::LoadScript("list(s_tutorialpopup)");
    GlobalScriptsArray[158] = DataType::LoadScript("list(s_bouncetext)");
    GlobalScriptsArray[131] = DataType::LoadScript("s_bouncetext");
    GlobalScriptsArray[47] = DataType::LoadScript("list(s_taxizapoptions)");
    GlobalScriptsArray[265] = DataType::LoadScript("s_taxizapoptions");
    GlobalScriptsArray[231] = DataType::LoadScript("list(s_radarblip)");
    GlobalScriptsArray[169] = DataType::LoadScript("s_radarblip");
    GlobalScriptsArray[118] = DataType::LoadScript("list(s_powerupsprite)");
    GlobalScriptsArray[406] = DataType::LoadScript("s_powerupsprite");
    GlobalScriptsArray[72] = DataType::LoadScript("s_overall_award");
    GlobalScriptsArray[180] = DataType::LoadScript("list(s_scorebar)");
    GlobalScriptsArray[8] = DataType::LoadScript("list(s_menuoption)");
    GlobalScriptsArray[177] = DataType::LoadScript("s_scorebar");
    GlobalScriptsArray[343] = DataType::LoadScript("s_menuoption");
    GlobalScriptsArray[93] = DataType::LoadScript("list(s_healthwatchedentity)");
    GlobalScriptsArray[188] = DataType::LoadScript("s_healthwatchedentity");
    GlobalScriptsArray[108] = DataType::LoadScript("s_comboawardstring");
    GlobalScriptsArray[363] = DataType::LoadScript("list(s_comboaward)");
    GlobalScriptsArray[207] = DataType::LoadScript("s_comboaward");
    GlobalScriptsArray[292] = DataType::LoadScript("s_collisioninfo");
    GlobalScriptsArray[91] = DataType::LoadScript("list(s_scorenotification)");
    GlobalScriptsArray[314] = DataType::LoadScript("s_scorenotification");
    GlobalScriptsArray[138] = DataType::LoadScript("list(crossblend_override)");
    GlobalScriptsArray[1] = DataType::LoadScript("crossblend_override");
    GlobalScriptsArray[407] = DataType::LoadScript("list(s_uniquecombomove)");
    GlobalScriptsArray[15] = DataType::LoadScript("s_uniquecombomove");
    GlobalScriptsArray[221] = DataType::LoadScript("s_ctrlbone");
    GlobalScriptsArray[400] = DataType::LoadScript("random_sound");

    // NOTE: register game scripts.
    //  TODO: probably, these should be moved on top of this source file, only call actual Create functions here.
#include "scripts/common.h"
#include "scripts/cache.h"
#include "scripts/common_trigger.h"
#include "scripts/common_animslot.h"
#include "scripts/IntroCommon.h"
#include "scripts/vehicle_sub_part_descriptor.h"
//#include "scripts/Vehicle_sound_ctrl.h"
//#include "scripts/vehicle_root.h"
//#include "scripts/vehicle_navigator.h"
//#include "scripts/vehicle_ai.h"
//#include "scripts/VehicleRepulsor.h"
#include "scripts/HUD_health_bar.h"
#include "scripts/master_mission_ctrl.h"
#include "scripts/master_game_pulse_ctrl.h"
#include "scripts/Trigger_Activate_Weapons.h"
#include "scripts/Hydrant_Root.h"
#include "scripts/Ladder_Root.h"
#include "scripts/Magnet.h"
#include "scripts/MarcoDist.h"
#include "scripts/Pathtype.h"
// TODO: much much more.

    if (GetGlobalPropertyListChecksum() == SCRIPT_PROPERTIES_LOADED_CRC)
        GetGlobalCommandListChecksum();

    GlobalScriptsArray[350] = Scriptbaked::GetGlobalScriptById(0);
    GlobalScriptsArray[240] = Scriptbaked::GetGlobalScriptById(1);
    GlobalScriptsArray[310] = Scriptbaked::GetGlobalScriptById(213);
    GlobalScriptsArray[226] = Scriptbaked::GetGlobalScriptById(2);
    GlobalScriptsArray[409] = (void*)Scriptbaked::GetScriptIdByName("common");
    GlobalScriptsArray[274] = (void*)Scriptbaked::GetScriptIdByName("cache");
    GlobalScriptsArray[357] = Scriptbaked::GetGlobalScriptById(152);
    GlobalScriptsArray[303] = Scriptbaked::GetGlobalScriptById(361);
    GlobalScriptsArray[235] = Scriptbaked::GetGlobalScriptById(3);
    GlobalScriptsArray[313] = Scriptbaked::GetGlobalScriptById(4);
    GlobalScriptsArray[285] = Scriptbaked::GetGlobalScriptById(6);
    GlobalScriptsArray[278] = Scriptbaked::GetGlobalScriptById(371);
    GlobalScriptsArray[21] = Scriptbaked::GetGlobalScriptById(387);
    GlobalScriptsArray[59] = Scriptbaked::GetGlobalScriptById(373);
    GlobalScriptsArray[239] = Scriptbaked::GetGlobalScriptById(7);
    GlobalScriptsArray[311] = Scriptbaked::GetGlobalScriptById(390);
    GlobalScriptsArray[286] = Scriptbaked::GetGlobalScriptById(13);
    GlobalScriptsArray[190] = Scriptbaked::GetGlobalScriptById(9);
    GlobalScriptsArray[74] = Scriptbaked::GetGlobalScriptById(78);
    GlobalScriptsArray[215] = Scriptbaked::GetGlobalScriptById(8);
    GlobalScriptsArray[237] = Scriptbaked::GetGlobalScriptById(14);
    GlobalScriptsArray[382] = Scriptbaked::GetGlobalScriptById(79);
    GlobalScriptsArray[234] = Scriptbaked::GetGlobalScriptById(89);
    GlobalScriptsArray[238] = Scriptbaked::GetGlobalScriptById(380);
    GlobalScriptsArray[30] = Scriptbaked::GetGlobalScriptById(273);
    GlobalScriptsArray[170] = Scriptbaked::GetGlobalScriptById(392);
    GlobalScriptsArray[194] = Scriptbaked::GetGlobalScriptById(116);
    GlobalScriptsArray[376] = Scriptbaked::GetGlobalScriptById(137);
    GlobalScriptsArray[399] = Scriptbaked::GetGlobalScriptById(374);
    GlobalScriptsArray[291] = Scriptbaked::GetGlobalScriptById(10);
    GlobalScriptsArray[404] = Scriptbaked::GetGlobalScriptById(11);
    GlobalScriptsArray[287] = Scriptbaked::GetGlobalScriptById(12);
    GlobalScriptsArray[383] = Scriptbaked::GetGlobalScriptById(164);
    GlobalScriptsArray[300] = Scriptbaked::GetGlobalScriptById(395);
    GlobalScriptsArray[86] = Scriptbaked::GetGlobalScriptById(16);
    GlobalScriptsArray[322] = Scriptbaked::GetGlobalScriptById(17);
    GlobalScriptsArray[122] = Scriptbaked::GetGlobalScriptById(19);
    GlobalScriptsArray[335] = Scriptbaked::GetGlobalScriptById(20);
    GlobalScriptsArray[364] = Scriptbaked::GetGlobalScriptById(21);
    GlobalScriptsArray[318] = Scriptbaked::GetGlobalScriptById(22);
    GlobalScriptsArray[13] = Scriptbaked::GetGlobalScriptById(23);
    GlobalScriptsArray[305] = Scriptbaked::GetGlobalScriptById(25);
    GlobalScriptsArray[61] = Scriptbaked::GetGlobalScriptById(26);
    GlobalScriptsArray[57] = Scriptbaked::GetGlobalScriptById(27);
    GlobalScriptsArray[155] = Scriptbaked::GetGlobalScriptById(28);
    GlobalScriptsArray[320] = Scriptbaked::GetGlobalScriptById(29);
    GlobalScriptsArray[339] = Scriptbaked::GetGlobalScriptById(30);
    GlobalScriptsArray[139] = Scriptbaked::GetGlobalScriptById(31);
    GlobalScriptsArray[284] = Scriptbaked::GetGlobalScriptById(32);
    GlobalScriptsArray[327] = Scriptbaked::GetGlobalScriptById(33);
    GlobalScriptsArray[340] = (void*)Scriptbaked::GetScriptIdByName("common_trigger");
    GlobalScriptsArray[378] = Scriptbaked::GetGlobalScriptById(34);
    GlobalScriptsArray[250] = Scriptbaked::GetGlobalScriptById(35);
    GlobalScriptsArray[110] = Scriptbaked::GetGlobalScriptById(36);
    GlobalScriptsArray[244] = Scriptbaked::GetGlobalScriptById(37);
    GlobalScriptsArray[167] = Scriptbaked::GetGlobalScriptById(38);
    GlobalScriptsArray[183] = Scriptbaked::GetGlobalScriptById(39);
    GlobalScriptsArray[220] = Scriptbaked::GetGlobalScriptById(40);
    GlobalScriptsArray[144] = Scriptbaked::GetGlobalScriptById(41);
    GlobalScriptsArray[0] = Scriptbaked::GetGlobalScriptById(42);
    GlobalScriptsArray[228] = Scriptbaked::GetGlobalScriptById(43);
    GlobalScriptsArray[256] = Scriptbaked::GetGlobalScriptById(91);
    GlobalScriptsArray[353] = Scriptbaked::GetGlobalScriptById(299);
    GlobalScriptsArray[408] = Scriptbaked::GetGlobalScriptById(362);
    GlobalScriptsArray[319] = Scriptbaked::GetGlobalScriptById(223);
    GlobalScriptsArray[166] = Scriptbaked::GetGlobalScriptById(44);
    GlobalScriptsArray[323] = Scriptbaked::GetGlobalScriptById(45);
    GlobalScriptsArray[308] = Scriptbaked::GetGlobalScriptById(46);
    GlobalScriptsArray[161] = Scriptbaked::GetGlobalScriptById(47);
    GlobalScriptsArray[209] = Scriptbaked::GetGlobalScriptById(48);
    GlobalScriptsArray[63] = Scriptbaked::GetGlobalScriptById(49);
    GlobalScriptsArray[99] = Scriptbaked::GetGlobalScriptById(50);
    GlobalScriptsArray[387] = Scriptbaked::GetGlobalScriptById(51);
    GlobalScriptsArray[268] = Scriptbaked::GetGlobalScriptById(52);
    GlobalScriptsArray[393] = Scriptbaked::GetGlobalScriptById(53);
    GlobalScriptsArray[349] = Scriptbaked::GetGlobalScriptById(54);
    GlobalScriptsArray[392] = Scriptbaked::GetGlobalScriptById(55);
    GlobalScriptsArray[29] = Scriptbaked::GetGlobalScriptById(145);
    GlobalScriptsArray[276] = Scriptbaked::GetGlobalScriptById(56);
    GlobalScriptsArray[184] = Scriptbaked::GetGlobalScriptById(57);
    GlobalScriptsArray[232] = Scriptbaked::GetGlobalScriptById(58);
    GlobalScriptsArray[31] = Scriptbaked::GetGlobalScriptById(59);
    GlobalScriptsArray[267] = Scriptbaked::GetGlobalScriptById(60);
    GlobalScriptsArray[289] = Scriptbaked::GetGlobalScriptById(61);
    GlobalScriptsArray[173] = Scriptbaked::GetGlobalScriptById(62);
    GlobalScriptsArray[270] = Scriptbaked::GetGlobalScriptById(372);
    GlobalScriptsArray[384] = Scriptbaked::GetGlobalScriptById(63);
    GlobalScriptsArray[175] = Scriptbaked::GetGlobalScriptById(133);
    GlobalScriptsArray[115] = Scriptbaked::GetGlobalScriptById(64);
    GlobalScriptsArray[338] = Scriptbaked::GetGlobalScriptById(65);
    GlobalScriptsArray[391] = Scriptbaked::GetGlobalScriptById(136);
    GlobalScriptsArray[157] = Scriptbaked::GetGlobalScriptById(66);
    GlobalScriptsArray[151] = Scriptbaked::GetGlobalScriptById(92);
    GlobalScriptsArray[395] = Scriptbaked::GetGlobalScriptById(67);
    GlobalScriptsArray[211] = Scriptbaked::GetGlobalScriptById(68);
    GlobalScriptsArray[219] = Scriptbaked::GetGlobalScriptById(69);
    GlobalScriptsArray[362] = Scriptbaked::GetGlobalScriptById(70);
    GlobalScriptsArray[100] = Scriptbaked::GetGlobalScriptById(71);
    GlobalScriptsArray[347] = Scriptbaked::GetGlobalScriptById(72);
    GlobalScriptsArray[272] = Scriptbaked::GetGlobalScriptById(73);
    GlobalScriptsArray[297] = Scriptbaked::GetGlobalScriptById(74);
    GlobalScriptsArray[236] = Scriptbaked::GetGlobalScriptById(75);
    GlobalScriptsArray[197] = Scriptbaked::GetGlobalScriptById(76);
    GlobalScriptsArray[64] = Scriptbaked::GetGlobalScriptById(77);
    GlobalScriptsArray[55] = Scriptbaked::GetGlobalScriptById(391);
    GlobalScriptsArray[168] = Scriptbaked::GetGlobalScriptById(105);
    GlobalScriptsArray[283] = Scriptbaked::GetGlobalScriptById(80);
    GlobalScriptsArray[301] = Scriptbaked::GetGlobalScriptById(81);
    GlobalScriptsArray[143] = Scriptbaked::GetGlobalScriptById(82);
    GlobalScriptsArray[154] = Scriptbaked::GetGlobalScriptById(83);
    GlobalScriptsArray[133] = Scriptbaked::GetGlobalScriptById(84);
    GlobalScriptsArray[48] = Scriptbaked::GetGlobalScriptById(85);
    GlobalScriptsArray[172] = Scriptbaked::GetGlobalScriptById(86);
    GlobalScriptsArray[181] = Scriptbaked::GetGlobalScriptById(87);
    GlobalScriptsArray[405] = Scriptbaked::GetGlobalScriptById(90);
    GlobalScriptsArray[90] = Scriptbaked::GetGlobalScriptById(93);
    GlobalScriptsArray[229] = Scriptbaked::GetGlobalScriptById(94);
    GlobalScriptsArray[337] = Scriptbaked::GetGlobalScriptById(386);
    GlobalScriptsArray[45] = Scriptbaked::GetGlobalScriptById(95);
    GlobalScriptsArray[97] = Scriptbaked::GetGlobalScriptById(97);
    GlobalScriptsArray[117] = Scriptbaked::GetGlobalScriptById(98);
    GlobalScriptsArray[198] = Scriptbaked::GetGlobalScriptById(99);
    GlobalScriptsArray[360] = Scriptbaked::GetGlobalScriptById(100);
    GlobalScriptsArray[114] = Scriptbaked::GetGlobalScriptById(101);
    GlobalScriptsArray[163] = Scriptbaked::GetGlobalScriptById(102);
    GlobalScriptsArray[271] = Scriptbaked::GetGlobalScriptById(103);
    GlobalScriptsArray[354] = Scriptbaked::GetGlobalScriptById(104);
    GlobalScriptsArray[302] = Scriptbaked::GetGlobalScriptById(106);
    GlobalScriptsArray[73] = Scriptbaked::GetGlobalScriptById(107);
    GlobalScriptsArray[295] = Scriptbaked::GetGlobalScriptById(108);
    GlobalScriptsArray[375] = Scriptbaked::GetGlobalScriptById(109);
    GlobalScriptsArray[71] = Scriptbaked::GetGlobalScriptById(256);
    GlobalScriptsArray[196] = Scriptbaked::GetGlobalScriptById(110);
    GlobalScriptsArray[126] = Scriptbaked::GetGlobalScriptById(383);
    GlobalScriptsArray[389] = Scriptbaked::GetGlobalScriptById(139);
    GlobalScriptsArray[142] = Scriptbaked::GetGlobalScriptById(389);
    GlobalScriptsArray[396] = Scriptbaked::GetGlobalScriptById(111);
    GlobalScriptsArray[53] = Scriptbaked::GetGlobalScriptById(112);
    GlobalScriptsArray[275] = Scriptbaked::GetGlobalScriptById(396);
    GlobalScriptsArray[386] = Scriptbaked::GetGlobalScriptById(113);
    GlobalScriptsArray[205] = Scriptbaked::GetGlobalScriptById(114);
    GlobalScriptsArray[124] = Scriptbaked::GetGlobalScriptById(115);
    GlobalScriptsArray[123] = Scriptbaked::GetGlobalScriptById(117);
    GlobalScriptsArray[245] = Scriptbaked::GetGlobalScriptById(150);
    GlobalScriptsArray[84] = Scriptbaked::GetGlobalScriptById(120);
    GlobalScriptsArray[200] = Scriptbaked::GetGlobalScriptById(121);
    GlobalScriptsArray[266] = Scriptbaked::GetGlobalScriptById(122);
    GlobalScriptsArray[252] = Scriptbaked::GetGlobalScriptById(124);
    GlobalScriptsArray[171] = Scriptbaked::GetGlobalScriptById(125);
    GlobalScriptsArray[132] = Scriptbaked::GetGlobalScriptById(385);
    GlobalScriptsArray[185] = Scriptbaked::GetGlobalScriptById(126);
    GlobalScriptsArray[397] = Scriptbaked::GetGlobalScriptById(127);
    GlobalScriptsArray[217] = Scriptbaked::GetGlobalScriptById(129);
    GlobalScriptsArray[254] = Scriptbaked::GetGlobalScriptById(130);
    GlobalScriptsArray[164] = Scriptbaked::GetGlobalScriptById(131);
    GlobalScriptsArray[371] = Scriptbaked::GetGlobalScriptById(132);
    GlobalScriptsArray[309] = Scriptbaked::GetGlobalScriptById(134);
    GlobalScriptsArray[202] = Scriptbaked::GetGlobalScriptById(135);
    GlobalScriptsArray[34] = Scriptbaked::GetGlobalScriptById(219);
    GlobalScriptsArray[17] = Scriptbaked::GetGlobalScriptById(138);
    GlobalScriptsArray[233] = Scriptbaked::GetGlobalScriptById(397);
    GlobalScriptsArray[269] = Scriptbaked::GetGlobalScriptById(140);
    GlobalScriptsArray[153] = Scriptbaked::GetGlobalScriptById(141);
    GlobalScriptsArray[128] = Scriptbaked::GetGlobalScriptById(142);
    GlobalScriptsArray[342] = Scriptbaked::GetGlobalScriptById(143);
    GlobalScriptsArray[402] = Scriptbaked::GetGlobalScriptById(144);
    GlobalScriptsArray[137] = Scriptbaked::GetGlobalScriptById(147);
    GlobalScriptsArray[112] = Scriptbaked::GetGlobalScriptById(148);
    GlobalScriptsArray[76] = Scriptbaked::GetGlobalScriptById(149);
    GlobalScriptsArray[341] = Scriptbaked::GetGlobalScriptById(151);
    GlobalScriptsArray[377] = Scriptbaked::GetGlobalScriptById(154);
    GlobalScriptsArray[355] = (void*)Scriptbaked::GetScriptIdByName("introcommon");
    GlobalScriptsArray[330] = Scriptbaked::GetGlobalScriptById(155);
    GlobalScriptsArray[374] = Scriptbaked::GetGlobalScriptById(156);
    GlobalScriptsArray[79] = Scriptbaked::GetGlobalScriptById(157);
    GlobalScriptsArray[150] = Scriptbaked::GetGlobalScriptById(158);
    GlobalScriptsArray[293] = Scriptbaked::GetGlobalScriptById(159);
    GlobalScriptsArray[37] = Scriptbaked::GetGlobalScriptById(160);
    GlobalScriptsArray[352] = Scriptbaked::GetGlobalScriptById(161);
    GlobalScriptsArray[9] = Scriptbaked::GetGlobalScriptById(162);
    GlobalScriptsArray[80] = Scriptbaked::GetGlobalScriptById(163);
    GlobalScriptsArray[11] = Scriptbaked::GetGlobalScriptById(165);
    GlobalScriptsArray[40] = Scriptbaked::GetGlobalScriptById(166);
    GlobalScriptsArray[22] = Scriptbaked::GetGlobalScriptById(167);
    GlobalScriptsArray[149] = Scriptbaked::GetGlobalScriptById(168);
    GlobalScriptsArray[379] = Scriptbaked::GetGlobalScriptById(169);
    GlobalScriptsArray[264] = Scriptbaked::GetGlobalScriptById(170);
    GlobalScriptsArray[262] = Scriptbaked::GetGlobalScriptById(171);
    GlobalScriptsArray[10] = Scriptbaked::GetGlobalScriptById(172);
    GlobalScriptsArray[141] = Scriptbaked::GetGlobalScriptById(173);
    GlobalScriptsArray[257] = Scriptbaked::GetGlobalScriptById(174);
    GlobalScriptsArray[333] = Scriptbaked::GetGlobalScriptById(175);
    GlobalScriptsArray[331] = Scriptbaked::GetGlobalScriptById(176);
    GlobalScriptsArray[227] = Scriptbaked::GetGlobalScriptById(177);
    GlobalScriptsArray[263] = Scriptbaked::GetGlobalScriptById(178);
    GlobalScriptsArray[191] = Scriptbaked::GetGlobalScriptById(179);
    GlobalScriptsArray[315] = Scriptbaked::GetGlobalScriptById(180);
    GlobalScriptsArray[121] = Scriptbaked::GetGlobalScriptById(181);
    GlobalScriptsArray[298] = Scriptbaked::GetGlobalScriptById(182);
    GlobalScriptsArray[60] = Scriptbaked::GetGlobalScriptById(183);
    GlobalScriptsArray[116] = Scriptbaked::GetGlobalScriptById(184);
    GlobalScriptsArray[62] = Scriptbaked::GetGlobalScriptById(185);
    GlobalScriptsArray[317] = Scriptbaked::GetGlobalScriptById(186);
    GlobalScriptsArray[103] = Scriptbaked::GetGlobalScriptById(187);
    GlobalScriptsArray[160] = Scriptbaked::GetGlobalScriptById(188);
    GlobalScriptsArray[174] = Scriptbaked::GetGlobalScriptById(189);
    GlobalScriptsArray[312] = Scriptbaked::GetGlobalScriptById(190);
    GlobalScriptsArray[348] = Scriptbaked::GetGlobalScriptById(191);
    GlobalScriptsArray[44] = Scriptbaked::GetGlobalScriptById(192);
    GlobalScriptsArray[3] = Scriptbaked::GetGlobalScriptById(193);
    GlobalScriptsArray[24] = Scriptbaked::GetGlobalScriptById(194);
    GlobalScriptsArray[41] = Scriptbaked::GetGlobalScriptById(195);
    GlobalScriptsArray[258] = Scriptbaked::GetGlobalScriptById(196);
    GlobalScriptsArray[102] = Scriptbaked::GetGlobalScriptById(199);
    GlobalScriptsArray[70] = Scriptbaked::GetGlobalScriptById(200);
    GlobalScriptsArray[280] = Scriptbaked::GetGlobalScriptById(201);
    GlobalScriptsArray[101] = Scriptbaked::GetGlobalScriptById(202);
    GlobalScriptsArray[304] = Scriptbaked::GetGlobalScriptById(203);
    GlobalScriptsArray[87] = Scriptbaked::GetGlobalScriptById(204);
    GlobalScriptsArray[35] = Scriptbaked::GetGlobalScriptById(206);
    GlobalScriptsArray[156] = Scriptbaked::GetGlobalScriptById(208);
    GlobalScriptsArray[33] = Scriptbaked::GetGlobalScriptById(209);
    GlobalScriptsArray[26] = Scriptbaked::GetGlobalScriptById(211);
    GlobalScriptsArray[109] = Scriptbaked::GetGlobalScriptById(212);
    GlobalScriptsArray[316] = Scriptbaked::GetGlobalScriptById(215);
    GlobalScriptsArray[77] = Scriptbaked::GetGlobalScriptById(216);
    GlobalScriptsArray[178] = Scriptbaked::GetGlobalScriptById(217);
    GlobalScriptsArray[208] = Scriptbaked::GetGlobalScriptById(218);
    GlobalScriptsArray[14] = Scriptbaked::GetGlobalScriptById(220);
    GlobalScriptsArray[241] = Scriptbaked::GetGlobalScriptById(221);
    GlobalScriptsArray[145] = Scriptbaked::GetGlobalScriptById(222);
    GlobalScriptsArray[113] = Scriptbaked::GetGlobalScriptById(322);
    GlobalScriptsArray[189] = Scriptbaked::GetGlobalScriptById(224);
    GlobalScriptsArray[4] = Scriptbaked::GetGlobalScriptById(225);
    GlobalScriptsArray[195] = Scriptbaked::GetGlobalScriptById(226);
    GlobalScriptsArray[390] = Scriptbaked::GetGlobalScriptById(227);
    GlobalScriptsArray[32] = Scriptbaked::GetGlobalScriptById(228);
    GlobalScriptsArray[367] = Scriptbaked::GetGlobalScriptById(229);
    GlobalScriptsArray[224] = Scriptbaked::GetGlobalScriptById(231);
    GlobalScriptsArray[388] = Scriptbaked::GetGlobalScriptById(232);
    GlobalScriptsArray[206] = Scriptbaked::GetGlobalScriptById(234);
    GlobalScriptsArray[127] = Scriptbaked::GetGlobalScriptById(235);
    GlobalScriptsArray[88] = Scriptbaked::GetGlobalScriptById(236);
    GlobalScriptsArray[246] = Scriptbaked::GetGlobalScriptById(237);
    GlobalScriptsArray[16] = Scriptbaked::GetGlobalScriptById(241);
    GlobalScriptsArray[176] = Scriptbaked::GetGlobalScriptById(243);
    GlobalScriptsArray[186] = Scriptbaked::GetGlobalScriptById(244);
    GlobalScriptsArray[261] = Scriptbaked::GetGlobalScriptById(245);
    GlobalScriptsArray[2] = Scriptbaked::GetGlobalScriptById(246);
    GlobalScriptsArray[366] = Scriptbaked::GetGlobalScriptById(247);
    GlobalScriptsArray[130] = Scriptbaked::GetGlobalScriptById(248);
    GlobalScriptsArray[106] = Scriptbaked::GetGlobalScriptById(249);
    GlobalScriptsArray[78] = Scriptbaked::GetGlobalScriptById(252);
    GlobalScriptsArray[96] = Scriptbaked::GetGlobalScriptById(254);
    GlobalScriptsArray[89] = Scriptbaked::GetGlobalScriptById(255);
    GlobalScriptsArray[120] = Scriptbaked::GetGlobalScriptById(257);
    GlobalScriptsArray[212] = Scriptbaked::GetGlobalScriptById(259);
    GlobalScriptsArray[344] = Scriptbaked::GetGlobalScriptById(261);
    GlobalScriptsArray[329] = Scriptbaked::GetGlobalScriptById(262);
    GlobalScriptsArray[213] = Scriptbaked::GetGlobalScriptById(265);
    GlobalScriptsArray[328] = Scriptbaked::GetGlobalScriptById(268);
    GlobalScriptsArray[82] = Scriptbaked::GetGlobalScriptById(269);
    GlobalScriptsArray[249] = Scriptbaked::GetGlobalScriptById(272);
    GlobalScriptsArray[27] = Scriptbaked::GetGlobalScriptById(274);
    GlobalScriptsArray[192] = Scriptbaked::GetGlobalScriptById(275);
    GlobalScriptsArray[321] = Scriptbaked::GetGlobalScriptById(276);
    GlobalScriptsArray[12] = Scriptbaked::GetGlobalScriptById(277);
    GlobalScriptsArray[325] = Scriptbaked::GetGlobalScriptById(282);
    GlobalScriptsArray[225] = Scriptbaked::GetGlobalScriptById(296);
    GlobalScriptsArray[140] = Scriptbaked::GetGlobalScriptById(297);
    GlobalScriptsArray[332] = Scriptbaked::GetGlobalScriptById(298);
    GlobalScriptsArray[299] = Scriptbaked::GetGlobalScriptById(302);
    GlobalScriptsArray[359] = Scriptbaked::GetGlobalScriptById(303);
    GlobalScriptsArray[222] = Scriptbaked::GetGlobalScriptById(304);
    GlobalScriptsArray[38] = Scriptbaked::GetGlobalScriptById(308);
    GlobalScriptsArray[39] = Scriptbaked::GetGlobalScriptById(310);
    GlobalScriptsArray[135] = Scriptbaked::GetGlobalScriptById(312);
    GlobalScriptsArray[230] = Scriptbaked::GetGlobalScriptById(313);
    GlobalScriptsArray[46] = Scriptbaked::GetGlobalScriptById(314);
    GlobalScriptsArray[259] = Scriptbaked::GetGlobalScriptById(319);
    GlobalScriptsArray[162] = Scriptbaked::GetGlobalScriptById(323);
    GlobalScriptsArray[7] = Scriptbaked::GetGlobalScriptById(327);
    GlobalScriptsArray[307] = Scriptbaked::GetGlobalScriptById(333);
    GlobalScriptsArray[324] = Scriptbaked::GetGlobalScriptById(336);
    GlobalScriptsArray[385] = Scriptbaked::GetGlobalScriptById(339);
    GlobalScriptsArray[98] = Scriptbaked::GetGlobalScriptById(346);
    GlobalScriptsArray[19] = (void*)Scriptbaked::GetScriptIdByName("common_animslot");
    GlobalScriptsArray[85] = Scriptbaked::GetGlobalScriptById(347);
    GlobalScriptsArray[373] = Scriptbaked::GetGlobalScriptById(348);
    GlobalScriptsArray[54] = Scriptbaked::GetGlobalScriptById(351);
    GlobalScriptsArray[51] = Scriptbaked::GetGlobalScriptById(352);
    GlobalScriptsArray[67] = Scriptbaked::GetGlobalScriptById(353);
    GlobalScriptsArray[281] = Scriptbaked::GetGlobalScriptById(354);
    GlobalScriptsArray[223] = Scriptbaked::GetGlobalScriptById(355);
    GlobalScriptsArray[288] = Scriptbaked::GetGlobalScriptById(356);
    GlobalScriptsArray[23] = Scriptbaked::GetGlobalScriptById(357);
    GlobalScriptsArray[42] = Scriptbaked::GetGlobalScriptById(358);
    GlobalScriptsArray[398] = Scriptbaked::GetGlobalScriptById(360);
    GlobalScriptsArray[210] = Scriptbaked::GetGlobalScriptById(363);
    GlobalScriptsArray[358] = Scriptbaked::GetGlobalScriptById(364);
    GlobalScriptsArray[260] = Scriptbaked::GetGlobalScriptById(365);
    GlobalScriptsArray[361] = Scriptbaked::GetGlobalScriptById(366);
    GlobalScriptsArray[6] = Scriptbaked::GetGlobalScriptById(370);
    GlobalScriptsArray[214] = Scriptbaked::GetGlobalScriptById(367);
    GlobalScriptsArray[201] = Scriptbaked::GetGlobalScriptById(368);
    GlobalScriptsArray[204] = Scriptbaked::GetGlobalScriptById(369);
    GlobalScriptsArray[66] = Scriptbaked::GetGlobalScriptById(375);
    GlobalScriptsArray[282] = Scriptbaked::GetGlobalScriptById(388);
    GlobalScriptsArray[351] = Scriptbaked::GetGlobalScriptById(376);
    GlobalScriptsArray[346] = Scriptbaked::GetGlobalScriptById(377);
    GlobalScriptsArray[199] = Scriptbaked::GetGlobalScriptById(378);
    GlobalScriptsArray[394] = Scriptbaked::GetGlobalScriptById(393);
    GlobalScriptsArray[129] = Scriptbaked::GetGlobalScriptById(381);
    GlobalScriptsArray[75] = Scriptbaked::GetGlobalScriptById(382);
    GlobalScriptsArray[111] = Scriptbaked::GetGlobalScriptById(384);
    GlobalScriptsArray[193] = Scriptbaked::GetGlobalScriptById(394);
    GlobalScriptsArray[125] = Scriptbaked::GetGlobalScriptById(398);
}

void GlobalProperty::GetNameAndType(String& outStr) const
{
    if (m_PropertyType != tNOTHING)
    {
        char buf[256] = {};
        strncpy(buf, m_PropertyName, strlen(m_PropertyName));
        buf[strlen(m_PropertyName)] = ':';
        strcat(buf, m_PropertyType->m_TypeName.m_Str);

        outStr = buf;
        return;
    }
    else
        outStr = m_PropertyName;
}

void GlobalProperty::ClearGlobalProperties()
{
    GlobalPropertiesList.clear();
    GlobalPropertiesMap.clear();

    GlobalPropertyListChecksumObtained = false;
}

GlobalProperty* GlobalProperty::GetById(unsigned int id)
{
    return &GlobalPropertiesList[id];
}

GlobalProperty::GlobalProperty(const char* const propertyname, unsigned int ind)
{
    m_PropertyId = ind;

    size_t propnamestrlen = strchr(propertyname, ':') - propertyname;
    m_PropertyName = new char[propnamestrlen + 1];
    strncpy(m_PropertyName, propertyname, propnamestrlen);
    m_PropertyName[propnamestrlen] = NULL;

    m_PropertyType = DataType::GetTypeByName(propertyname + propnamestrlen + 1);
}

GlobalProperty::~GlobalProperty()
{
    if (m_PropertyName)
        delete[] m_PropertyName;
}

void GlobalCommand::AddArgumentType(DataType* argtype)
{
#ifdef INCLUDE_FIXES
    if (!argtype)
    {
        LogDump::LogA("GlobalCommand::AddArgumentType(\"%s\") empty DataType argument passed!\n", m_CommandName);
        return;
    }
#endif
    String emptystr;
    m_Arguments.m_ArgumentsList.emplace_back(emptystr, argtype, m_Arguments.m_TotalSizeBytes);
    m_Arguments.m_TotalSizeBytes += argtype->m_Size;
    m_Arguments.m_TotalSize += argtype->GetTypeSize();
}

void GlobalCommand::GetReturnTypeString(String& outStr)
{
    outStr = m_ArgumentsString;

    if (m_Arguments.m_ArgumentsList[0].m_ScriptType->m_Size)
    {
        char buf[256] = {};

        strcpy(buf, m_ArgumentsString);
        strcat(buf, ":");
        strcat(buf, m_Arguments.m_ArgumentsList[0].m_ScriptType->m_TypeName.m_Str);

        size_t buflen = strlen(buf);

        delete[] m_ArgumentsString;
        m_ArgumentsString = new char[buflen + 1];

        strcpy(m_ArgumentsString, buf);
    }
}

void GlobalCommand::ClearGlobalCommands()
{
    GlobalCommandsList.clear();
    GlobalCommandsMap.clear();

    GlobalCommandListChecksumObtained = false;
}

GlobalCommand::GlobalCommand(const char* const commandname, const unsigned int commandind)
{
    MESSAGE_CLASS_CREATED(GlobalCommand);

    char cmdname[256] = {};
    strcpy(cmdname, commandname);

    m_Arguments.m_TotalSize = NULL;
    m_Arguments.m_TotalSizeBytes = NULL;
    m_GlobalIndex = commandind;

    if (!commandname || !*commandname || !strchr(commandname, ':'))
    {
        AddArgumentType(tNOTHING);
    }
    else
    {
        AddArgumentType(DataType::GetTypeByName(strchr(commandname, ':') + 1));
        *strchr(cmdname, ':') = NULL;
    }

    char* const brcktopenpos = strchr(cmdname, '(');
    const char* const brcktclospos = strrchr(cmdname, ')');

    if (!brcktopenpos || !brcktclospos)
    {
        m_CommandName = _strdup(cmdname);
    }
    else
    {
        if (brcktopenpos + 1 <= brcktclospos)
        {
            char args[256] = {};
            strcpy(args, brcktopenpos + 1);
            *strrchr(args, ')') = NULL;

            char* tok = strtok(args, ",");
            while (tok)
            {
                DataType* argscripttype = DataType::LoadScript(tok);
#ifdef INCLUDE_FIXES
                // NOTE: this should NOT happen!
                if (!argscripttype)
                {
                    LogDump::LogA("*** Failed to load script \"%s\" ( command: \"%s\" ) ***\n", tok, commandname);
                    AddArgumentType(tNOTHING);
                }
                else
#endif     
                    AddArgumentType(argscripttype);
                tok = strtok(NULL, ",");
            }
        }

        *brcktopenpos = NULL;
        m_CommandName = _strdup(cmdname);
    }

    m_ArgumentsString = _strdup(cmdname);
}

GlobalCommand::~GlobalCommand()
{
    MESSAGE_CLASS_DESTROYED(GlobalCommand);

    delete m_ArgumentsString;
    delete m_CommandName;
}

Scriptbaked::Scriptbaked(const char* const scriptName, const char* const parentName, bool a3, bool a4)
{
    m_ScriptSize = 0;
    field_5C = field_5C & 0xFFFFFFFC | (a3 != 0) | (2 * (a4 != 0));

    ScriptsList.push_back(this);

    m_Name = scriptName;
    m_PropertiesBlocksTotal = 0;
    m_BaseEntity = DataType::GetScriptEntityByName(parentName);
    field_60 = 0;
}

void Scriptbaked::AddMember(const int fieldId, char* defaultValue, const int a3)
{
#ifdef INCLUDE_FIXES
    if (fieldId == -1)
    {
        LogDump::LogA("Scriptbaked::AddMember(%d, \"%s\", %d) FAILED!\n", fieldId, defaultValue, a3);
        return;
    }
#endif
    GlobalProperty* propertyRef = GlobalProperty::GetById(fieldId);

#ifdef INCLUDE_FIXES
    assert(propertyRef != nullptr);
#endif

    m_PropertiesValues[fieldId] = m_PropertiesList.size();

    Scriptbaked::Property prop;
    prop.m_Info = propertyRef;
    prop.m_Offset = m_PropertiesValues.size();
    prop.m_DefaultValue = defaultValue;
    prop.field_C = a3;

    m_PropertiesList.push_back(prop);
}

void Scriptbaked::AddMethod(short methodid, void (*scriptthreadhandler)(ScriptThread*), void (*methodptr)(ScriptThread*, void*))
{
    m_MethodsList.push_back({ methodid, 0, scriptthreadhandler, methodptr });
}

void Scriptbaked::CalculateSize()
{
    m_PropertiesBlocksTotal = ((2 * m_PropertiesList.size() + 31) / 32);
    m_ScriptSize = m_ScriptSize + m_PropertiesBlocksTotal;

    if (m_PropertiesList.size() > 0)
    {
        unsigned int j = 0;
        for (unsigned int i = m_PropertiesList.size(); i > 0; --i)
        {
            m_PropertiesList[j].m_Offset = m_ScriptSize;
            m_ScriptSize += m_PropertiesList[j++].m_Info->m_PropertyType->m_Size;
        }
    }

    AssignScriptToEntity(m_BaseEntity);

    if (m_MethodsList.size() > 0)
    {
        for (unsigned int i = 0; i < m_MethodsList.size(); ++i)
        {
            unsigned int j = 1;
            if (i < m_MethodsList.size())
            {
                for (; i < m_MethodsList.size(); ++i, ++j)
                    if (m_MethodsList[i + 1].m_Id != m_MethodsList[i].m_Id)
                        break;
            }
            m_MethodsList[i + 1].m_PropertyBlockId = j;
        }
    }
}

#pragma message(TODO_IMPLEMENTATION)
bool Scriptbaked::_48A7E0(Node* node, int scriptId, void* args)
{
    if (scriptId == -1)
        return false;

    if (!node->m_ScriptData)
        return false;

    if (!node->m_ScriptData->m_ScriptThread)
        return false;

    if (node->m_ScriptData->m_ScriptThread->m_ThreadFlags.Suspended)
        return false;

    return false;
}

void Scriptbaked::ClearEntityProperties(Entity* ent)
{
    if (m_PropertiesList.size())
    {
        for (unsigned int i = 0; i < m_PropertiesList.size(); ++i)
            m_PropertiesList[i].m_Info->m_PropertyType->Delete((char*)&ent->m_Parameters[m_PropertiesList[i].m_Offset]);

        delete ent->m_Parameters;
    }
}

class EntityType* Scriptbaked::GetAttachedScript() const
{
    char buf[256] = {};
    sprintf(buf, "%s(%s)", m_Name.m_Str, m_BaseEntity->m_TypeName.m_Str);

    return DataType::GetScriptEntityByName(buf);
}

const int Scriptbaked::GetPropertiesListSize() const
{
    return m_PropertiesList.size();
}

void Scriptbaked::GetEntityPropertyValue(Entity* ent, const unsigned int propertyindex, int* outPropValue)
{
    unsigned int propertyvaluesize = m_PropertiesList[propertyindex].m_Info->m_PropertyType->m_Size;
    int* entpropertyvalue = &ent->m_Parameters[m_PropertiesList[propertyindex].m_Offset];

    if (propertyvaluesize > 0)
        for (; propertyvaluesize; --propertyvaluesize)
            *outPropValue++ = *entpropertyvalue++;
}

bool Scriptbaked::HasPropertyId(const unsigned int propertyid) const
{
    return m_PropertiesValues.find(propertyid) != m_PropertiesValues.end();
}

void Scriptbaked::CopyScriptParameters(Entity* entity)
{
    int* parameters = new int[m_ScriptSize];

    for (unsigned int i = 0; i < m_PropertiesList.size(); ++i)
        if (m_PropertiesList[i].m_DefaultValue)
            m_PropertiesList[i].m_Info->m_PropertyType->MakeFromString(m_PropertiesList[i].m_DefaultValue, (char*)&parameters[m_PropertiesList[i].m_Offset]);
        else
            m_PropertiesList[i].m_Info->m_PropertyType->ReturnNew(&parameters[m_PropertiesList[i].m_Offset]);

    if (m_PropertiesBlocksTotal)
        memset(parameters, NULL, m_PropertiesBlocksTotal * sizeof(*parameters));

    entity->m_Parameters = parameters;
}

void Scriptbaked::GetMethodParams(void(*methodPtr)(ScriptThread*), std::vector<DataType*>& outParams) const
{
    if (!m_ParametersList.size())
        return;

    unsigned int paramOffset = 0;
    for (unsigned int i = 0; i < m_ParametersList.size(); ++i, paramOffset++)
        if (m_ParametersList[i].m_ProcPtr == methodPtr)
            break;

    if (paramOffset >= m_ParametersList.size())
        return;

    for (; paramOffset < m_ParametersList.size(); ++paramOffset)
    {
        if (m_ParametersList[paramOffset].m_ProcPtr != methodPtr || !m_ParametersList[paramOffset].m_ParamType)
            break;

        outParams.push_back(m_ParametersList[paramOffset].m_ParamType);
    }
}

int Scriptbaked::GetParameterProcedureIndex(void(*procedure)(ScriptThread*)) const
{
    if (!m_ParametersList.size())
        return -1;

    unsigned int index = 0;
    for (std::vector<Parameter>::const_iterator it = m_ParametersList.cbegin(); it->m_ProcPtr != procedure; it++, ++index);

    return index >= m_ParametersList.size() ? -1 : index;
}

const int Scriptbaked::GetPropertyValueByIndex(const int index) const
{
    auto& it = m_PropertiesValues.find(index);
    if (it != m_PropertiesValues.end())
        return (*it).first;
    else
        return -1;
}

void Scriptbaked::AddLocal(void(*procPtr)(ScriptThread*), DataType* localType)
{
    m_ParametersList.push_back({ procPtr, localType });
}

void Scriptbaked::AddProperty(Node* scriptNode, const unsigned int propertyIndex, const int* const propertyValue)
{
    DataType* propertyType = m_PropertiesList[propertyIndex].m_Info->m_PropertyType;
    int* nodePropertyPtr = &scriptNode->m_Parameters[m_PropertiesList[propertyIndex].m_Offset];
    const unsigned int slot = scriptNode->m_Parameters[propertyIndex / 16];
    const unsigned int index = 1 << (2 * (propertyIndex & 15));

    if ( ((slot & index) == 0 || ((2 * index) & slot) == 0) && !propertyType->AreEqual(nodePropertyPtr, propertyValue) )
        scriptNode->_86B560(propertyIndex, nodePropertyPtr);

    if (DataType::IsSimpleType(propertyType))
    {
        DataType::CopyValue(nodePropertyPtr, propertyValue, propertyType->m_Size);
    }
    else
    {
        propertyType->Delete((char*)nodePropertyPtr);
        propertyType->Clone(propertyValue, nodePropertyPtr);
    }
}

void Scriptbaked::AddPropertyByReference(Node* callerNode, const int propertyInd, const void* data)
{
    std::map<int, int>::const_iterator propval = m_PropertiesValues.find(propertyInd);
    if (propval->first)
    {
        AddProperty(callerNode, propertyInd, (const int*)data);
    }
    else
    {
        String buf;
        GlobalProperty::GetById(propertyInd)->GetNameAndType(buf);
#if defined(INCLUDE_FIXES) && defined(VERBOSE_LOGGING)
        //  NOTE: editor/debug leftover?
        debug("AddPropertyByReference: script does not have this propety! id=%d, type=%s", propertyInd, buf.m_Str);
#endif
    }
}

EntityType* Scriptbaked::AssignScriptToEntity(EntityType* parent)
{
    if (!m_BaseEntity)
        return nullptr;

    if (!parent)
    {
        LogDump::LogA("'%s' do not descent from '%s', script '%s' cannot be used on a '%s'\n",
#ifdef INCLUDE_FIXES
            "(null)",
#else
            parent->m_TypeName.m_Str,
#endif
            m_BaseEntity->m_TypeName.m_Str,
            m_Name.m_Str,
#ifdef INCLUDE_FIXES
            "(null)");
#else
            parent->m_TypeName.m_Str);
#endif

        return nullptr;
    }

    EntityType* parentent = parent;
    while (m_BaseEntity != parentent)
    {
        parentent = parent->m_Parent;
        if (!parentent)
        {
            LogDump::LogA("'%s' do not descent from '%s', script '%s' cannot be used on a '%s'\n",
                parent->m_TypeName.m_Str,
                m_BaseEntity->m_TypeName.m_Str,
                m_Name.m_Str,
                parent->m_TypeName.m_Str);

            return nullptr;
        }
    }

    char entname[128] = {};
    sprintf(entname, "%s(%s)", m_Name.m_Str, parent->m_TypeName.m_Str);

    EntityType* ent = new EntityType(entname);
    ent->InheritFrom(parent);
    ent->m_Script = this;
    ent->PropagateProperties();

    return ent;
}

Scriptbaked* Scriptbaked::GetGlobalScriptByName(const char* name)
{
    if (!name || !*name)
        return nullptr;

    if (!ScriptsList.size())
        return nullptr;

    const size_t namelen = strlen(name);
    for (unsigned int i = 0; i < ScriptsList.size(); ++i)
        if (strncmp(ScriptsList[i]->m_Name.m_Str, name, namelen) == NULL)
            return ScriptsList[i];

    return nullptr;
}

Scriptbaked* Scriptbaked::GetGlobalScriptById(const unsigned int id)
{
#ifdef INCLUDE_FIXES
    if (id >= ScriptsList.size())
    {
        LogDump::LogA("GetGlobalScriptById: id=%d out of range!\n", id);
        return nullptr;
    }

    Scriptbaked* script = ScriptsList[id];
    if (!script)
        LogDump::LogA("GetGlobalScriptById: script for id=%d not found!\n", id);

    return script;
#else
    return ScriptsList[id];
#endif
}

int Scriptbaked::GetScriptIdByName(const char* const name)
{
    if (ScriptsList.size() <= 0)
        return -1;

    for (unsigned int i = 0; i < ScriptsList.size(); ++i)
        if ((ScriptsList[i]->field_5C & 1) != 0 && strcmp(ScriptsList[i]->m_Name.m_Str, name) == 0)
            return i;

    return -1;
}

void Scriptbaked::InstantiateGlobalScripts()
{
    if (!Scene::SceneInstance)
        return;

    for (unsigned int i = 0; i < ScriptsList.size(); ++i)
    {
        auto script = ScriptsList[i];
        if (!(script->field_5C & 1))
            continue;

        Node* child = Scene::SceneInstance->m_FirstChild;
        if (child)
        {
            bool foundScript = false;
            while (!foundScript)
            {
                if (script->GetAttachedScript() == child->m_ScriptEntity &&
                    String::EqualIgnoreCase(script->GetAttachedScript()->m_TypeName.m_Str, child->m_Name, strlen(child->m_Name)))
                        foundScript = true;

                child = child->m_NextSibling;
                if (!child)
                {
                    if (foundScript)
                        break;

                    Node* newNode = (Node*)(script->GetAttachedScript()->CreateNode());
                    if (newNode)
                    {
                        EntityType* nnEnt = newNode->m_ScriptEntity;
                        if (nnEnt)
                        {
                            while (tNode != nnEnt)
                            {
                                nnEnt = nnEnt->m_Parent;
                                if (!nnEnt)
                                {
                                    newNode = nullptr;
                                    break;
                                }
                            }
                        }

                        newNode->SetName(script->GetAttachedScript()->m_TypeName.m_Str);
                        newNode->SetParent(Scene::SceneInstance);
                    }
                }
            }
        }
        else
        {
            Node* newNode = (Node*)(script->GetAttachedScript()->CreateNode());
            if (newNode)
            {
                EntityType* nnEnt = newNode->m_ScriptEntity;
                if (nnEnt)
                {
                    while (tNode != nnEnt)
                    {
                        nnEnt = nnEnt->m_Parent;
                        if (!nnEnt)
                        {
                            newNode = nullptr;
                            break;
                        }
                    }
                }

                newNode->SetName(script->GetAttachedScript()->m_TypeName.m_Str);
                newNode->SetParent(Scene::SceneInstance);
            }
        }
    }

    AssignCommonNodes();
}

void Scriptbaked::AssignCommonNodes()
{
    if (ScriptsList.size() > 0 && ScriptsList.size() > SceneScriptEntitiesList.size())
        SceneScriptEntitiesList.resize(ScriptsList.size());

    if (ScriptsList.size() <= 0)
        return;

    for (unsigned int i = 0; i < ScriptsList.size(); ++i)
    {
        auto sc = ScriptsList[i];
        if (!(sc->field_5C & 1))
            continue;

        Node* scriptnode = (Node*)Scene::SceneInstance->FindNode(sc->GetAttachedScript()->m_TypeName.m_Str);
        SceneScriptEntitiesList[i] = scriptnode;

        if (_stricmp(sc->m_Name.m_Str, "cache") == NULL)
            CacheScriptNode = scriptnode;
        else if (_stricmp(sc->m_Name.m_Str, "common") == NULL)
            CommonScriptNode = scriptnode;
        else if (_stricmp(sc->m_Name.m_Str, "common_animslot") == NULL)
            CommonAnimSlotScriptNode = scriptnode;
        else if (_stricmp(sc->m_Name.m_Str, "common_trigger") == NULL)
            CommonTriggerScriptNode = scriptnode;
        else if (_stricmp(sc->m_Name.m_Str, "IntroCommon") == NULL)
            IntroCommonScriptNode = scriptnode;
    }
}

unsigned int Scriptbaked::GetScriptIdByFullName(const char* const name)
{
    const char* ddpos = strchr(name, ':');
    char scriptname[256] = {};
    size_t scriptnamelen = 0;
    if (ddpos)
    {
        strncpy(scriptname, name, ddpos - name);
        scriptname[ddpos - name] = NULL;
        scriptnamelen = ddpos - name;

        for (auto it = GlobalPropertiesList.cbegin(); it != GlobalPropertiesList.cend(); it++)
            if (strncmp(it->m_PropertyName, scriptname, scriptnamelen) == NULL)
                return it->m_PropertyId;
    }
    else
    {
        scriptnamelen = strlen(name);
        for (auto it = GlobalPropertiesList.cbegin(); it != GlobalPropertiesList.cend(); it++)
            if (strncmp(it->m_PropertyName, name, scriptnamelen) == NULL)
                return it->m_PropertyId;
    }

    debug("GetScriptIdByFullName(\"%s\") FAILED!\n", name);
    return -1;
}

GlobalCommand::Argument::Argument(String& argname, const DataType* argtype, unsigned int argsize)
{
    m_PropertyName = argname;
    m_ScriptType = (DataType*)argtype;
    m_TotalSizeBytes = argsize;
}