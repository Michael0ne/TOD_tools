class Trigger_Activate_Objective : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_Objective", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Activate_Objective.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Activate_Objective::Create();