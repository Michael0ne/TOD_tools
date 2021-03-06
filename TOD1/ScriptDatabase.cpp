#include "ScriptDatabase.h"
#include "LogDump.h"
#include "Performance.h"
#include "Globals.h"
#include "EntityType.h"
#include "AssetManager.h"
#include "NothingType.h"
#include "ScriptType.h"
#include "Entity.h"
#include "Scene.h"

std::vector<GlobalProperty>		GlobalPropertiesList;
std::map<String, unsigned int>	GlobalPropertiesMap;
std::vector<GlobalCommand>		GlobalCommandsList;
std::map<String, unsigned int>	GlobalCommandsMap;
std::vector<GlobalScript*> GlobalScript::ScriptsList;
std::vector<Node*> GlobalScript::SceneScriptEntitiesList;

unsigned int	GlobalPropertyListChecksum;
bool			GlobalPropertyListChecksumObtained;
unsigned int	GlobalCommandListChecksum;
bool			GlobalCommandListChecksumObtained;
BaseType*       GlobalScriptsArray[410];
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

            if (checksum_str.m_nLength >= 10000)
                break;
            checksum_str.Append(tempstr.m_szString);
        }
    }

    GlobalPropertyListChecksum = Utils::CalcCRC32(checksum_str.m_szString, checksum_str.m_nLength);
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

            checksum_str.Append(tempstr.m_szString);
        }
    }

    GlobalCommandListChecksum = Utils::CalcCRC32(checksum_str.m_szString, checksum_str.m_nLength);
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

int RegisterGlobalProperty(const char* const propertyname, bool existingProperty)
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
        if (strncmp(GlobalPropertiesList[propid].m_PropertyType->m_TypeName.m_szString, proptype, strlen(proptype)) == NULL)
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
        BaseType::LoadScript(proptype);

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

int RegisterGlobalCommand(const char* const commandname, bool existingCommand)
{
    if (!GlobalCommandsList.size())
        GlobalCommandsList.reserve(3000);

    int cmdid = GetCommandByName(commandname);
    const char* const ddotpos = strchr(commandname, ':');
    if (existingCommand && cmdid >= 0)
    {
        if (ddotpos)
            BaseType::GetTypeByName(ddotpos + 1);

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

    DWORD timeStart = Performance::GetMilliseconds();
    unsigned int	totalProperties = NULL;
    unsigned int	totalCommands = NULL;

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

        RegisterGlobalCommand(commandName, false);
    }

    LogDump::LogA("Done loading script database (%dms)\n", Performance::GetMilliseconds() - timeStart);
#ifdef INCLUDE_FIXES
    LogDump::LogA("Read %d properties and %d commands\n", totalProperties, totalCommands);
#endif
}

bool FindScript(const char* const scriptname, String& zipname)
{
    if (Script::ScriptsPath.m_nLength == NULL)
        return false;

    char scriptnamefull[256] = {};
    sprintf(scriptnamefull, "%s.script", scriptname);

    if (File::SearchScriptFile(Script::ScriptsPath.m_szString, scriptnamefull, zipname))
        return true;

    LogDump::LogA("Script '%s.script' not found (search path is '%s')\n", scriptname, Script::ScriptsPath.m_szString);
    return false;
}

