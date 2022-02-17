class Trigger_Activate_Weapons : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_Weapons", "Node", false, false);

		inst->AddStructElement(GetScriptIdByFullName("ActionType:integer"), "51", 0);
		inst->AddStructElement(GetScriptIdByFullName("Sub_Type:integer"), "0", 0);
		inst->AddStructElement(GetScriptIdByFullName("ActivateWho:integer"), "0", 0);
		inst->AddStructElement(GetScriptIdByFullName("Weapon_Type:integer"), "0", 0);
		inst->AddStructElement(GetScriptIdByFullName("Ammo_Amount:integer"), "10", 0);
		inst->AddStructElement(GetScriptIdByFullName("SwitchToNewWeapon:truth"), "false", 0);
		inst->AddStructElement(139, nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("Delay:number"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("APBonus:integer"), "0", 0);
		inst->AddStructElement(GetScriptIdByFullName("TargetEntity:Entity"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("SoundEntity:Entity"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("TriggerSoundEmitter:Entity"), nullptr, 0);
		inst->AddStructElement(GetScriptIdByFullName("Activated:truth"), "false", 0);
		inst->AddStructElement(GetScriptIdByFullName("AllowActivate__truth"), "true", 0);

		inst->CalculateSize();

		return inst;
	};
};

Trigger_Activate_Weapons::Create();