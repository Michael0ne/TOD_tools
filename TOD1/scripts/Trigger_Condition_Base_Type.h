class Trigger_Condition_Base_Type : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Condition_Base_Type", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Condition_Base_Type.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Condition_Base_Type::Create();