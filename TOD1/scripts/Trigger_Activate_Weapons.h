class Trigger_Activate_Weapons : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_Weapons", "Node", false, false);

		inst->AddMember(GetScriptIdByFullName("ActionType:integer"), "51", 0);
		inst->AddMember(GetScriptIdByFullName("Sub_Type:integer"), "0", 0);
		inst->AddMember(GetScriptIdByFullName("ActivateWho:integer"), "0", 0);
		inst->AddMember(GetScriptIdByFullName("Weapon_Type:integer"), "0", 0);
		inst->AddMember(GetScriptIdByFullName("Ammo_Amount:integer"), "10", 0);
		inst->AddMember(GetScriptIdByFullName("SwitchToNewWeapon:truth"), "false", 0);
		inst->AddMember(139, nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("Delay:number"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("APBonus:integer"), "0", 0);
		inst->AddMember(GetScriptIdByFullName("TargetEntity:Entity"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("SoundEntity:Entity"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("TriggerSoundEmitter:Entity"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("Activated:truth"), "false", 0);
		inst->AddMember(GetScriptIdByFullName("AllowActivate:truth"), "true", 0);

		inst->CalculateSize();

		return inst;
	};
};

Trigger_Activate_Weapons::Create();