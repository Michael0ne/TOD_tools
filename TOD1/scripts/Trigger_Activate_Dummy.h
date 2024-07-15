class Trigger_Activate_Dummy : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_Dummy", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Activate_Dummy.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Activate_Dummy::Create();