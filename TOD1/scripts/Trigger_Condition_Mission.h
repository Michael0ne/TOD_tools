class Trigger_Condition_Mission : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Condition_Mission", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Condition_Mission.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Condition_Mission::Create();