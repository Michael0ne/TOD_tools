class master_control_trigger_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("master_control_trigger_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to master_control_trigger_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

master_control_trigger_ctrl::Create();