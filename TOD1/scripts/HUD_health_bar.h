class HUD_health_bar : public GlobalScript
{
public:
	static GlobalScript* Create()
	{
		static GlobalScript* inst = new GlobalScript("HUD_health_bar", "Node", true, false);

		//inst->field_60 = _6A6780();

		inst->AddStructElement(GetScriptIdByFullName("MonitorStamina:truth"), "false", 0);
		inst->AddStructElement(GetScriptIdByFullName("InitOK:truth"), "true", 0);
		inst->AddStructElement(GetScriptIdByFullName("Resizing:truth"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("HealthEffectsBar:Entity"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("LeftBracket:Entity"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("RightBracket:Entity"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("MidSection:Entity"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("HealthBarFillSprite:Entity"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("HealthBarGodSprite:Entity"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("FrameOpacity:number"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("BasicHealth:number"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("CurrentHealthPercentage:number"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("CriticalHealthLimit:number"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("HealthBarOrgSize:number"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("HealthBarCurrentSize:number"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("ShowTime:number"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("FadeTime:number"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("LeftOffset:number"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("RightOffset:number"), nullptr, 0);

		inst->CalculateSize();

		return inst;
	}
};

HUD_health_bar::Create();