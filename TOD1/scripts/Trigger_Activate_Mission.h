class Trigger_Activate_Mission : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_Mission", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Activate_Mission.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Activate_Mission::Create();