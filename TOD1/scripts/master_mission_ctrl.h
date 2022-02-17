class master_mission_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("master_mission_ctrl", "Node", false, true);

		inst->AddStructElement(GetScriptIdByFullName("Base_Type:integer"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("Show_Data:truth"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("Mission_Nodes:list(Entity)"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("LastMission_Conditions:list(Entity)"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("Mission_List:list(s_Mission_data)"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("Curr_L_Combo:number"), "0.000000", 0);
		inst->AddStructElement(GetScriptIdByFullName("Curr_Best_Move:integer"), "0", 0);
		inst->AddStructElement(GetScriptIdByFullName("EnemiesKilledOnCurrentMission:integer"), "0", 0);
		inst->AddStructElement(GetScriptIdByFullName("NeutralsKilledOnCurrentMission:integer"), "0", 0);
		inst->AddStructElement(GetScriptIdByFullName("TargetName:string"), nullptr, 0);

		inst->AddStructElement(GetScriptIdByFullName("Teleport_to:string"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("Current_Mission:integer"), "-1", 0);
		inst->AddStructElement(GetScriptIdByFullName("Last_Mission:integer"), "-1", 0);
		inst->AddStructElement(GetScriptIdByFullName("Last_StoryMission:integer"), "-1", 0);
		inst->AddStructElement(GetScriptIdByFullName("Update_Last_Mission:truth"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("Mission_Result:integer"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("Mission_Score:integer"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("i:integer"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("j:integer"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("Release_MM:truth"), "false", 0);

		inst->AddStructElement(GetScriptIdByFullName("Start_Mission:integer"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("SMSinceMM:integer"), "0", 0);
		inst->AddStructElement(GetScriptIdByFullName("DontLoadMap:truth"), "false", 0);
		inst->AddStructElement(GetScriptIdByFullName("Default_InterMission:integer"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("ConfigMission:string"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("Start_from_game:truth"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("CheatComplete:truth"), "false", 0);
		inst->AddStructElement(GetScriptIdByFullName("On_Tutorial:truth"), "false", 0);
		inst->AddStructElement(GetScriptIdByFullName("Cutscene_player:Entity"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("FadeUBS:truth"), nullptr, 0);

		inst->AddStructElement(GetScriptIdByFullName("Trig_on_mission_load_list:list(Entity)"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("Game_Completed:truth"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("IgnoreAllMissionsUnlocked:truth"), "false", 0);
		inst->AddStructElement(GetScriptIdByFullName("MissionLoad:truth"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("MissionCanceled:truth"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("SUBTITLENODE:Entity"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("LastSubMap:Entity"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("LastMap:Entity"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("IgnoreWakeup:truth"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("Include_in_load_list:list(s_BlockLoad)"), nullptr, 0);

		inst->AddStructElement(GetScriptIdByFullName("EndMissionCleanupCalledOnce:truth"), "false", 0);
		inst->AddStructElement(GetScriptIdByFullName("Current_loaded_MainMap:integer"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("Current_loaded_SubMap:integer"), nullptr, 0);

		inst->CalculateSize();

		return inst;
	}
};

master_mission_ctrl::Create();