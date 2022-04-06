class reverse_sprite_fader : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("reverse_sprite_fader", "Node", false, false);

		inst->AddMember(GetScriptIdByFullName("MaxDistance:number"), "25.000000", NULL);
		inst->AddMember(GetScriptIdByFullName("MinDistance:number"), "2.000000", NULL);
		inst->AddMember(GetScriptIdByFullName("reverse:truth"), "true", NULL);
		inst->AddMember(GetScriptIdByFullName("AllowActivation:truth"), "true", NULL);
		inst->AddMember(GetScriptIdByFullName("one_over_distance:number"), nullptr, NULL);
		inst->AddMember(GetScriptIdByFullName("SelfNode:Entity"), nullptr, NULL);

		inst->AddMethod(314, nullptr, nullptr);
		inst->AddMethod(353, nullptr, nullptr);

		inst->AddLocal(nullptr, nullptr);
		inst->AddLocal(nullptr, nullptr);

		inst->CalculateSize();
		return inst;
	}
};

reverse_sprite_fader::Create();