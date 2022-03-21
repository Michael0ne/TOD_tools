class master_mission_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("master_mission_ctrl", "Node", false, true);

		inst->AddMember(GetScriptIdByFullName("Base_Type:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("Show_Data:truth"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("Mission_Nodes:list(Entity)"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("LastMission_Conditions:list(Entity)"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("Mission_List:list(s_Mission_data)"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("Curr_L_Combo:number"), "0.000000", 0);
		inst->AddMember(GetScriptIdByFullName("Curr_Best_Move:integer"), "0", 0);
		inst->AddMember(GetScriptIdByFullName("EnemiesKilledOnCurrentMission:integer"), "0", 0);
		inst->AddMember(GetScriptIdByFullName("NeutralsKilledOnCurrentMission:integer"), "0", 0);
		inst->AddMember(GetScriptIdByFullName("TargetName:string"), nullptr, 0);

		inst->AddMember(GetScriptIdByFullName("Teleport_to:string"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("Current_Mission:integer"), "-1", 0);
		inst->AddMember(GetScriptIdByFullName("Last_Mission:integer"), "-1", 0);
		inst->AddMember(GetScriptIdByFullName("Last_StoryMission:integer"), "-1", 0);
		inst->AddMember(GetScriptIdByFullName("Update_Last_Mission:truth"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("Mission_Result:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("Mission_Score:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("i:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("j:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("Release_MM:truth"), "false", 0);

		inst->AddMember(GetScriptIdByFullName("Start_Mission:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("SMSinceMM:integer"), "0", 0);
		inst->AddMember(GetScriptIdByFullName("DontLoadMap:truth"), "false", 0);
		inst->AddMember(GetScriptIdByFullName("Default_InterMission:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("ConfigMission:string"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("Start_from_game:truth"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("CheatComplete:truth"), "false", 0);
		inst->AddMember(GetScriptIdByFullName("On_Tutorial:truth"), "false", 0);
		inst->AddMember(GetScriptIdByFullName("Cutscene_player:Entity"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("FadeUBS:truth"), nullptr, 0);

		inst->AddMember(GetScriptIdByFullName("Trig_on_mission_load_list:list(Entity)"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("Game_Completed:truth"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("IgnoreAllMissionsUnlocked:truth"), "false", 0);
		inst->AddMember(GetScriptIdByFullName("MissionLoad:truth"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("MissionCanceled:truth"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("SUBTITLENODE:Entity"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("LastSubMap:Entity"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("LastMap:Entity"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("IgnoreWakeup:truth"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("Include_in_load_list:list(s_BlockLoad)"), nullptr, 0);

		inst->AddMember(GetScriptIdByFullName("EndMissionCleanupCalledOnce:truth"), "false", 0);
		inst->AddMember(GetScriptIdByFullName("Current_loaded_MainMap:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("Current_loaded_SubMap:integer"), nullptr, 0);

		inst->CalculateSize();

		return inst;
	}
};

master_mission_ctrl::Create();