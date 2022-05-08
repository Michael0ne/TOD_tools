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
DataType* GlobalScriptsArray[410];
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
    char propname[64] = {};
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

short RegisterGlobalProperty(const char* const propertyname, bool existingProperty)
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
    if (existingProperty && propid >= 0)
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

int GetCommandId(const char* const commandname, bool existingCommand)
{
    if (!GlobalCommandsList.size())
        GlobalCommandsList.reserve(3000);

    int cmdid = GetCommandByName(commandname);
    const char* const ddotpos = strchr(commandname, ':');
    if (existingCommand && cmdid >= 0)
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
    File dbfile(path, 0x21, true);

    if (!dbfile.IsFileOpen())
        return;

    DWORD timeStart = Timer::GetMilliseconds();
    unsigned int totalProperties = NULL;
    unsigned int totalCommands = NULL;

    dbfile.Read(&totalProperties, sizeof(totalProperties));
    GlobalPropertiesList.reserve(totalProperties);

    for (unsigned int i = 0; i < totalProperties; i++)
    {
        char propertyName[128] = {};
        unsigned int propertyNameLen = NULL;

        dbfile.Read(&propertyNameLen, sizeof(propertyNameLen));
        dbfile.Read(propertyName, propertyNameLen);

        RegisterGlobalProperty(propertyName, false);
    }

    dbfile.Read(&totalCommands, sizeof(totalCommands));
    GlobalCommandsList.reserve(totalCommands);

    for (unsigned int i = 0; i < totalCommands; i++)
    {
        char commandName[128] = {};
        unsigned int commandNameLen = NULL;

        dbfile.Read(&commandNameLen, sizeof(commandNameLen));
        dbfile.Read(commandName, commandNameLen);

        GetCommandId(commandName, false);
    }

    LogDump::LogA("Done loading script database (%dms)\n", Timer::GetMilliseconds() - timeStart);
#ifdef INCLUDE_FIXES
    LogDump::LogA("Read %d properties and %d commands\n", totalProperties, totalCommands);
#endif
}

bool FindScript(const char* const scriptname, String& zipname)
{
    if (Script::ScriptsPath.m_Length == NULL)
        return false;

    char scriptnamefull[256] = {};
    sprintf(scriptnamefull, "%s.script", scriptname);

    if (File::SearchScriptFile(Script::ScriptsPath.m_Str, scriptnamefull, zipname))
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

    // TODO: assign loaded scripts to global variables here.
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

GlobalProperty& GlobalProperty::GetById(unsigned int id)
{
    return GlobalPropertiesList[id];
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

void Scriptbaked::AddMember(const int fieldId, const char* const defaultValue, const int a3)
{
#ifdef INCLUDE_FIXES
    if (fieldId == -1)
    {
        LogDump::LogA("Scriptbaked::AddMember(%d, \"%s\", %d) FAILED!\n", fieldId, defaultValue, a3);
        return;
    }
#endif
    m_PropertiesValues[fieldId] = m_PropertiesList.size();
    m_PropertiesList.push_back({ &GlobalProperty::GetById(fieldId), m_PropertiesValues.size(), (char*)defaultValue, a3 });
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
        GlobalProperty::GetById(propertyInd).GetNameAndType(buf);
#if defined(INCLUDE_FIXES) && defined(VERBOSE_LOGGING)
        //  NOTE: editor/debug leftover?
        debug("Scriptbaked::489D40: script does not have this propety! id=%d, type=%s", propertyInd, buf.m_Str);
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
    return ScriptsList[id];
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
    // TODO: there's a trouble when looking for an Entity-specific properties. Take that into account?
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