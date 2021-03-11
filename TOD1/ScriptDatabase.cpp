#include "ScriptDatabase.h"
#include "LogDump.h"
#include "Performance.h"
#include "Globals.h"
#include "ScriptTypes.h"

std::vector<GlobalProperty>		GlobalPropertiesList;	//	@A3CF20
std::map<String, unsigned int>	GlobalPropertiesMap;	//	@A3CF30
std::vector<GlobalCommand>		GlobalCommandsList;	//	@A11470
std::map<String, unsigned int>	GlobalCommandsMap;	//	@A3CF08
std::vector<GlobalScript> GlobalScript::ScriptsList;
unsigned int	GlobalPropertyListChecksum;	//	@A3CF40
bool			GlobalPropertyListChecksumObtained;	//	@A3CF1C
unsigned int	GlobalCommandListChecksum;	//	@A3CF18
bool			GlobalCommandListChecksumObtained;	//	@A3CEF4

unsigned int GetGlobalPropertyListChecksum()
{
	if (GlobalPropertyListChecksumObtained)
		return GlobalPropertyListChecksum;

#ifdef INCLUDE_FIXES
	char checksum_str[16348] = {};
#else
	char checksum_str[102400] = {};
#endif
	unsigned int checksum_str_len = NULL;

	if (GlobalPropertiesList.size() > 0)
	{
		for (std::vector<GlobalProperty>::const_iterator it = GlobalPropertiesList.cbegin(); it != GlobalPropertiesList.cend(); ++it)
		{
			String tempstr;
			it->GetNameAndType(tempstr);
			size_t rettypestrlen = strlen(tempstr.m_szString);

			if (checksum_str_len + rettypestrlen > sizeof(checksum_str))
				break;
			else
				checksum_str_len += rettypestrlen;

			if (*checksum_str == NULL)
				strcpy(checksum_str, tempstr.m_szString);
			else
				strcat(checksum_str, tempstr.m_szString);
		}
	}

	GlobalPropertyListChecksum = Utils::CalcCRC32(checksum_str, checksum_str_len);
	GlobalPropertyListChecksumObtained = true;
	return GlobalPropertyListChecksum;
}

