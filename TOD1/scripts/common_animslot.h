class common_animslot : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("common_animslot", "Node", false, false);

		inst->AddMember(GetScriptIdByFullName("CurrentAnimSlot:Entity"), nullptr, 0);

		inst->CalculateSize();

		return inst;
	};
};

common_animslot::Create();