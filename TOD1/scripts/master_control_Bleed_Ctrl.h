class master_control_Bleed_Ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("master_control_Bleed_Ctrl", "Node", false, false);
		
		//	TODO: add members and methods to master_control_Bleed_Ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

master_control_Bleed_Ctrl::Create();