#pragma message(TODO_IMPLEMENTATION)
void LoadScripts()
{
    //	NOTE: register built-in scripts.
    ScriptType::ScriptFieldsList fields;
    fields.Add("RecievingEntity", BaseType::LoadScript("Entity"), 0);
    fields.Add("Damage", BaseType::LoadScript("number"), 0);
    fields.Add("Owner", BaseType::LoadScript("Entity"), 0);
    fields.Add("inflictor_node", BaseType::LoadScript("Entity"), 0);
    fields.Add("RecievingCollVolume", BaseType::LoadScript("Entity"), 0);
    fields.Add("CollisionImpulse", BaseType::LoadScript("vector"), 0);
    fields.Add("Peercontactpoint", BaseType::LoadScript("vector"), 0);
    fields.Add("Damage_type", BaseType::LoadScript("integer"), 0);

    new ScriptType("Take_a_hit_info", fields);
    fields.Clear();

    fields.Add("pBlock", BaseType::LoadScript("Entity"), 0);
    fields.Add("sTeleportName", BaseType::LoadScript("string"), 0);

    new ScriptType("s_BlockLoad", fields);
    fields.Clear();

    fields.Add("pResource", BaseType::LoadScript("Entity"), 0);
    fields.Add("pPlaceholder", BaseType::LoadScript("Entity"), 0);

    new ScriptType("s_GenericRes", fields);
    fields.Clear();

    fields.Add("pPlaceholder", BaseType::LoadScript("Entity"), 0);
    fields.Add("RangeSqr", BaseType::LoadScript("number"), 0);
    fields.Add("bInView", BaseType::LoadScript("truth"), 0);
    fields.Add("bIsVisible", BaseType::LoadScript("truth"), 0);

    new ScriptType("s_PlaceholderPriority", fields);
    fields.Clear();

    fields.Add("RightHandGunList", BaseType::LoadScript("list(truth)"), 0);
    fields.Add("LeftHandGunList", BaseType::LoadScript("list(truth)"), 0);
    fields.Add("AmmoList", BaseType::LoadScript("list(integer)"), 0);
    fields.Add("MaxAmmoList", BaseType::LoadScript("list(integer)"), 0);
    fields.Add("AmmoInPrimWeapon", BaseType::LoadScript("list(integer)"), 0);
    fields.Add("AmmoInSecWeapon", BaseType::LoadScript("list(integer)"), 0);

    new ScriptType("s_Inventory", fields);
    fields.Clear();

    fields.Add("pPlaceHolder", BaseType::LoadScript("Entity"), 0);
    fields.Add("vLocalPosition", BaseType::LoadScript("vector"), 0);

    new ScriptType("s_AIPassenger", fields);
    fields.Clear();

    fields.Add("PowerUpList", BaseType::LoadScript("list(integer)"), 0);
    fields.Add("MaxPowerUpList", BaseType::LoadScript("list(integer)"), 0);

    new ScriptType("s_PowerUpInventory", fields);
    fields.Clear();

    fields.Add("Type", BaseType::LoadScript("integer"), 0);
    fields.Add("Sprite", BaseType::LoadScript("Entity"), 0);
    fields.Add("BGSprite", BaseType::LoadScript("Entity"), 0);
    fields.Add("OrgSize", BaseType::LoadScript("vector"), 0);
    fields.Add("NormalSize", BaseType::LoadScript("vector"), 0);
    fields.Add("NormalPos", BaseType::LoadScript("vector"), 0);
    fields.Add("SelectedSize", BaseType::LoadScript("vector"), 0);
    fields.Add("SelectedPos", BaseType::LoadScript("vector"), 0);
    fields.Add("AmountTB", BaseType::LoadScript("Entity"), 0);
    fields.Add("AmountTB_BG", BaseType::LoadScript("Entity"), 0);

    new ScriptType("s_PowerUpSprite", fields);
    fields.Clear();

    fields.Add("BlipSprite", BaseType::LoadScript("Entity"), 0);
    fields.Add("LocationSprite", BaseType::LoadScript("Entity"), 0);
    fields.Add("atchedEntity", BaseType::LoadScript("Entity"), 0);
    fields.Add("EntityType", BaseType::LoadScript("integer"), 0);
    fields.Add("SubMapIndex", BaseType::LoadScript("integer"), 0);

    new ScriptType("s_RadarBlip", fields);
    fields.Clear();

    fields.Add("OptionName", BaseType::LoadScript("integer"), 0);
    fields.Add("TextField", BaseType::LoadScript("Entity"), 0);
    fields.Add("TextFieldBG", BaseType::LoadScript("Entity"), 0);
    fields.Add("OptionAction", BaseType::LoadScript("integer"), 0);

    new ScriptType("s_MenuOption", fields);
    fields.Clear();

    fields.Add("TaxiZapEntry", BaseType::LoadScript("Entity"), 0);
    fields.Add("EntryIcon", BaseType::LoadScript("Entity"), 0);
    fields.Add("ShowIcon", BaseType::LoadScript("truth"), 0);
    fields.Add("EntryText", BaseType::LoadScript("integer"), 0);
    fields.Add("EntryType", BaseType::LoadScript("integer"), 0);
    fields.Add("EntryDetailTextType", BaseType::LoadScript("integer"), 0);
    fields.Add("EntryDetailTextIndex", BaseType::LoadScript("integer"), 0);
    fields.Add("EntrySpacing", BaseType::LoadScript("truth"), 0);
    fields.Add("IsActiveEntry", BaseType::LoadScript("truth"), 0);

    new ScriptType("s_TaxiZapOptions", fields);
    fields.Clear();

    fields.Add("IsAchieved", BaseType::LoadScript("truth"), 0);
    fields.Add("NameTextField", BaseType::LoadScript("Entity"), 0);
    fields.Add("NameTextFieldBG", BaseType::LoadScript("Entity"), 0);
    fields.Add("ScoreTextField", BaseType::LoadScript("Entity"), 0);
    fields.Add("Stroke3", BaseType::LoadScript("Entity"), 0);
    fields.Add("Stroke4", BaseType::LoadScript("Entity"), 0);
    fields.Add("FlashEffect", BaseType::LoadScript("Entity"), 0);
    fields.Add("AwardType", BaseType::LoadScript("integer"), 0);
    fields.Add("AwardAP", BaseType::LoadScript("integer"), 0);
    fields.Add("AwardModelList", BaseType::LoadScript("list(Entity)"), 0);
    fields.Add("AwardModelListBg", BaseType::LoadScript("list(Entity)"), 0);
    fields.Add("NumOfAwards", BaseType::LoadScript("integer"), 0);

    new ScriptType("s_ScoreBar", fields);
    fields.Clear();

    fields.Add("TextBox", BaseType::LoadScript("Entity"), 0);
    fields.Add("TextBoxBG", BaseType::LoadScript("Entity"), 0);
    fields.Add("BounceSpeed", BaseType::LoadScript("number"), 0);
    fields.Add("OrgScale", BaseType::LoadScript("number"), 0);
    fields.Add("MaxScale", BaseType::LoadScript("number"), 0);
    fields.Add("Expand", BaseType::LoadScript("truth"), 0);

    new ScriptType("s_BounceText", fields);
    fields.Clear();

    fields.Add("Placeholder", BaseType::LoadScript("Entity"), 0);
    fields.Add("StartPoint", BaseType::LoadScript("vector"), 0);
    fields.Add("NewPoint", BaseType::LoadScript("Entity"), 0);
    fields.Add("LastPoint", BaseType::LoadScript("Entity"), 0);
    fields.Add("currenttime", BaseType::LoadScript("number"), 0);
    fields.Add("at_pos", BaseType::LoadScript("truth"), 0);

    new ScriptType("s_ped_resource", fields);
    fields.Clear();

    fields.Add("pBone", BaseType::LoadScript("Entity"), 0);
    fields.Add("vCurrentAngle", BaseType::LoadScript("vector"), 0);
    fields.Add("vTargetAngle", BaseType::LoadScript("vector"), 0);
    fields.Add("Speed", BaseType::LoadScript("number"), 0);
    fields.Add("eRotateSpace", BaseType::LoadScript("integer"), 0);
    fields.Add("bActive", BaseType::LoadScript("truth"), 0);

    new ScriptType("s_CtrlBone", fields);
    fields.Clear();

    fields.Add("Objective", BaseType::LoadScript("Entity"), 0);
    fields.Add("Show", BaseType::LoadScript("truth"), 0);
    fields.Add("Status", BaseType::LoadScript("integer"), 0);

    new ScriptType("s_Mission_Objective", fields);
    fields.Clear();

    fields.Add("Mission_Valid", BaseType::LoadScript("truth"), 0);
    fields.Add("Mission", BaseType::LoadScript("integer"), 0);
    fields.Add("Mission_type", BaseType::LoadScript("integer"), 0);
    fields.Add("Award_achived", BaseType::LoadScript("integer"), 0);
    fields.Add("Mission_Entity", BaseType::LoadScript("Entity"), 0);
    fields.Add("Mission_complete", BaseType::LoadScript("truth"), 0);
    fields.Add("Mission_open", BaseType::LoadScript("truth"), 0);
    fields.Add("Mission_name", BaseType::LoadScript("integer"), 0);
    fields.Add("Mission_start_submap", BaseType::LoadScript("integer"), 0);
    fields.Add("Highest_Score", BaseType::LoadScript("integer"), 0);
    fields.Add("Current_Score", BaseType::LoadScript("integer"), 0);
    fields.Add("L_Combo", BaseType::LoadScript("number"), 0);
    fields.Add("Best_Move_string", BaseType::LoadScript("string"), 0);
    fields.Add("Best_Move", BaseType::LoadScript("integer"), 0);
    fields.Add("FollowUpMission", BaseType::LoadScript("integer"), 0);
    fields.Add("MissionConditions", BaseType::LoadScript("list(Entity)"), 0);
    fields.Add("ObjectiveList", BaseType::LoadScript("list(Entity)"), 0);

    new ScriptType("s_Mission_data", fields);
    fields.Clear();

    fields.Add("TextBox", BaseType::LoadScript("Entity"), 0);
    fields.Add("TextBox_BG", BaseType::LoadScript("Entity"), 0);
    fields.Add("SlideUpTime", BaseType::LoadScript("number"), 0);
    fields.Add("StartSlideTime", BaseType::LoadScript("number"), 0);
    fields.Add("SlideX", BaseType::LoadScript("number"), 0);
    fields.Add("SlideY", BaseType::LoadScript("number"), 0);
    fields.Add("FromPos", BaseType::LoadScript("vector"), 0);
    fields.Add("TargetPos", BaseType::LoadScript("vector"), 0);
    fields.Add("FromOpacity", BaseType::LoadScript("number"), 0);
    fields.Add("TargetOpacity", BaseType::LoadScript("number"), 0);
    fields.Add("StayTime", BaseType::LoadScript("number"), 0);
    fields.Add("InUse", BaseType::LoadScript("truth"), 0);

    new ScriptType("s_ComboAwardString", fields);
    fields.Clear();

    fields.Add("MoveName", BaseType::LoadScript("integer"), 0);
    fields.Add("Bonus", BaseType::LoadScript("integer"), 0);
    fields.Add("GoodieAward", BaseType::LoadScript("integer"), 0);
    fields.Add("MoveBitName", BaseType::LoadScript("integer"), 0);
    fields.Add("SoundIndex", BaseType::LoadScript("integer"), 0);
    fields.Add("SoundFrequency", BaseType::LoadScript("number"), 0);
    fields.Add("CCmove", BaseType::LoadScript("integer"), 0);

    new ScriptType("s_UniqueComboMove", fields);
    fields.Clear();

    fields.Add("KillsNeeded", BaseType::LoadScript("integer"), 0);
    fields.Add("ComboBonus", BaseType::LoadScript("integer"), 0);
    fields.Add("GoodieAwards", BaseType::LoadScript("list(integer)"), 0);
    fields.Add("AwardString", BaseType::LoadScript("integer"), 0);

    new ScriptType("s_ComboAward", fields);
    fields.Clear();

    fields.Add("Position", BaseType::LoadScript("vector"), 0);
    fields.Add("CurrentHealth", BaseType::LoadScript("number"), 0);
    fields.Add("CurrentWeapons", BaseType::LoadScript("list(string)"), 0);
    fields.Add("AmmoActiveWeapon", BaseType::LoadScript("number"), 0);

    new ScriptType("s_StatusPoint", fields);
    fields.Clear();

    fields.Add("ActionsPoints", BaseType::LoadScript("integer"), 0);
    fields.Add("EnemyKills", BaseType::LoadScript("integer"), 0);
    fields.Add("NeutralKills", BaseType::LoadScript("integer"), 0);
    fields.Add("BarrelsKilled", BaseType::LoadScript("integer"), 0);
    fields.Add("AvarageHealth", BaseType::LoadScript("number"), 0);
    fields.Add("UsedKillTypes", BaseType::LoadScript("list(string)"), 0);
    fields.Add("MostOftenKill", BaseType::LoadScript("list(integer)"), 0);
    fields.Add("LargestCombo", BaseType::LoadScript("string"), 0);
    fields.Add("DrivingCar", BaseType::LoadScript("integer"), 0);
    fields.Add("MenuOpened", BaseType::LoadScript("integer"), 0);
    fields.Add("MaxNumOfWeapon", BaseType::LoadScript("integer"), 0);
    fields.Add("MostUsedWeapon", BaseType::LoadScript("integer"), 0);
    fields.Add("Medals", BaseType::LoadScript("integer"), 0);

    new ScriptType("s_Statistics", fields);
    fields.Clear();

    fields.Add("pCallBack", BaseType::LoadScript("Entity"), 0);
    fields.Add("eMessage", BaseType::LoadScript("integer"), 0);
    fields.Add("nTimer", BaseType::LoadScript("number"), 0);
    fields.Add("bRealTime", BaseType::LoadScript("truth"), 0);

    new ScriptType("s_DelayedMessage", fields);
    fields.Clear();

    fields.Add("HasCollided", BaseType::LoadScript("truth"), 0);
    fields.Add("ColEntity", BaseType::LoadScript("Entity"), 0);
    fields.Add("ColVolume", BaseType::LoadScript("Entity"), 0);
    fields.Add("ColMaterial", BaseType::LoadScript("integer"), 0);
    fields.Add("ColFaceProperty", BaseType::LoadScript("integer"), 0);
    fields.Add("ColNormal", BaseType::LoadScript("vector"), 0);
    fields.Add("ColPos", BaseType::LoadScript("vector"), 0);
    fields.Add("SetDecal", BaseType::LoadScript("truth"), 0);

    new ScriptType("s_CollisionInfo", fields);
    fields.Clear();

    fields.Add("CollisionEntity", BaseType::LoadScript("Entity"), 0);
    fields.Add("RealNode", BaseType::LoadScript("Entity"), 0);
    fields.Add("CollisionPos", BaseType::LoadScript("vector"), 0);
    fields.Add("ColliderEntity", BaseType::LoadScript("Entity"), 0);

    new ScriptType("s_VehicleCollision", fields);
    fields.Clear();

    fields.Add("Sound", BaseType::LoadScript("list(Entity)"), 0);
    fields.Add("Volume", BaseType::LoadScript("list(number)"), 0);
    fields.Add("Min_Pause", BaseType::LoadScript("list(number)"), 0);
    fields.Add("Max_Pause", BaseType::LoadScript("list(number)"), 0);
    fields.Add("Position_Type", BaseType::LoadScript("list(integer)"), 0);
    fields.Add("Position_Entity", BaseType::LoadScript("list(Entity)"), 0);

    new ScriptType("Random_sound", fields);
    fields.Clear();

    fields.Add("ent_1", BaseType::LoadScript("Entity"), 0);
    fields.Add("ent_2", BaseType::LoadScript("Entity"), 0);

    new ScriptType("s_Entity_pair", fields);
    fields.Clear();

    fields.Add("Watched", BaseType::LoadScript("Entity"), 0);
    fields.Add("WatchedName", BaseType::LoadScript("integer"), 0);
    fields.Add("HealthPercentage", BaseType::LoadScript("number"), 0);
    fields.Add("InFocus", BaseType::LoadScript("truth"), 0);
    fields.Add("LocalShowTime", BaseType::LoadScript("number"), 0);

    new ScriptType("s_HealthWatchedEntity", fields);
    fields.Clear();

    fields.Add("GamePulse", BaseType::LoadScript("integer"), 0);
    fields.Add("UnderPulse", BaseType::LoadScript("integer"), 0);
    fields.Add("UberPulse", BaseType::LoadScript("integer"), 0);
    fields.Add("StayTime", BaseType::LoadScript("number"), 0);
    fields.Add("toHighPulses", BaseType::LoadScript("integer"), 0);

    new ScriptType("s_pulse", fields);
    fields.Clear();

    fields.Add("at_entity", BaseType::LoadScript("Entity"), 0);
    fields.Add("pos_offset", BaseType::LoadScript("vector"), 0);
    fields.Add("ParticleOwner", BaseType::LoadScript("Entity"), 0);
    fields.Add("ParticleSys", BaseType::LoadScript("Entity"), 0);
    fields.Add("at_pos", BaseType::LoadScript("vector"), 0);
    fields.Add("LifeTime", BaseType::LoadScript("number"), 0);

    new ScriptType("s_particel_dam", fields);
    fields.Clear();

    fields.Add("list_index", BaseType::LoadScript("integer"), 0);
    fields.Add("dist", BaseType::LoadScript("number"), 0);

    new ScriptType("s_list_dist", fields);
    fields.Clear();

    fields.Add("PedestrianGroup", BaseType::LoadScript("integer"), 0);
    fields.Add("Minimum", BaseType::LoadScript("integer"), 0);
    fields.Add("Maximum", BaseType::LoadScript("integer"), 0);

    new ScriptType("s_neighborhood_config", fields);
    fields.Clear();

    fields.Add("PathPoint", BaseType::LoadScript("Entity"), 0);
    fields.Add("Neighborhood", BaseType::LoadScript("Entity"), 0);

    new ScriptType("s_pedestrian_points", fields);
    fields.Clear();

    fields.Add("Object", BaseType::LoadScript("Entity"), 0);
    fields.Add("StartOpacity", BaseType::LoadScript("number"), 0);
    fields.Add("paPerSec", BaseType::LoadScript("number"), 0);
    fields.Add("DelayTimer", BaseType::LoadScript("number"), 0);

    new ScriptType("s_fadeEntity", fields);
    fields.Clear();

    fields.Add("Valid", BaseType::LoadScript("truth"), 0);
    fields.Add("Type", BaseType::LoadScript("integer"), 0);
    fields.Add("Relation", BaseType::LoadScript("integer"), 0);

    new ScriptType("s_target_cache_info", fields);
    fields.Clear();

    fields.Add("TargetScore", BaseType::LoadScript("integer"), 0);
    fields.Add("OverallAwardType", BaseType::LoadScript("integer"), 0);
    fields.Add("AwardString", BaseType::LoadScript("integer"), 0);
    fields.Add("TutAwardString", BaseType::LoadScript("integer"), 0);

    new ScriptType("s_Overall_Award", fields);
    fields.Clear();

    fields.Add("NotifyThis", BaseType::LoadScript("Entity"), 0);
    fields.Add("RequiredScore", BaseType::LoadScript("integer"), 0);

    new ScriptType("s_ScoreNotification", fields);
    fields.Clear();

    fields.Add("position", BaseType::LoadScript("vector"), 0);
    fields.Add("normal", BaseType::LoadScript("vector"), 0);
    fields.Add("type", BaseType::LoadScript("integer"), 0);

    new ScriptType("s_waiting_decal", fields);
    fields.Clear();

    fields.Add("particle", BaseType::LoadScript("Entity"), 0);
    fields.Add("target", BaseType::LoadScript("Entity"), 0);
    fields.Add("impact_pos", BaseType::LoadScript("vector"), 0);
    fields.Add("impact_orient", BaseType::LoadScript("quaternion"), 0);

    new ScriptType("s_waiting_particle", fields);
    fields.Clear();

    fields.Add("position", BaseType::LoadScript("vector"), 0);
    fields.Add("normal", BaseType::LoadScript("vector"), 0);
    fields.Add("impact_dir", BaseType::LoadScript("vector"), 0);
    fields.Add("size", BaseType::LoadScript("number"), 0);
    fields.Add("type", BaseType::LoadScript("integer"), 0);
    fields.Add("probe", BaseType::LoadScript("Entity"), 0);
    fields.Add("ignore", BaseType::LoadScript("Entity"), 0);

    new ScriptType("s_waiting_blood_decal", fields);
    fields.Clear();

    fields.Add("dist", BaseType::LoadScript("number"), 0);
    fields.Add("FromPoint", BaseType::LoadScript("Entity"), 0);
    fields.Add("ToPoint", BaseType::LoadScript("Entity"), 0);
    fields.Add("OneOverLength", BaseType::LoadScript("number"), 0);
    fields.Add("Status", BaseType::LoadScript("integer"), 0);
    fields.Add("EndStatus", BaseType::LoadScript("integer"), 0);
    fields.Add("Type", BaseType::LoadScript("integer"), 0);
    fields.Add("Queue", BaseType::LoadScript("list(Entity)"), 0);

    new ScriptType("s_traffic_section", fields);
    fields.Clear();

    fields.Add("CaptionIndex", BaseType::LoadScript("integer"), 0);
    fields.Add("Tutorials", BaseType::LoadScript("list(Entity)"), 0);

    new ScriptType("s_TutorialMenu", fields);
    fields.Clear();

    fields.Add("obj_text", BaseType::LoadScript("integer"), 0);
    fields.Add("obj_text_alt", BaseType::LoadScript("integer"), 0);
    fields.Add("use_pause_mode", BaseType::LoadScript("truth"), 0);
    fields.Add("delay", BaseType::LoadScript("number"), 0);
    fields.Add("justify", BaseType::LoadScript("truth"), 0);

    new ScriptType("s_TutorialPopUp", fields);
    fields.Clear();

    fields.Add("logic_condition", BaseType::LoadScript("integer"), 0);
    fields.Add("actor", BaseType::LoadScript("Entity"), 0);

    new ScriptType("s_PendingLogicCondition", fields);
    fields.Clear();

    fields.Add("TutorialText", BaseType::LoadScript("integer"), 0);
    fields.Add("index_list", BaseType::LoadScript("list(integer)"), 0);

    new ScriptType("s_TutorialIndex", fields);
    fields.Clear();

    fields.Add("LanguageName", BaseType::LoadScript("string"), 0);
    fields.Add("Language", BaseType::LoadScript("integer"), 0);
    fields.Add("Flag", BaseType::LoadScript("Entity"), 0);
    fields.Add("TextB", BaseType::LoadScript("Entity"), 0);
    fields.Add("LanguageCode", BaseType::LoadScript("string"), 0);

    new ScriptType("s_Language", fields);
    fields.Clear();

    fields.Add("unit", BaseType::LoadScript("Entity"), 0);
    fields.Add("unit_pos", BaseType::LoadScript("vector"), 0);
    fields.Add("unit_orient", BaseType::LoadScript("quaternion"), 0);
    fields.Add("unit_velocity", BaseType::LoadScript("vector"), 0);

    new ScriptType("element", fields);
    fields.Clear();

    fields.Add("unit", BaseType::LoadScript("Entity"), 0);
    fields.Add("hinge", BaseType::LoadScript("vector"), 0);
    fields.Add("unit_orient", BaseType::LoadScript("quaternion"), 0);
    fields.Add("unit_angle_velocity", BaseType::LoadScript("number"), 0);
    fields.Add("unit_angle", BaseType::LoadScript("number"), 0);
    fields.Add("unit_max_angle", BaseType::LoadScript("number"), 0);
    fields.Add("unit_zones", BaseType::LoadScript("integer"), 0);
    fields.Add("unit_in_use", BaseType::LoadScript("integer"), 0);
    fields.Add("unit_rotation_inverse", BaseType::LoadScript("number"), 0);
    fields.Add("unit_bent_step", BaseType::LoadScript("integer"), 0);
    fields.Add("unit_grav_direction", BaseType::LoadScript("number"), 0);

    new ScriptType("hinge_element", fields);
    fields.Clear();

    fields.Add("parameter_name", BaseType::LoadScript("string"), 0);
    fields.Add("parameter_value", BaseType::LoadScript("number"), 0);

    new ScriptType("parameter", fields);
    fields.Clear();

    fields.Add("crossblend_speed", BaseType::LoadScript("number"), 0);
    fields.Add("anim_slot", BaseType::LoadScript("Entity"), 0);

    new ScriptType("crossblend_override", fields);
    fields.Clear();

    if (GlobalPropertiesList.size() == SCRIPT_PROPERTIES_BUILTIN_TOTAL)
#ifdef INCLUDE_FIXES
        LogDump::LogA("%s built-in properties initialized.\n", GlobalCommandsList.size());
#else
        GlobalCommandsList.size();	//	NOTE: why?
#endif

    if (GetGlobalPropertyListChecksum() == SCRIPT_PROPERTIES_BUILTIN_CRC)
        GetGlobalCommandListChecksum();

#ifdef INCLUDE_FIXES
    char path[1024] = {};
    g_AssetManager->GetPlatformSpecificPath(path, "/data/scripts/stable/Database.bin", "bin", Asset::PlatformId::PC);
    ReadDatabaseFile(path);
#else
    String tmpstr;
    g_AssetManager->GetPlatformSpecificPath(tmpstr, "/data/scripts/stable/Database.bin", "bin", Asset::PlatformId::PC);
    ReadDatabaseFile(tmpstr.m_szString);
#endif

    if (GlobalPropertiesList.size() == SCRIPT_PROPERTIES_TOTAL)
#ifdef INCLUDE_FIXES
        LogDump::LogA("%s more properties loaded.\n", GlobalCommandsList.size());
#else
        GlobalCommandsList.size();	//	NOTE: why?
#endif

    if (GetGlobalPropertyListChecksum() == SCRIPT_PROPERTIES_LOADED_CRC)
        GetGlobalCommandListChecksum();

    //	NOTE: global variables?
    GlobalScriptsArray[65] = BaseType::LoadScript("list(entity)");
    GlobalScriptsArray[50] = BaseType::LoadScript("list(string)");
    GlobalScriptsArray[334] = BaseType::LoadScript("list(truth)");
    GlobalScriptsArray[203] = BaseType::LoadScript("list(list(entity))");
    GlobalScriptsArray[18] = BaseType::LoadScript("list(number)");
    GlobalScriptsArray[380] = BaseType::LoadScript("list(vector)");
    GlobalScriptsArray[25] = BaseType::LoadScript("list(quaternion)");
    GlobalScriptsArray[372] = BaseType::LoadScript("list(s_aipassenger)");
    GlobalScriptsArray[251] = BaseType::LoadScript("s_aipassenger");
    GlobalScriptsArray[58] = BaseType::LoadScript("list(list(integer))");
    GlobalScriptsArray[216] = BaseType::LoadScript("list(integer)");
    GlobalScriptsArray[159] = BaseType::LoadScript("list(s_traffic_section)");
    GlobalScriptsArray[152] = BaseType::LoadScript("s_traffic_section");
    GlobalScriptsArray[28] = BaseType::LoadScript("list(element)");
    GlobalScriptsArray[253] = BaseType::LoadScript("list(hinge_element)");
    GlobalScriptsArray[273] = BaseType::LoadScript("element");
    GlobalScriptsArray[279] = BaseType::LoadScript("hinge_element");
    GlobalScriptsArray[49] = BaseType::LoadScript("list(parameter)");
    GlobalScriptsArray[369] = BaseType::LoadScript("list(take_a_hit_info)");
    GlobalScriptsArray[381] = BaseType::LoadScript("parameter");
    GlobalScriptsArray[365] = BaseType::LoadScript("list(s_tutorialmenu)");
    GlobalScriptsArray[290] = BaseType::LoadScript("list(s_mission_data)");
    GlobalScriptsArray[92] = BaseType::LoadScript("s_tutorialmenu");
    GlobalScriptsArray[218] = BaseType::LoadScript("dict(integer)");
    GlobalScriptsArray[277] = BaseType::LoadScript("s_mission_data");
    GlobalScriptsArray[336] = BaseType::LoadScript("list(s_genericres)");
    GlobalScriptsArray[255] = BaseType::LoadScript("s_genericres");
    GlobalScriptsArray[403] = BaseType::LoadScript("list(list(s_genericres))");
    GlobalScriptsArray[148] = BaseType::LoadScript("list(s_vehiclecollision)");
    GlobalScriptsArray[187] = BaseType::LoadScript("s_vehiclecollision");
    GlobalScriptsArray[345] = BaseType::LoadScript("list(s_target_cache_info)");
    GlobalScriptsArray[107] = BaseType::LoadScript("s_target_cache_info");
    GlobalScriptsArray[179] = BaseType::LoadScript("list(s_ctrlbone)");
    GlobalScriptsArray[370] = BaseType::LoadScript("list(s_entity_pair)");
    GlobalScriptsArray[296] = BaseType::LoadScript("s_entity_pair");
    GlobalScriptsArray[248] = BaseType::LoadScript("list(list(truth))");
    GlobalScriptsArray[52] = BaseType::LoadScript("list(s_neighborhood_config)");
    GlobalScriptsArray[134] = BaseType::LoadScript("s_neighborhood_config");
    GlobalScriptsArray[69] = BaseType::LoadScript("list(s_pendinglogiccondition)");
    GlobalScriptsArray[136] = BaseType::LoadScript("s_pendinglogiccondition");
    GlobalScriptsArray[243] = BaseType::LoadScript("list(s_pedestrian_points)");
    GlobalScriptsArray[43] = BaseType::LoadScript("s_pedestrian_points");
    GlobalScriptsArray[306] = BaseType::LoadScript("list(s_particel_dam)");
    GlobalScriptsArray[326] = BaseType::LoadScript("s_particel_dam");
    GlobalScriptsArray[356] = BaseType::LoadScript("list(s_list_dist)");
    GlobalScriptsArray[401] = BaseType::LoadScript("s_list_dist");
    GlobalScriptsArray[182] = BaseType::LoadScript("list(s_fadeentity)");
    GlobalScriptsArray[20] = BaseType::LoadScript("s_fadeentity");
    GlobalScriptsArray[83] = BaseType::LoadScript("list(s_blockload)");
    GlobalScriptsArray[95] = BaseType::LoadScript("s_blockload");
    GlobalScriptsArray[119] = BaseType::LoadScript("list(s_delayedmessage)");
    GlobalScriptsArray[165] = BaseType::LoadScript("s_delayedmessage");
    GlobalScriptsArray[5] = BaseType::LoadScript("list(s_waiting_blood_decal)");
    GlobalScriptsArray[294] = BaseType::LoadScript("s_waiting_blood_decal");
    GlobalScriptsArray[147] = BaseType::LoadScript("take_a_hit_info");
    GlobalScriptsArray[368] = BaseType::LoadScript("list(s_overall_award)");
    GlobalScriptsArray[146] = BaseType::LoadScript("list(s_waiting_decal)");
    GlobalScriptsArray[242] = BaseType::LoadScript("list(s_waiting_particle)");
    GlobalScriptsArray[56] = BaseType::LoadScript("s_waiting_decal");
    GlobalScriptsArray[68] = BaseType::LoadScript("s_waiting_particle");
    GlobalScriptsArray[81] = BaseType::LoadScript("list(s_language)");
    GlobalScriptsArray[247] = BaseType::LoadScript("s_language");
    GlobalScriptsArray[105] = BaseType::LoadScript("list(s_tutorialindex)");
    GlobalScriptsArray[104] = BaseType::LoadScript("s_tutorialpopup");
    GlobalScriptsArray[94] = BaseType::LoadScript("s_tutorialindex");
    GlobalScriptsArray[36] = BaseType::LoadScript("list(s_tutorialpopup)");
    GlobalScriptsArray[158] = BaseType::LoadScript("list(s_bouncetext)");
    GlobalScriptsArray[131] = BaseType::LoadScript("s_bouncetext");
    GlobalScriptsArray[47] = BaseType::LoadScript("list(s_taxizapoptions)");
    GlobalScriptsArray[265] = BaseType::LoadScript("s_taxizapoptions");
    GlobalScriptsArray[231] = BaseType::LoadScript("list(s_radarblip)");
    GlobalScriptsArray[169] = BaseType::LoadScript("s_radarblip");
    GlobalScriptsArray[118] = BaseType::LoadScript("list(s_powerupsprite)");
    GlobalScriptsArray[406] = BaseType::LoadScript("s_powerupsprite");
    GlobalScriptsArray[72] = BaseType::LoadScript("s_overall_award");
    GlobalScriptsArray[180] = BaseType::LoadScript("list(s_scorebar)");
    GlobalScriptsArray[8] = BaseType::LoadScript("list(s_menuoption)");
    GlobalScriptsArray[177] = BaseType::LoadScript("s_scorebar");
    GlobalScriptsArray[343] = BaseType::LoadScript("s_menuoption");
    GlobalScriptsArray[93] = BaseType::LoadScript("list(s_healthwatchedentity)");
    GlobalScriptsArray[188] = BaseType::LoadScript("s_healthwatchedentity");
    GlobalScriptsArray[108] = BaseType::LoadScript("s_comboawardstring");
    GlobalScriptsArray[363] = BaseType::LoadScript("list(s_comboaward)");
    GlobalScriptsArray[207] = BaseType::LoadScript("s_comboaward");
    GlobalScriptsArray[292] = BaseType::LoadScript("s_collisioninfo");
    GlobalScriptsArray[91] = BaseType::LoadScript("list(s_scorenotification)");
    GlobalScriptsArray[314] = BaseType::LoadScript("s_scorenotification");
    GlobalScriptsArray[138] = BaseType::LoadScript("list(crossblend_override)");
    GlobalScriptsArray[1] = BaseType::LoadScript("crossblend_override");
    GlobalScriptsArray[407] = BaseType::LoadScript("list(s_uniquecombomove)");
    GlobalScriptsArray[15] = BaseType::LoadScript("s_uniquecombomove");
    GlobalScriptsArray[221] = BaseType::LoadScript("s_ctrlbone");
    GlobalScriptsArray[400] = BaseType::LoadScript("random_sound");
    
    //	NOTE: register game scripts.
    #include "scripts/common.h"
    #include "scripts/HUD_health_bar.h"
    //	TODO: much much more.

    if (GetGlobalPropertyListChecksum() == SCRIPT_PROPERTIES_LOADED_CRC)
        GetGlobalCommandListChecksum();

    //	TODO: assign loaded scripts to global variables here.
}

