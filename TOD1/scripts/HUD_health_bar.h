class HUD_health_bar : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_health_bar", "Node", true, false);

		//inst->field_60 = _6A6780();

		inst->AddMember(GetScriptIdByFullName("MonitorStamina:truth"), "false", 0);
		inst->AddMember(GetScriptIdByFullName("InitOK:truth"), "true", 0);
		inst->AddMember(GetScriptIdByFullName("Resizing:truth"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("HealthEffectsBar:Entity"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("LeftBracket:Entity"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("RightBracket:Entity"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("MidSection:Entity"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("HealthBarFillSprite:Entity"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("HealthBarGodSprite:Entity"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("FrameOpacity:number"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("BasicHealth:number"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("CurrentHealthPercentage:number"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("CriticalHealthLimit:number"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("HealthBarOrgSize:number"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("HealthBarCurrentSize:number"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("ShowTime:number"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("FadeTime:number"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("LeftOffset:number"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("RightOffset:number"), nullptr, 0);

		inst->CalculateSize();

		return inst;
	}
};

HUD_health_bar::Create();