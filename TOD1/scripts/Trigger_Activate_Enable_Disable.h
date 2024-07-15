class Trigger_Activate_Enable_Disable : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_Enable_Disable", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Activate_Enable_Disable.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Activate_Enable_Disable::Create();