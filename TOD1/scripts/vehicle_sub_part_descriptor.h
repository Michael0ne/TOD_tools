class vehicle_sub_part_descriptor : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("vehicle_sub_part_descriptor", "Node", false, false);

		inst->AddMember(GetScriptIdByFullName("Base_Type:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("Sub_Type:integer"), nullptr, 0);

		inst->CalculateSize();

		return inst;
	};
};

vehicle_sub_part_descriptor::Create();