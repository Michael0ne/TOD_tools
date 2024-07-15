class master_ctrl_rumble_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("master_ctrl_rumble_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to master_ctrl_rumble_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

master_ctrl_rumble_ctrl::Create();