unsigned int GetGlobalCommandListChecksum()
{
	if (GlobalCommandListChecksumObtained)
		return GlobalCommandListChecksum;

#ifdef INCLUDE_FIXES
	char checksum_str[16348] = {};
#else
	char checksum_str[102400] = {};
#endif
	unsigned int checksum_str_len = NULL;

	if (GlobalCommandsList.size() > 0)
	{
		for (std::vector<GlobalCommand>::iterator it = GlobalCommandsList.begin(); it != GlobalCommandsList.end(); ++it)
		{
			String tempstr;
			(*it).GetReturnTypeString(tempstr);
			size_t rettypestrlen = strlen(tempstr.m_szString);

			if (checksum_str_len + rettypestrlen > sizeof(checksum_str))
				break;
			else
				checksum_str_len += rettypestrlen;

			if (*checksum_str == NULL)
				strcpy(checksum_str, tempstr.m_szString);
			else
				strcat(checksum_str, tempstr.m_szString);
		}
	}

	GlobalCommandListChecksum = Utils::CalcCRC32(checksum_str, checksum_str_len);
	GlobalCommandListChecksumObtained = true;
	return GlobalCommandListChecksum;
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

extern int GetCommandByName_Impl(const char* const commandname)
{
	String cmdname = commandname;
	cmdname.ToLowerCase();

	const auto& valiter = GlobalCommandsMap.find(cmdname);
	return valiter->second ? valiter->second : -1;
}

int GetCommandByName(const char* const commandname)
{
	const char* const ddotpos = strchr(commandname, ':');
	if (ddotpos)
	{
		char cmdname[64] = {};
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
			return propid;
		else
			return -1;
	}
	else
	{
		ScriptType::LoadScript(proptype);

		unsigned int propind = GlobalPropertiesList.size();
		GlobalPropertiesList.emplace_back(propertyname, propind);

		char propname[64] = {};
		strncpy(propname, propertyname, ddotpos - propertyname);
		String::ToLowerCase(propname);

		GlobalPropertiesMap[propname] = propind;
		GlobalPropertyListChecksumObtained = false;

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
			ScriptType::GetTypeByName(ddotpos + 1);

		return cmdid;
	}
	else
	{
		int commandind = GlobalCommandsList.size();
		GlobalCommandsList.emplace_back(commandname, commandind);
		char cmdname[64] = {};
		strncpy(cmdname, commandname, ddotpos - commandname);
		GlobalCommandsMap[cmdname] = commandind;

		return commandind;
	}
}

#pragma message(TODO_IMPLEMENTATION)
//	NOTE: this is unoptimized version.
void ReadDatabaseFile(const char* path)
{
	LogDump::LogA("Loading script database\n");
	File dbfile(path, 0x21, true);

	if (!dbfile.IsFileOpen())
		return;

	DWORD timeStart = Performance::GetMilliseconds();
	unsigned int totalScripts = NULL;

	dbfile.Read(&totalScripts, sizeof(totalScripts));
	GlobalPropertiesList.resize(totalScripts);

	if (totalScripts <= NULL)
	{
		unsigned int totalCommands = NULL;
		dbfile.Read(&totalCommands, sizeof(totalCommands));

		if (totalCommands <= NULL)
		{
			LogDump::LogA("Done loading script database (%dms)\n", Performance::GetMilliseconds() - timeStart);
			return;
		}

		char buffer[1024] = {};
		unsigned int commandsRead = NULL;
		while (true)
		{
			unsigned int commandlen = NULL;
			dbfile.Read(&commandlen, sizeof(commandlen));

			if (commandlen + 1 >= sizeof(buffer))
				break;

			dbfile.Read(buffer, sizeof(buffer));
			RegisterGlobalCommand(buffer, false);

			memset(buffer, NULL, sizeof(buffer));

			if (commandsRead++ >= totalCommands)
			{
				LogDump::LogA("Done loading script database (%dms)\n", Performance::GetMilliseconds() - timeStart);
				return;
			}
		}
	}
	else
	{
		char buffer[1024] = {};
		unsigned int propertiesRead = NULL;
		while (true)
		{
			unsigned int propertylen = NULL;
			dbfile.Read(&propertylen, sizeof(propertylen));

			if (propertylen + 1 >= sizeof(propertylen))
				break;

			dbfile.Read(buffer, sizeof(buffer));
			RegisterGlobalProperty(buffer, false);

			if (propertiesRead++ >= totalScripts)
			{
				unsigned int totalCommands = NULL;
				dbfile.Read(&totalCommands, sizeof(totalCommands));

				if (totalCommands <= NULL)
				{
					LogDump::LogA("Done loading script database (%dms)\n", Performance::GetMilliseconds() - timeStart);
					return;
				}

				char buffer[1024] = {};
				unsigned int commandsRead = NULL;
				while (true)
				{
					unsigned int commandlen = NULL;
					dbfile.Read(&commandlen, sizeof(commandlen));

					if (commandlen + 1 >= sizeof(buffer))
						break;

					dbfile.Read(buffer, sizeof(buffer));
					RegisterGlobalCommand(buffer, false);

					memset(buffer, NULL, sizeof(buffer));

					if (commandsRead++ >= totalCommands)
					{
						LogDump::LogA("Done loading script database (%dms)\n", Performance::GetMilliseconds() - timeStart);
						return;
					}
				}
			}
		}
	}
}

#pragma message(TODO_IMPLEMENTATION)
//	FIXME: right now, there are too much allocations going on here (for each field on array assignment, for each string allocation and copy).
void LoadScripts()
{
	/*
	ScriptFieldsList params;

	//	NOTE: initialize all builtin scripts (structures) here.
	//	TODO: once finished, implement it using macro like this: DECLARE_SCRIPT("s_GenericRes", { {"pResource", GetScriptType("Entity")}, {"pPlaceholder", GetScripTtype("Entity")} }).
	params.Add({ "RecievingEntity", LoadScript("Entity"), NULL });
	params.Add({ "Damage", LoadScript("number"), NULL });
	params.Add({ "Owner", LoadScript("Entity"), NULL });
	params.Add({ "inflictor_node", LoadScript("Entity"), NULL });
	params.Add({ "RecievingCollVolume", LoadScript("Entity"), NULL });
	params.Add({ "CollisionImpulse", LoadScript("vector"), NULL });
	params.Add({ "Peercontactpoint", LoadScript("vector"), NULL });
	params.Add({ "Damage_type", LoadScript("integer"), NULL });

	new ScriptType_Script("Take_a_hit_info", params);
	params.Clear();

	params.Add({ "pBlock", LoadScript("Entity"), NULL });
	params.Add({ "sTeleportName", LoadScript("string"), NULL });

	new ScriptType_Script("s_BlockLoad", params);
	params.Clear();

	params.Add({ "pResource", LoadScript("Entity"), NULL });
	params.Add({ "pPlaceholder", LoadScript("Entity"), NULL });

	new ScriptType_Script("s_GenericRes", params);
	params.Clear();

	params.Add({ "pPlaceholder", LoadScript("Entity"), NULL });
	params.Add({ "RangeSqr", LoadScript("number"), NULL });
	params.Add({ "bInView", LoadScript("truth"), NULL });
	params.Add({ "bIsVisible", LoadScript("truth"), NULL });

	new ScriptType_Script("s_PlaceholderPriority", params);
	params.Clear();

	params.Add({ "RightHandGunList", LoadScript("list(truth)"), NULL });
	params.Add({ "LeftHandGunList", LoadScript("list(truth)"), NULL });
	params.Add({ "AmmoList", LoadScript("list(integer)"), NULL });
	params.Add({ "MaxAmmoList", LoadScript("list(integer)"), NULL });
	params.Add({ "AmmoInPrimWeapon", LoadScript("list(integer)"), NULL });
	params.Add({ "AmmoInSecWeapon", LoadScript("list(integer)"), NULL });

	new ScriptType_Script("s_Inventory", params);
	params.Clear();

	params.Add({ "pPlaceHolder", LoadScript("Entity"), NULL });
	params.Add({ "vLocalPosition", LoadScript("vector"), NULL });

	new ScriptType_Script("s_AIPassenger", params);
	params.Clear();

	params.Add({ "PowerUpList", LoadScript("list(integer)"), NULL });
	params.Add({ "MaxPowerUpList", LoadScript("list(integer)"), NULL });

	new ScriptType_Script("s_PowerUpInventory", params);
	params.Clear();

	params.Add({ "Type", LoadScript("integer"), NULL });
	params.Add({ "Sprite", LoadScript("Entity"), NULL });
	params.Add({ "BGSprite", LoadScript("Entity"), NULL });
	params.Add({ "OrgSize", LoadScript("vector"), NULL });
	params.Add({ "NormalSize", LoadScript("vector"), NULL });
	params.Add({ "NormalPos", LoadScript("vector"), NULL });
	params.Add({ "SelectedSize", LoadScript("vector"), NULL });
	params.Add({ "SelectedPos", LoadScript("vector"), NULL });
	params.Add({ "AmountTB", LoadScript("Entity"), NULL });
	params.Add({ "AmountTB_BG", LoadScript("Entity"), NULL });

	new ScriptType_Script("s_PowerUpSprite", params);
	params.Clear();

	params.Add({ "BlipSprite", LoadScript("Entity"), NULL });
	params.Add({ "LocationSprite", LoadScript("Entity"), NULL });
	params.Add({ "WatchedEntity", LoadScript("Entity"), NULL });
	params.Add({ "EntityType", LoadScript("integer"), NULL });
	params.Add({ "SubMapIndex", LoadScript("integer"), NULL });

	new ScriptType_Script("s_RadarBlip", params);
	params.Clear();

	params.Add({ "OptionName", LoadScript("integer"), NULL });
	params.Add({ "TextField", LoadScript("Entity"), NULL });
	params.Add({ "TextFieldBG", LoadScript("Entity"), NULL });
	params.Add({ "OptionAction", LoadScript("integer"), NULL });

	new ScriptType_Script("s_MenuOption", params);
	params.Clear();

	params.Add({ "TaxiZapEntry", LoadScript("Entity"), NULL });
	params.Add({ "EntryIcon", LoadScript("Entity"), NULL });
	params.Add({ "ShowIcon", LoadScript("truth"), NULL });
	params.Add({ "EntryText", LoadScript("integer"), NULL });
	params.Add({ "EntryType", LoadScript("integer"), NULL });
	params.Add({ "EntryDetailTextType", LoadScript("integer"), NULL });
	params.Add({ "EntryDetailTextIndex", LoadScript("integer"), NULL });
	params.Add({ "EntrySpacing", LoadScript("truth"), NULL });
	params.Add({ "IsActiveEntry", LoadScript("truth"), NULL });

	new ScriptType_Script("s_TaxiZapOptions", params);
	params.Clear();

	params.Add({ "IsAchieved", LoadScript("truth"), NULL });
	params.Add({ "NameTextField", LoadScript("Entity"), NULL });
	params.Add({ "NameTextFieldBG", LoadScript("Entity"), NULL });
	params.Add({ "ScoreTextField", LoadScript("Entity"), NULL });
	params.Add({ "Stroke3", LoadScript("Entity"), NULL });
	params.Add({ "Stroke4", LoadScript("Entity"), NULL });
	params.Add({ "FlashEffect", LoadScript("Entity"), NULL });
	params.Add({ "AwardType", LoadScript("integer"), NULL });
	params.Add({ "AwardAP", LoadScript("integer"), NULL });
	params.Add({ "AwardModelList", LoadScript("list(Entity)"), NULL });
	params.Add({ "AwardModelListBg", LoadScript("list(Entity)"), NULL });
	params.Add({ "NumOfAwards", LoadScript("integer"), NULL });

	new ScriptType_Script("s_ScoreBar", params);
	params.Clear();

	params.Add({ "TextBox", LoadScript("Entity"), NULL });
	params.Add({ "TextBoxBG", LoadScript("Entity"), NULL });
	params.Add({ "BounceSpeed", LoadScript("number"), NULL });
	params.Add({ "OrgScale", LoadScript("number"), NULL });
	params.Add({ "MaxScale", LoadScript("number"), NULL });
	params.Add({ "Expand", LoadScript("truth"), NULL });

	new ScriptType_Script("s_BounceText", params);
	params.Clear();

	params.Add({ "Placeholder", LoadScript("Entity"), NULL });
	params.Add({ "StartPoint", LoadScript("vector"), NULL });
	params.Add({ "NewPoint", LoadScript("Entity"), NULL });
	params.Add({ "LastPoint", LoadScript("Entity"), NULL });
	params.Add({ "currenttime", LoadScript("number"), NULL });
	params.Add({ "at_pos", LoadScript("truth"), NULL });

	new ScriptType_Script("s_ped_resource", params);
	params.Clear();

	params.Add({ "pBone", LoadScript("Entity"), NULL });
	params.Add({ "vCurrentAngle", LoadScript("vector"), NULL });
	params.Add({ "vTargetAngle", LoadScript("vector"), NULL });
	params.Add({ "Speed", LoadScript("number"), NULL });
	params.Add({ "eRotateSpace", LoadScript("integer"), NULL });
	params.Add({ "bActive", LoadScript("truth"), NULL });

	new ScriptType_Script("s_CtrlBone", params);
	params.Clear();

	params.Add({ "Objective", LoadScript("Entity"), NULL });
	params.Add({ "Show", LoadScript("truth"), NULL });
	params.Add({ "Status", LoadScript("integer"), NULL });

	new ScriptType_Script("s_Mission_Objective", params);
	params.Clear();

	params.Add({ "Mission_Valid", LoadScript("truth"), NULL });
	params.Add({ "Mission", LoadScript("integer"), NULL });
	params.Add({ "Mission_type", LoadScript("integer"), NULL });
	params.Add({ "Award_achived", LoadScript("integer"), NULL });
	params.Add({ "Mission_Entity", LoadScript("Entity"), NULL });
	params.Add({ "Mission_complete", LoadScript("truth"), NULL });
	params.Add({ "Mission_open", LoadScript("truth"), NULL });
	params.Add({ "Mission_name", LoadScript("integer"), NULL });
	params.Add({ "Mission_start_submap", LoadScript("integer"), NULL });
	params.Add({ "Highest_Score", LoadScript("integer"), NULL });
	params.Add({ "Current_Score", LoadScript("integer"), NULL });
	params.Add({ "L_Combo", LoadScript("number"), NULL });
	params.Add({ "Best_Move_String", LoadScript("string"), NULL });
	params.Add({ "Best_Move", LoadScript("integer"), NULL });
	params.Add({ "FollowUpMission", LoadScript("integer"), NULL });
	params.Add({ "MissionConditions", LoadScript("list(Entity)"), NULL });
	params.Add({ "ObjectiveList", LoadScript("list(Entity)"), NULL });

	new ScriptType_Script("s_Mission_data", params);
	params.Clear();

	params.Add({ "TextBox", LoadScript("Entity"), NULL });
	params.Add({ "TextBox_BG", LoadScript("Entity"), NULL });
	params.Add({ "SlideUpTime", LoadScript("number"), NULL });
	params.Add({ "StartSlideTime", LoadScript("number"), NULL });
	params.Add({ "SlideX", LoadScript("number"), NULL });
	params.Add({ "SlideY", LoadScript("number"), NULL });
	params.Add({ "FromPos", LoadScript("vector"), NULL });
	params.Add({ "TargetPos", LoadScript("vector"), NULL });
	params.Add({ "FromOpacity", LoadScript("number"), NULL });
	params.Add({ "TargetOpacity", LoadScript("number"), NULL });
	params.Add({ "StayTime", LoadScript("number"), NULL });
	params.Add({ "InUse", LoadScript("truth"), NULL });

	new ScriptType_Script("s_ComboAwardString", params);
	params.Clear();

	params.Add({ "MoveName", LoadScript("integer"), NULL });
	params.Add({ "Bonus", LoadScript("integer"), NULL });
	params.Add({ "GoodieAward", LoadScript("integer"), NULL });
	params.Add({ "MoveBitName", LoadScript("integer"), NULL });
	params.Add({ "SoundIndex", LoadScript("integer"), NULL });
	params.Add({ "SoundFrequency", LoadScript("number"), NULL });
	params.Add({ "CCmove", LoadScript("integer"), NULL });

	new ScriptType_Script("s_UniqueComboMove", params);
	params.Clear();

	params.Add({ "KillsNeeded", LoadScript("integer"), NULL });
	params.Add({ "ComboBonus", LoadScript("integer"), NULL });
	params.Add({ "GoodieAwards", LoadScript("list(integer)"), NULL });
	params.Add({ "AwardString", LoadScript("integer"), NULL });

	new ScriptType_Script("s_ComboAward", params);
	params.Clear();

	params.Add({ "Position", LoadScript("vector"), NULL });
	params.Add({ "CurrentHealth", LoadScript("number"), NULL });
	params.Add({ "CurrentWeapons", LoadScript("list(string)"), NULL });
	params.Add({ "AmmoActiveWeapon", LoadScript("number"), NULL });

	new ScriptType_Script("s_StatusPoint", params);
	params.Clear();

	params.Add({ "ActionsPoints", LoadScript("integer"), NULL });
	params.Add({ "EnemyKills", LoadScript("integer"), NULL });
	params.Add({ "NeutralKills", LoadScript("integer"), NULL });
	params.Add({ "BarrelsKilled", LoadScript("integer"), NULL });
	params.Add({ "AvarageHealth", LoadScript("number"), NULL });
	params.Add({ "UsedKillTypes", LoadScript("list(string)"), NULL });
	params.Add({ "MostOftenKill", LoadScript("list(integer)"), NULL });
	params.Add({ "LargestCombo", LoadScript("string"), NULL });
	params.Add({ "DrivingCar", LoadScript("integer"), NULL });
	params.Add({ "MenuOpened", LoadScript("integer"), NULL });
	params.Add({ "MaxNumOfWeapon", LoadScript("integer"), NULL });
	params.Add({ "MostUsedWeapon", LoadScript("integer"), NULL });
	params.Add({ "Medals", LoadScript("integer"), NULL });

	new ScriptType_Script("s_Statistics", params);
	params.Clear();

	params.Add({ "pCallBack", LoadScript("Entity"), NULL });
	params.Add({ "eMessage", LoadScript("integer"), NULL });
	params.Add({ "nTimer", LoadScript("number"), NULL });
	params.Add({ "bRealTime", LoadScript("truth"), NULL });

	new ScriptType_Script("s_DelayedMessage", params);
	params.Clear();

	params.Add({ "HasCollided", LoadScript("truth"), NULL });
	params.Add({ "ColEntity", LoadScript("Entity"), NULL });
	params.Add({ "ColVolume", LoadScript("Entity"), NULL });
	params.Add({ "ColMaterial", LoadScript("integer"), NULL });
	params.Add({ "ColFaceProperty", LoadScript("integer"), NULL });
	params.Add({ "ColNormal", LoadScript("vector"), NULL });
	params.Add({ "ColPos", LoadScript("vector"), NULL });
	params.Add({ "SetDecal", LoadScript("truth"), NULL });

	new ScriptType_Script("s_CollisionInfo", params);
	params.Clear();

	params.Add({ "CollisionEntity", LoadScript("Entity"), NULL });
	params.Add({ "RealNode", LoadScript("Entity"), NULL });
	params.Add({ "CollisionPos", LoadScript("vector"), NULL });
	params.Add({ "ColliderEntity", LoadScript("Entity"), NULL });

	new ScriptType_Script("s_VehicleCollision", params);
	params.Clear();

	params.Add({ "Sound", LoadScript("list(Entity)"), NULL });
	params.Add({ "Volume", LoadScript("list(number)"), NULL });
	params.Add({ "Min_Pause", LoadScript("list(number)"), NULL });
	params.Add({ "Max_Pause", LoadScript("list(number)"), NULL });
	params.Add({ "Position_Type", LoadScript("list(integer)"), NULL });
	params.Add({ "Position_Enity", LoadScript("list(Entity)"), NULL });

	new ScriptType_Script("s_Random_sound", params);
	params.Clear();

	params.Add({ "ent_1", LoadScript("Entity"), NULL });
	params.Add({ "ent_2", LoadScript("Entity"), NULL });

	new ScriptType_Script("s_Entity_pair", params);
	params.Clear();

	params.Add({ "Watched", LoadScript("Entity"), NULL });
	params.Add({ "WatchedName", LoadScript("integer"), NULL });
	params.Add({ "HealthPercentage", LoadScript("number"), NULL });
	params.Add({ "InFocus", LoadScript("truth"), NULL });
	params.Add({ "LocalShowTime", LoadScript("number"), NULL });

	new ScriptType_Script("s_HealthWatchedEntity", params);
	params.Clear();

	params.Add({ "GamePulse", LoadScript("integer"), NULL });
	params.Add({ "UnderPulse", LoadScript("integer"), NULL });
	params.Add({ "UberPulse", LoadScript("integer"), NULL });
	params.Add({ "StayTime", LoadScript("number"), NULL });
	params.Add({ "toHighPulses", LoadScript("integer"), NULL });

	new ScriptType_Script("s_pulse", params);
	params.Clear();

	params.Add({ "at_entity", LoadScript("Entity"), NULL });
	params.Add({ "pos_offset", LoadScript("vector"), NULL });
	params.Add({ "ParticleOwner", LoadScript("Entity"), NULL });
	params.Add({ "ParticleSys", LoadScript("Entity"), NULL });
	params.Add({ "at_pos", LoadScript("vector"), NULL });
	params.Add({ "LifeTime", LoadScript("number"), NULL });

	new ScriptType_Script("s_particel_dam", params);
	params.Clear();

	params.Add({ "list_index", LoadScript("integer"), NULL });
	params.Add({ "dist", LoadScript("number"), NULL });

	new ScriptType_Script("s_list_dist", params);
	params.Clear();

	params.Add({ "PedestrianGroup", LoadScript("integer"), NULL });
	params.Add({ "Minimum", LoadScript("integer"), NULL });
	params.Add({ "Maximum", LoadScript("integer"), NULL });

	new ScriptType_Script("s_neighborhood_config", params);
	params.Clear();

	params.Add({ "PathPoint", LoadScript("Entity"), NULL });
	params.Add({ "Neighborhood", LoadScript("Entity"), NULL });

	new ScriptType_Script("s_pedestrian_points", params);
	params.Clear();

	params.Add({ "Object", LoadScript("Entity"), NULL });
	params.Add({ "StartOpacity", LoadScript("number"), NULL });
	params.Add({ "OpaPerSec", LoadScript("number"), NULL });
	params.Add({ "DelayTimer", LoadScript("number"), NULL });

	new ScriptType_Script("s_fadeEntity", params);
	params.Clear();

	params.Add({ "Valid", LoadScript("truth"), NULL });
	params.Add({ "Type", LoadScript("integer"), NULL });
	params.Add({ "Relation", LoadScript("integer"), NULL });

	new ScriptType_Script("s_target_cache_info", params);
	params.Clear();

	params.Add({ "TargetScore", LoadScript("integer"), NULL });
	params.Add({ "OverallAwardType", LoadScript("integer"), NULL });
	params.Add({ "AwardString", LoadScript("integer"), NULL });
	params.Add({ "TutAwardString", LoadScript("integer"), NULL });

	new ScriptType_Script("s_Overall_Award", params);
	params.Clear();

	params.Add({ "NotifyThis", LoadScript("Entity"), NULL });
	params.Add({ "RequiredScore", LoadScript("integer"), NULL });

	new ScriptType_Script("s_ScoreNotification", params);
	params.Clear();

	params.Add({ "position", LoadScript("vector"), NULL });
	params.Add({ "normal", LoadScript("vector"), NULL });
	params.Add({ "type", LoadScript("integer"), NULL });

	new ScriptType_Script("s_waiting_decal", params);
	params.Clear();

	params.Add({ "particle", LoadScript("Entity"), NULL });
	params.Add({ "target", LoadScript("Entity"), NULL });
	params.Add({ "impact_pos", LoadScript("vector"), NULL });
	params.Add({ "impact_orient", LoadScript("quaternion"), NULL });

	new ScriptType_Script("s_waiting_particle", params);
	params.Clear();

	params.Add({ "position", LoadScript("vector"), NULL });
	params.Add({ "normal", LoadScript("vector"), NULL });
	params.Add({ "impact_dir", LoadScript("vector"), NULL });
	params.Add({ "size", LoadScript("number"), NULL });
	params.Add({ "type", LoadScript("integer"), NULL });
	params.Add({ "probe", LoadScript("Entity"), NULL });
	params.Add({ "ignore", LoadScript("Entity"), NULL });

	new ScriptType_Script("s_waiting_blood_decal", params);
	params.Clear();

	params.Add({ "dist", LoadScript("number"), NULL });
	params.Add({ "FromPoint", LoadScript("Entity"), NULL });
	params.Add({ "ToPoint", LoadScript("Entity"), NULL });
	params.Add({ "OneOverLength", LoadScript("number"), NULL });
	params.Add({ "Status", LoadScript("integer"), NULL });
	params.Add({ "EndStatus", LoadScript("integer"), NULL });
	params.Add({ "Type", LoadScript("integer"), NULL });
	params.Add({ "Queue", LoadScript("list(Entity)"), NULL });

	new ScriptType_Script("s_traffic_section", params);
	params.Clear();

	params.Add({ "CaptionIndex", LoadScript("integer"), NULL });
	params.Add({ "Tutorials", LoadScript("list(Entity)"), NULL });

	new ScriptType_Script("s_TutorialMenu", params);
	params.Clear();

	params.Add({ "obj_text", LoadScript("integer"), NULL });
	params.Add({ "obj_text_alt", LoadScript("integer"), NULL });
	params.Add({ "use_pause_mode", LoadScript("truth"), NULL });
	params.Add({ "delay", LoadScript("number"), NULL });
	params.Add({ "justify", LoadScript("truth"), NULL });

	new ScriptType_Script("s_TutorialPopUp", params);
	params.Clear();

	params.Add({ "logic_condition", LoadScript("integer"), NULL });
	params.Add({ "actor", LoadScript("Entity"), NULL });

	new ScriptType_Script("s_PendingLogicalCondition", params);
	params.Clear();

	params.Add({ "TutorialText", LoadScript("integer"), NULL });
	params.Add({ "index_list", LoadScript("list(integer)"), NULL });

	new ScriptType_Script("s_TutorialIndex", params);
	params.Clear();

	params.Add({ "LanguageName", LoadScript("string"), NULL });
	params.Add({ "Language", LoadScript("integer"), NULL });
	params.Add({ "Flag", LoadScript("Entity"), NULL });
	params.Add({ "TextB", LoadScript("Entity"), NULL });
	params.Add({ "LanguageCode", LoadScript("string"), NULL });

	new ScriptType_Script("s_Language", params);
	params.Clear();

	params.Add({ "unit", LoadScript("Entity"), NULL });
	params.Add({ "unit_pos", LoadScript("vector"), NULL });
	params.Add({ "unit_orient", LoadScript("quaternion"), NULL });
	params.Add({ "unit_velocity", LoadScript("vector"), NULL });

	new ScriptType_Script("element", params);
	params.Clear();

	params.Add({ "unit", LoadScript("Entity"), NULL });
	params.Add({ "hinge", LoadScript("vector"), NULL });
	params.Add({ "unit_orient", LoadScript("quaternion"), NULL });
	params.Add({ "unit_angle_velocity", LoadScript("number"), NULL });
	params.Add({ "unit_angle", LoadScript("number"), NULL });
	params.Add({ "unit_max_angle", LoadScript("number"), NULL });
	params.Add({ "unit_zones", LoadScript("integer"), NULL });
	params.Add({ "unit_in_use", LoadScript("integer"), NULL });
	params.Add({ "unit_rotation_inverse", LoadScript("number"), NULL });
	params.Add({ "unit_bent_step", LoadScript("integer"), NULL });
	params.Add({ "unit_grav_direction", LoadScript("number"), NULL });

	new ScriptType_Script("hinge_element", params);
	params.Clear();

	params.Add({ "parameter_name", LoadScript("string"), NULL });
	params.Add({ "parameter_value", LoadScript("number"), NULL });

	new ScriptType_Script("parameter", params);
	params.Clear();

	params.Add({ "crossblend_speed", LoadScript("number"), NULL });
	params.Add({ "anim_slot", LoadScript("Entity"), NULL });

	new ScriptType_Script("crossblend_override", params);
	params.Clear();

	if (GetGlobalPropertiesListTotalEntries() == SCRIPT_PROPERTIES_BUILTIN_TOTAL)
		GetGlobalCommandsListTotalEntries();

	if (GetGlobalPropertiesListCRC() == SCRIPT_PROPERTIES_BUILTIN_CRC)
		GetGlobalCommandsListCRC();

	ReadDatabaseFile(File::GetFullResourcePath("/data/scripts/stable/Database.bin", "bin", 0));

	if (GetGlobalPropertiesListTotalEntries() == SCRIPT_PROPERTIES_TOTAL)
		GetGlobalCommandsListTotalEntries();

	if (GetGlobalPropertiesListCRC() == SCRIPT_PROPERTIES_LOADED_CRC)
		GetGlobalCommandsListCRC();

	GlobalScriptsArray[65] = GetScriptByType("list(entity)");
	*/
	//	TODO: much much more here...
}

void GlobalProperty::GetNameAndType(String& outStr) const
{
	if (m_PropertyType != tNOTHING)
	{
		char buf[256] = {};
		strcpy(buf, m_PropertyName);
		buf[strlen(m_PropertyName)] = ':';
		strcat(buf, m_PropertyType->m_TypeName.m_szString);

		outStr = buf;
		return;
	}
	else
		outStr = m_PropertyName;
}

GlobalProperty::GlobalProperty(const char* const propertyname, unsigned int ind)
{
	m_PropertyId = ind;

	size_t propnamestrlen = strchr(propertyname, ':') - propertyname;
	m_PropertyName = new char[propnamestrlen];
	strncpy_s(m_PropertyName, propnamestrlen + 1, propertyname, propnamestrlen);
	m_PropertyType = ScriptType::GetTypeByName(propertyname + propnamestrlen + 1);
}

GlobalProperty::~GlobalProperty()
{
	if (m_PropertyName)
		delete[] m_PropertyName;
}

void GlobalCommand::AddArgumentType(const ScriptType* argtype)
{
	String emptystr;
	m_Arguments.m_ArgumentsList.emplace_back(emptystr, argtype, m_Arguments.m_TotalSizeBytes);
	m_Arguments.m_TotalSizeBytes += argtype->m_Size;
	m_Arguments.m_TotalSize += argtype->GetTypeSize();
}

void GlobalCommand::GetReturnTypeString(String& outStr)
{
	if (m_Arguments.m_ArgumentsList[0].m_ScriptType->m_Size)
	{
		char buf[128] = {};
		sprintf(buf, "%s:%s", m_ArgumentsString, m_Arguments.m_ArgumentsList[0].m_ScriptType->m_TypeName.m_szString);

		outStr = buf;
		return;
	}
	else
		outStr = m_ArgumentsString;
}

GlobalCommand::GlobalCommand(const char* const commandname, const unsigned int commandind)
{
	MESSAGE_CLASS_CREATED(GlobalCommand);

	//m_GlobalIndex = commandind;

	//const char* const ddotpos = strchr(commandname, ':');
	//if (ddotpos)
}

#pragma message(TODO_IMPLEMENTATION)
class ScriptType_Entity* GlobalScript::AssignScriptToEntity(const ScriptType_Entity* parent)
{
	return nullptr;
}

GlobalScript* GlobalScript::GetGlobalScriptByName(const char* name)
{
	if (!name || !*name)
		return nullptr;

	if (!ScriptsList.size())
		return nullptr;

	const size_t namelen = strlen(name);
	for (unsigned int i = 0; i < ScriptsList.size(); ++i)
		if (strncmp(ScriptsList[i].m_Name.m_szString, name, namelen) == NULL)
			return &ScriptsList[i];

	return nullptr;
}

GlobalCommand::CommandArgument::CommandArgument(String& argname, const ScriptType* argtype, unsigned int argsize)
{
	m_PropertyName = argname;
	m_ScriptType = (ScriptType*)argtype;
	m_TotalSizeBytes = argsize;
}