class Trigger_Activate_Tutorial_Ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_Tutorial_Ctrl", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Activate_Tutorial_Ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Activate_Tutorial_Ctrl::Create();