void GlobalProperty::GetNameAndType(String& outStr) const
{
    if (m_PropertyType != tNOTHING)
    {
        char buf[256] = {};
        strncpy(buf, m_PropertyName, strlen(m_PropertyName));
        buf[strlen(m_PropertyName)] = ':';
        strcat(buf, m_PropertyType->m_TypeName.m_szString);

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

    m_PropertyType = BaseType::GetTypeByName(propertyname + propnamestrlen + 1);
}

GlobalProperty::~GlobalProperty()
{
    if (m_PropertyName)
        delete[] m_PropertyName;
}

void GlobalCommand::AddArgumentType(BaseType* argtype)
{
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
        strcat(buf, m_Arguments.m_ArgumentsList[0].m_ScriptType->m_TypeName.m_szString);
        
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
        AddArgumentType(BaseType::GetTypeByName(strchr(commandname, ':') + 1));
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
                BaseType* argscripttype = BaseType::LoadScript(tok);
#ifdef INCLUDE_FIXES
                //	NOTE: this should NOT happen!
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

GlobalScript::GlobalScript(const char* const scriptName, const char* const parentName, bool a3, bool a4)
{
    m_ScriptSize = 0;
    field_5C = field_5C & 0xFFFFFFFC | (a3 != 0) | (2 * (a4 != 0));

    ScriptsList.push_back(this);

    m_Name = scriptName;
    m_PropertiesBlocksTotal = 0;
    m_BaseEntity = BaseType::GetScriptEntityByName(parentName);
    field_60 = 0;
}

void GlobalScript::AddStructElement(const int fieldId, const char* const defaultValue, const unsigned int a3)
{
#ifdef INCLUDE_FIXES
    if (fieldId == -1)
    {
        LogDump::LogA("GlobalScript::AddStructElement(%d, \"%s\", %d) FAILED!\n", fieldId, defaultValue, a3);
        return;
    }
#endif
    m_PropertiesValues[fieldId] = m_PropertiesList.size();
    m_PropertiesList.push_back({ &GlobalProperty::GetById(fieldId), m_PropertiesValues.size(), (char*)defaultValue, a3 });
}

void GlobalScript::AddMethod(unsigned short methodid, void (*scriptthreadhandler)(class ScriptThread*), void (*methodptr)(int*))
{
    m_MethodsList.push_back({ methodid, 0, scriptthreadhandler, methodptr });
}

void GlobalScript::CalculateSize()
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
bool GlobalScript::_48A7E0(Node* node, int scriptId, void* args)
{
    return true;
}

void GlobalScript::ClearEntityProperties(Entity* ent)
{
    if (m_PropertiesList.size())
    {
        for (unsigned int i = 0; i < m_PropertiesList.size(); ++i)
            m_PropertiesList[i].m_Info->m_PropertyType->stub4((char*)&ent->m_Parameters[m_PropertiesList[i].m_Offset]);

        delete ent->m_Parameters;
    }
}

class EntityType* GlobalScript::GetScriptEntity() const
{
    char buf[256] = {};
    sprintf(buf, "%s(%s)", m_Name.m_szString, m_BaseEntity->m_TypeName.m_szString);
    
    return BaseType::GetScriptEntityByName(buf);
}

const int GlobalScript::GetPropertiesListSize() const
{
    return m_PropertiesList.size();
}

void GlobalScript::GetEntityPropertyValue(Entity* ent, const unsigned int propertyindex, int* outPropValue)
{
    unsigned int propertyvaluesize = m_PropertiesList[propertyindex].m_Info->m_PropertyType->m_Size;
    int* entpropertyvalue = &ent->m_Parameters[m_PropertiesList[propertyindex].m_Offset];

    if (propertyvaluesize > 0)
        for (; propertyvaluesize; --propertyvaluesize)
            *outPropValue++ = *entpropertyvalue++;
}

EntityType* GlobalScript::AssignScriptToEntity(const EntityType* parent)
{
    if (!m_BaseEntity)
        return nullptr;

    if (!parent)
    {
        LogDump::LogA("'%s' do not descent from '%s', script '%s' cannot be used on a '%s'\n",
#ifdef INCLUDE_FIXES
            "(null)",
#else
            parent->m_TypeName.m_szString,
#endif
            m_BaseEntity->m_TypeName.m_szString,
            m_Name.m_szString,
#ifdef INCLUDE_FIXES
            "(null)");
#else
            parent->m_TypeName.m_szString);
#endif

        return nullptr;
    }

    EntityType* parentent = (EntityType*)parent;
    while (m_BaseEntity != parentent)
    {
        parentent = parent->m_Parent;
        if (!parentent)
        {
            LogDump::LogA("'%s' do not descent from '%s', script '%s' cannot be used on a '%s'\n",
                parent->m_TypeName.m_szString,
                m_BaseEntity->m_TypeName.m_szString,
                m_Name.m_szString,
                parent->m_TypeName.m_szString);

            return nullptr;
        }
    }

    char entname[128] = {};
    sprintf(entname, "%s(%s)", m_Name.m_szString, parent->m_TypeName.m_szString);

    EntityType* ent = new EntityType(entname);
    ent->InheritFrom(parent);
    ent->m_Script = this;
    ent->_86E9B0();

    return ent;
}

GlobalScript* GlobalScript::GetGlobalScriptByName(const char* name)
{
    if (!name || !*name)
        return nullptr;

    if (!ScriptsList.size())
        return nullptr;

    const size_t namelen = strlen(name);
    for (unsigned int i = 0; i < ScriptsList.size(); ++i)
        if (strncmp(ScriptsList[i]->m_Name.m_szString, name, namelen) == NULL)
            return ScriptsList[i];

    return nullptr;
}

GlobalScript* GlobalScript::GetGlobalScriptById(const unsigned int id)
{
    return ScriptsList[id];
}

int GlobalScript::GetScriptIdByName(const char* const name)
{
    if (ScriptsList.size() <= 0)
        return -1;

    for (unsigned int i = 0; i < ScriptsList.size(); ++i)
        if ((ScriptsList[i]->field_5C & 1) != 0 && strcmp(ScriptsList[i]->m_Name.m_szString, name) == 0)
            return i;

    return -1;
}

#pragma message(TODO_IMPLEMENTATION)
void GlobalScript::InstantiateGlobalScripts()
{
    if (!Scene::SceneInstance)
        return;

    for (unsigned int i = 0; i < ScriptsList.size(); ++i)
    {
        if (!ScriptsList[i]->field_5C & 1)
            continue;

        if (Scene::SceneInstance->m_FirstChild)
        {

        }
        else
        {

        }
    }

    AssignCommonNodes();
}

void GlobalScript::AssignCommonNodes()
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

        Node* scriptnode = (Node*)Scene::SceneInstance->FindNode(sc->GetScriptEntity()->m_TypeName.m_szString);
        SceneScriptEntitiesList[i] = scriptnode;

        if (_stricmp(sc->m_Name.m_szString, "cache") == NULL)
            CacheScriptNode = scriptnode;
        else if (_stricmp(sc->m_Name.m_szString, "common") == NULL)
            CommonScriptNode = scriptnode;
        else if (_stricmp(sc->m_Name.m_szString, "common_animslot") == NULL)
            CommonAnimSlotScriptNode = scriptnode;
        else if (_stricmp(sc->m_Name.m_szString, "common_trigger") == NULL)
            CommonTriggerScriptNode = scriptnode;
        else if (_stricmp(sc->m_Name.m_szString, "IntroCommon") == NULL)
            IntroCommonScriptNode = scriptnode;
    }
}

unsigned int GlobalScript::GetScriptIdByFullName(const char* const name)
{
    //	TODO: there's a trouble when looking for an Entity-specific properties. Take that into account?
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

GlobalCommand::CommandArgument::CommandArgument(String& argname, const BaseType* argtype, unsigned int argsize)
{
    m_PropertyName = argname;
    m_ScriptType = (BaseType*)argtype;
    m_TotalSizeBytes = argsize;
}