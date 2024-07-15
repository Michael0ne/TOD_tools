class Trigger_Activate_Tutorial : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_Tutorial", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Activate_Tutorial.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Activate_Tutorial::Create();