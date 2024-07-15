class Trigger_Activate_Cutscene : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_Cutscene", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Activate_Cutscene.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Activate_Cutscene::Create();