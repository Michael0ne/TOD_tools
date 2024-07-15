class Trigger_Activate_Racing_Countdown : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_Racing_Countdown", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Activate_Racing_Countdown.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Activate_Racing_Countdown::Create();