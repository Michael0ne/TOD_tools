#include "ScriptDatabase.h"
#include "LogDump.h"
#include "Performance.h"

namespace Script
{

	#pragma message(TODO_IMPLEMENTATION)
	ScriptType* GetScriptType(const char* _scripttype)
	{
		if (!_scripttype || !*_scripttype)
			return nullptr;

		return nullptr;
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
		GlobalPropertiesList.SetCapacityAndErase(totalScripts);

		if (totalScripts <= NULL)
		{
			unsigned int totalCommands = NULL;
			dbfile.Read(&totalCommands, sizeof(totalCommands));

			GlobalCommandsList.SetCapacityAndErase(totalCommands);

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
				//RegisterGlobalCommand(buffer);

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
				//RegisterGlobalProperty(buffer, false);

				if (propertiesRead++ >= totalScripts)
				{
					unsigned int totalCommands = NULL;
					dbfile.Read(&totalCommands, sizeof(totalCommands));

					GlobalCommandsList.SetCapacityAndErase(totalCommands);

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
						//RegisterGlobalCommand(buffer);

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
		ScriptFieldsList params;

		//	NOTE: initialize all builtin scripts (structures) here.
		//	TODO: once finished, implement it using macro like this: DECLARE_SCRIPT("s_GenericRes", { {"pResource", GetScriptType("Entity")}, {"pPlaceholder", GetScripTtype("Entity")} }).
		params.Add({ "RecievingEntity", GetScriptType("Entity"), NULL });
		params.Add({ "Damage", GetScriptType("number"), NULL });
		params.Add({ "Owner", GetScriptType("Entity"), NULL });
		params.Add({ "inflictor_node", GetScriptType("Entity"), NULL });
		params.Add({ "RecievingCollVolume", GetScriptType("Entity"), NULL });
		params.Add({ "CollisionImpulse", GetScriptType("vector"), NULL });
		params.Add({ "Peercontactpoint", GetScriptType("vector"), NULL });
		params.Add({ "Damage_type", GetScriptType("integer"), NULL });

		new ScriptType_Script("Take_a_hit_info", params);
		params.Clear();

		params.Add({ "pBlock", GetScriptType("Entity"), NULL });
		params.Add({ "sTeleportName", GetScriptType("string"), NULL });

		new ScriptType_Script("s_BlockLoad", params);
		params.Clear();

		params.Add({ "pResource", GetScriptType("Entity"), NULL });
		params.Add({ "pPlaceholder", GetScriptType("Entity"), NULL });

		new ScriptType_Script("s_GenericRes", params);
		params.Clear();

		params.Add({ "pPlaceholder", GetScriptType("Entity"), NULL });
		params.Add({ "RangeSqr", GetScriptType("number"), NULL });
		params.Add({ "bInView", GetScriptType("truth"), NULL });
		params.Add({ "bIsVisible", GetScriptType("truth"), NULL });

		new ScriptType_Script("s_PlaceholderPriority", params);
		params.Clear();

		params.Add({ "RightHandGunList", GetScriptType("list(truth)"), NULL });
		params.Add({ "LeftHandGunList", GetScriptType("list(truth)"), NULL });
		params.Add({ "AmmoList", GetScriptType("list(integer)"), NULL });
		params.Add({ "MaxAmmoList", GetScriptType("list(integer)"), NULL });
		params.Add({ "AmmoInPrimWeapon", GetScriptType("list(integer)"), NULL });
		params.Add({ "AmmoInSecWeapon", GetScriptType("list(integer)"), NULL });

		new ScriptType_Script("s_Inventory", params);
		params.Clear();

		params.Add({ "pPlaceHolder", GetScriptType("Entity"), NULL });
		params.Add({ "vLocalPosition", GetScriptType("vector"), NULL });

		new ScriptType_Script("s_AIPassenger", params);
		params.Clear();

		params.Add({ "PowerUpList", GetScriptType("list(integer)"), NULL });
		params.Add({ "MaxPowerUpList", GetScriptType("list(integer)"), NULL });

		new ScriptType_Script("s_PowerUpInventory", params);
		params.Clear();

		params.Add({ "Type", GetScriptType("integer"), NULL });
		params.Add({ "Sprite", GetScriptType("Entity"), NULL });
		params.Add({ "BGSprite", GetScriptType("Entity"), NULL });
		params.Add({ "OrgSize", GetScriptType("vector"), NULL });
		params.Add({ "NormalSize", GetScriptType("vector"), NULL });
		params.Add({ "NormalPos", GetScriptType("vector"), NULL });
		params.Add({ "SelectedSize", GetScriptType("vector"), NULL });
		params.Add({ "SelectedPos", GetScriptType("vector"), NULL });
		params.Add({ "AmountTB", GetScriptType("Entity"), NULL });
		params.Add({ "AmountTB_BG", GetScriptType("Entity"), NULL });

		new ScriptType_Script("s_PowerUpSprite", params);
		params.Clear();

		params.Add({ "BlipSprite", GetScriptType("Entity"), NULL });
		params.Add({ "LocationSprite", GetScriptType("Entity"), NULL });
		params.Add({ "WatchedEntity", GetScriptType("Entity"), NULL });
		params.Add({ "EntityType", GetScriptType("integer"), NULL });
		params.Add({ "SubMapIndex", GetScriptType("integer"), NULL });

		new ScriptType_Script("s_RadarBlip", params);
		params.Clear();

		params.Add({ "OptionName", GetScriptType("integer"), NULL });
		params.Add({ "TextField", GetScriptType("Entity"), NULL });
		params.Add({ "TextFieldBG", GetScriptType("Entity"), NULL });
		params.Add({ "OptionAction", GetScriptType("integer"), NULL });

		new ScriptType_Script("s_MenuOption", params);
		params.Clear();

		params.Add({ "TaxiZapEntry", GetScriptType("Entity"), NULL });
		params.Add({ "EntryIcon", GetScriptType("Entity"), NULL });
		params.Add({ "ShowIcon", GetScriptType("truth"), NULL });
		params.Add({ "EntryText", GetScriptType("integer"), NULL });
		params.Add({ "EntryType", GetScriptType("integer"), NULL });
		params.Add({ "EntryDetailTextType", GetScriptType("integer"), NULL });
		params.Add({ "EntryDetailTextIndex", GetScriptType("integer"), NULL });
		params.Add({ "EntrySpacing", GetScriptType("truth"), NULL });
		params.Add({ "IsActiveEntry", GetScriptType("truth"), NULL });

		new ScriptType_Script("s_TaxiZapOptions", params);
		params.Clear();

		params.Add({ "IsAchieved", GetScriptType("truth"), NULL });
		params.Add({ "NameTextField", GetScriptType("Entity"), NULL });
		params.Add({ "NameTextFieldBG", GetScriptType("Entity"), NULL });
		params.Add({ "ScoreTextField", GetScriptType("Entity"), NULL });
		params.Add({ "Stroke3", GetScriptType("Entity"), NULL });
		params.Add({ "Stroke4", GetScriptType("Entity"), NULL });
		params.Add({ "FlashEffect", GetScriptType("Entity"), NULL });
		params.Add({ "AwardType", GetScriptType("integer"), NULL });
		params.Add({ "AwardAP", GetScriptType("integer"), NULL });
		params.Add({ "AwardModelList", GetScriptType("list(Entity)"), NULL });
		params.Add({ "AwardModelListBg", GetScriptType("list(Entity)"), NULL });
		params.Add({ "NumOfAwards", GetScriptType("integer"), NULL });

		new ScriptType_Script("s_ScoreBar", params);
		params.Clear();

		params.Add({ "TextBox", GetScriptType("Entity"), NULL });
		params.Add({ "TextBoxBG", GetScriptType("Entity"), NULL });
		params.Add({ "BounceSpeed", GetScriptType("number"), NULL });
		params.Add({ "OrgScale", GetScriptType("number"), NULL });
		params.Add({ "MaxScale", GetScriptType("number"), NULL });
		params.Add({ "Expand", GetScriptType("truth"), NULL });

		new ScriptType_Script("s_BounceText", params);
		params.Clear();

		params.Add({ "Placeholder", GetScriptType("Entity"), NULL });
		params.Add({ "StartPoint", GetScriptType("vector"), NULL });
		params.Add({ "NewPoint", GetScriptType("Entity"), NULL });
		params.Add({ "LastPoint", GetScriptType("Entity"), NULL });
		params.Add({ "currenttime", GetScriptType("number"), NULL });
		params.Add({ "at_pos", GetScriptType("truth"), NULL });

		new ScriptType_Script("s_ped_resource", params);
		params.Clear();

		params.Add({ "pBone", GetScriptType("Entity"), NULL });
		params.Add({ "vCurrentAngle", GetScriptType("vector"), NULL });
		params.Add({ "vTargetAngle", GetScriptType("vector"), NULL });
		params.Add({ "Speed", GetScriptType("number"), NULL });
		params.Add({ "eRotateSpace", GetScriptType("integer"), NULL });
		params.Add({ "bActive", GetScriptType("truth"), NULL });

		new ScriptType_Script("s_CtrlBone", params);
		params.Clear();

		params.Add({ "Objective", GetScriptType("Entity"), NULL });
		params.Add({ "Show", GetScriptType("truth"), NULL });
		params.Add({ "Status", GetScriptType("integer"), NULL });

		new ScriptType_Script("s_Mission_Objective", params);
		params.Clear();

		params.Add({ "Mission_Valid", GetScriptType("truth"), NULL });
		params.Add({ "Mission", GetScriptType("integer"), NULL });
		params.Add({ "Mission_type", GetScriptType("integer"), NULL });
		params.Add({ "Award_achived", GetScriptType("integer"), NULL });
		params.Add({ "Mission_Entity", GetScriptType("Entity"), NULL });
		params.Add({ "Mission_complete", GetScriptType("truth"), NULL });
		params.Add({ "Mission_open", GetScriptType("truth"), NULL });
		params.Add({ "Mission_name", GetScriptType("integer"), NULL });
		params.Add({ "Mission_start_submap", GetScriptType("integer"), NULL });
		params.Add({ "Highest_Score", GetScriptType("integer"), NULL });
		params.Add({ "Current_Score", GetScriptType("integer"), NULL });
		params.Add({ "L_Combo", GetScriptType("number"), NULL });
		params.Add({ "Best_Move_String", GetScriptType("string"), NULL });
		params.Add({ "Best_Move", GetScriptType("integer"), NULL });
		params.Add({ "FollowUpMission", GetScriptType("integer"), NULL });
		params.Add({ "MissionConditions", GetScriptType("list(Entity)"), NULL });
		params.Add({ "ObjectiveList", GetScriptType("list(Entity)"), NULL });

		new ScriptType_Script("s_Mission_data", params);
		params.Clear();

		params.Add({ "TextBox", GetScriptType("Entity"), NULL });
		params.Add({ "TextBox_BG", GetScriptType("Entity"), NULL });
		params.Add({ "SlideUpTime", GetScriptType("number"), NULL });
		params.Add({ "StartSlideTime", GetScriptType("number"), NULL });
		params.Add({ "SlideX", GetScriptType("number"), NULL });
		params.Add({ "SlideY", GetScriptType("number"), NULL });
		params.Add({ "FromPos", GetScriptType("vector"), NULL });
		params.Add({ "TargetPos", GetScriptType("vector"), NULL });
		params.Add({ "FromOpacity", GetScriptType("number"), NULL });
		params.Add({ "TargetOpacity", GetScriptType("number"), NULL });
		params.Add({ "StayTime", GetScriptType("number"), NULL });
		params.Add({ "InUse", GetScriptType("truth"), NULL });

		new ScriptType_Script("s_ComboAwardString", params);
		params.Clear();

		params.Add({ "MoveName", GetScriptType("integer"), NULL });
		params.Add({ "Bonus", GetScriptType("integer"), NULL });
		params.Add({ "GoodieAward", GetScriptType("integer"), NULL });
		params.Add({ "MoveBitName", GetScriptType("integer"), NULL });
		params.Add({ "SoundIndex", GetScriptType("integer"), NULL });
		params.Add({ "SoundFrequency", GetScriptType("number"), NULL });
		params.Add({ "CCmove", GetScriptType("integer"), NULL });

		new ScriptType_Script("s_UniqueComboMove", params);
		params.Clear();

		params.Add({ "KillsNeeded", GetScriptType("integer"), NULL });
		params.Add({ "ComboBonus", GetScriptType("integer"), NULL });
		params.Add({ "GoodieAwards", GetScriptType("list(integer)"), NULL });
		params.Add({ "AwardString", GetScriptType("integer"), NULL });

		new ScriptType_Script("s_ComboAward", params);
		params.Clear();

		params.Add({ "Position", GetScriptType("vector"), NULL });
		params.Add({ "CurrentHealth", GetScriptType("number"), NULL });
		params.Add({ "CurrentWeapons", GetScriptType("list(string)"), NULL });
		params.Add({ "AmmoActiveWeapon", GetScriptType("number"), NULL });

		new ScriptType_Script("s_StatusPoint", params);
		params.Clear();

		params.Add({ "ActionsPoints", GetScriptType("integer"), NULL });
		params.Add({ "EnemyKills", GetScriptType("integer"), NULL });
		params.Add({ "NeutralKills", GetScriptType("integer"), NULL });
		params.Add({ "BarrelsKilled", GetScriptType("integer"), NULL });
		params.Add({ "AvarageHealth", GetScriptType("number"), NULL });
		params.Add({ "UsedKillTypes", GetScriptType("list(string)"), NULL });
		params.Add({ "MostOftenKill", GetScriptType("list(integer)"), NULL });
		params.Add({ "LargestCombo", GetScriptType("string"), NULL });
		params.Add({ "DrivingCar", GetScriptType("integer"), NULL });
		params.Add({ "MenuOpened", GetScriptType("integer"), NULL });
		params.Add({ "MaxNumOfWeapon", GetScriptType("integer"), NULL });
		params.Add({ "MostUsedWeapon", GetScriptType("integer"), NULL });
		params.Add({ "Medals", GetScriptType("integer"), NULL });

		new ScriptType_Script("s_Statistics", params);
		params.Clear();

		params.Add({ "pCallBack", GetScriptType("Entity"), NULL });
		params.Add({ "eMessage", GetScriptType("integer"), NULL });
		params.Add({ "nTimer", GetScriptType("number"), NULL });
		params.Add({ "bRealTime", GetScriptType("truth"), NULL });

		new ScriptType_Script("s_DelayedMessage", params);
		params.Clear();

		params.Add({ "HasCollided", GetScriptType("truth"), NULL });
		params.Add({ "ColEntity", GetScriptType("Entity"), NULL });
		params.Add({ "ColVolume", GetScriptType("Entity"), NULL });
		params.Add({ "ColMaterial", GetScriptType("integer"), NULL });
		params.Add({ "ColFaceProperty", GetScriptType("integer"), NULL });
		params.Add({ "ColNormal", GetScriptType("vector"), NULL });
		params.Add({ "ColPos", GetScriptType("vector"), NULL });
		params.Add({ "SetDecal", GetScriptType("truth"), NULL });

		new ScriptType_Script("s_CollisionInfo", params);
		params.Clear();

		params.Add({ "CollisionEntity", GetScriptType("Entity"), NULL });
		params.Add({ "RealNode", GetScriptType("Entity"), NULL });
		params.Add({ "CollisionPos", GetScriptType("vector"), NULL });
		params.Add({ "ColliderEntity", GetScriptType("Entity"), NULL });

		new ScriptType_Script("s_VehicleCollision", params);
		params.Clear();

		params.Add({ "Sound", GetScriptType("list(Entity)"), NULL });
		params.Add({ "Volume", GetScriptType("list(number)"), NULL });
		params.Add({ "Min_Pause", GetScriptType("list(number)"), NULL });
		params.Add({ "Max_Pause", GetScriptType("list(number)"), NULL });
		params.Add({ "Position_Type", GetScriptType("list(integer)"), NULL });
		params.Add({ "Position_Enity", GetScriptType("list(Entity)"), NULL });

		new ScriptType_Script("s_Random_sound", params);
		params.Clear();

		params.Add({ "ent_1", GetScriptType("Entity"), NULL });
		params.Add({ "ent_2", GetScriptType("Entity"), NULL });

		new ScriptType_Script("s_Entity_pair", params);
		params.Clear();

		params.Add({ "Watched", GetScriptType("Entity"), NULL });
		params.Add({ "WatchedName", GetScriptType("integer"), NULL });
		params.Add({ "HealthPercentage", GetScriptType("number"), NULL });
		params.Add({ "InFocus", GetScriptType("truth"), NULL });
		params.Add({ "LocalShowTime", GetScriptType("number"), NULL });

		new ScriptType_Script("s_HealthWatchedEntity", params);
		params.Clear();

		params.Add({ "GamePulse", GetScriptType("integer"), NULL });
		params.Add({ "UnderPulse", GetScriptType("integer"), NULL });
		params.Add({ "UberPulse", GetScriptType("integer"), NULL });
		params.Add({ "StayTime", GetScriptType("number"), NULL });
		params.Add({ "toHighPulses", GetScriptType("integer"), NULL });

		new ScriptType_Script("s_pulse", params);
		params.Clear();

		params.Add({ "at_entity", GetScriptType("Entity"), NULL });
		params.Add({ "pos_offset", GetScriptType("vector"), NULL });
		params.Add({ "ParticleOwner", GetScriptType("Entity"), NULL });
		params.Add({ "ParticleSys", GetScriptType("Entity"), NULL });
		params.Add({ "at_pos", GetScriptType("vector"), NULL });
		params.Add({ "LifeTime", GetScriptType("number"), NULL });

		new ScriptType_Script("s_particel_dam", params);
		params.Clear();

		params.Add({ "list_index", GetScriptType("integer"), NULL });
		params.Add({ "dist", GetScriptType("number"), NULL });

		new ScriptType_Script("s_list_dist", params);
		params.Clear();

		params.Add({ "PedestrianGroup", GetScriptType("integer"), NULL });
		params.Add({ "Minimum", GetScriptType("integer"), NULL });
		params.Add({ "Maximum", GetScriptType("integer"), NULL });

		new ScriptType_Script("s_neighborhood_config", params);
		params.Clear();

		params.Add({ "PathPoint", GetScriptType("Entity"), NULL });
		params.Add({ "Neighborhood", GetScriptType("Entity"), NULL });

		new ScriptType_Script("s_pedestrian_points", params);
		params.Clear();

		params.Add({ "Object", GetScriptType("Entity"), NULL });
		params.Add({ "StartOpacity", GetScriptType("number"), NULL });
		params.Add({ "OpaPerSec", GetScriptType("number"), NULL });
		params.Add({ "DelayTimer", GetScriptType("number"), NULL });

		new ScriptType_Script("s_fadeEntity", params);
		params.Clear();

		params.Add({ "Valid", GetScriptType("truth"), NULL });
		params.Add({ "Type", GetScriptType("integer"), NULL });
		params.Add({ "Relation", GetScriptType("integer"), NULL });

		new ScriptType_Script("s_target_cache_info", params);
		params.Clear();

		params.Add({ "TargetScore", GetScriptType("integer"), NULL });
		params.Add({ "OverallAwardType", GetScriptType("integer"), NULL });
		params.Add({ "AwardString", GetScriptType("integer"), NULL });
		params.Add({ "TutAwardString", GetScriptType("integer"), NULL });

		new ScriptType_Script("s_Overall_Award", params);
		params.Clear();

		params.Add({ "NotifyThis", GetScriptType("Entity"), NULL });
		params.Add({ "RequiredScore", GetScriptType("integer"), NULL });

		new ScriptType_Script("s_ScoreNotification", params);
		params.Clear();

		params.Add({ "position", GetScriptType("vector"), NULL });
		params.Add({ "normal", GetScriptType("vector"), NULL });
		params.Add({ "type", GetScriptType("integer"), NULL });

		new ScriptType_Script("s_waiting_decal", params);
		params.Clear();

		params.Add({ "particle", GetScriptType("Entity"), NULL });
		params.Add({ "target", GetScriptType("Entity"), NULL });
		params.Add({ "impact_pos", GetScriptType("vector"), NULL });
		params.Add({ "impact_orient", GetScriptType("quaternion"), NULL });

		new ScriptType_Script("s_waiting_particle", params);
		params.Clear();

		params.Add({ "position", GetScriptType("vector"), NULL });
		params.Add({ "normal", GetScriptType("vector"), NULL });
		params.Add({ "impact_dir", GetScriptType("vector"), NULL });
		params.Add({ "size", GetScriptType("number"), NULL });
		params.Add({ "type", GetScriptType("integer"), NULL });
		params.Add({ "probe", GetScriptType("Entity"), NULL });
		params.Add({ "ignore", GetScriptType("Entity"), NULL });

		new ScriptType_Script("s_waiting_blood_decal", params);
		params.Clear();

		params.Add({ "dist", GetScriptType("number"), NULL });
		params.Add({ "FromPoint", GetScriptType("Entity"), NULL });
		params.Add({ "ToPoint", GetScriptType("Entity"), NULL });
		params.Add({ "OneOverLength", GetScriptType("number"), NULL });
		params.Add({ "Status", GetScriptType("integer"), NULL });
		params.Add({ "EndStatus", GetScriptType("integer"), NULL });
		params.Add({ "Type", GetScriptType("integer"), NULL });
		params.Add({ "Queue", GetScriptType("list(Entity)"), NULL });

		new ScriptType_Script("s_traffic_section", params);
		params.Clear();

		params.Add({ "CaptionIndex", GetScriptType("integer"), NULL });
		params.Add({ "Tutorials", GetScriptType("list(Entity)"), NULL });

		new ScriptType_Script("s_TutorialMenu", params);
		params.Clear();

		params.Add({ "obj_text", GetScriptType("integer"), NULL });
		params.Add({ "obj_text_alt", GetScriptType("integer"), NULL });
		params.Add({ "use_pause_mode", GetScriptType("truth"), NULL });
		params.Add({ "delay", GetScriptType("number"), NULL });
		params.Add({ "justify", GetScriptType("truth"), NULL });

		new ScriptType_Script("s_TutorialPopUp", params);
		params.Clear();

		params.Add({ "logic_condition", GetScriptType("integer"), NULL });
		params.Add({ "actor", GetScriptType("Entity"), NULL });

		new ScriptType_Script("s_PendingLogicalCondition", params);
		params.Clear();

		params.Add({ "TutorialText", GetScriptType("integer"), NULL });
		params.Add({ "index_list", GetScriptType("list(integer)"), NULL });

		new ScriptType_Script("s_TutorialIndex", params);
		params.Clear();

		params.Add({ "LanguageName", GetScriptType("string"), NULL });
		params.Add({ "Language", GetScriptType("integer"), NULL });
		params.Add({ "Flag", GetScriptType("Entity"), NULL });
		params.Add({ "TextB", GetScriptType("Entity"), NULL });
		params.Add({ "LanguageCode", GetScriptType("string"), NULL });

		new ScriptType_Script("s_Language", params);
		params.Clear();

		params.Add({ "unit", GetScriptType("Entity"), NULL });
		params.Add({ "unit_pos", GetScriptType("vector"), NULL });
		params.Add({ "unit_orient", GetScriptType("quaternion"), NULL });
		params.Add({ "unit_velocity", GetScriptType("vector"), NULL });

		new ScriptType_Script("element", params);
		params.Clear();

		params.Add({ "unit", GetScriptType("Entity"), NULL });
		params.Add({ "hinge", GetScriptType("vector"), NULL });
		params.Add({ "unit_orient", GetScriptType("quaternion"), NULL });
		params.Add({ "unit_angle_velocity", GetScriptType("number"), NULL });
		params.Add({ "unit_angle", GetScriptType("number"), NULL });
		params.Add({ "unit_max_angle", GetScriptType("number"), NULL });
		params.Add({ "unit_zones", GetScriptType("integer"), NULL });
		params.Add({ "unit_in_use", GetScriptType("integer"), NULL });
		params.Add({ "unit_rotation_inverse", GetScriptType("number"), NULL });
		params.Add({ "unit_bent_step", GetScriptType("integer"), NULL });
		params.Add({ "unit_grav_direction", GetScriptType("number"), NULL });

		new ScriptType_Script("hinge_element", params);
		params.Clear();

		params.Add({ "parameter_name", GetScriptType("string"), NULL });
		params.Add({ "parameter_value", GetScriptType("number"), NULL });

		new ScriptType_Script("parameter", params);
		params.Clear();

		params.Add({ "crossblend_speed", GetScriptType("number"), NULL });
		params.Add({ "anim_slot", GetScriptType("Entity"), NULL });

		new ScriptType_Script("crossblend_override", params);
		params.Clear();

		//if (GetGlobalPropertiesListTotalEntries() == 711)
			//GetGlobalCommandsListTotalEntries();

		//if (GetGlobalPropertiesListCRC() == 0xE99606BA)
			//GetGlobalCommandsListCRC();

		//ReadDatabaseFile(File::GetFullResourcePath("/data/scripts/stable/Database.bin", "bin", 0));

		//if (GetGlobalPropertiesListTotalEntries() == 5994)
			//GetGlobalCommandsListTotalEntries();

		//if (GetGlobalPropertiesListCRC() == 0x65C37710)
			//GetGlobalCommandsListCRC();

		//GlobalScriptsArray[65] = GetScriptByType("list(entity)");
		//	TODO: much much more here...
	}
}