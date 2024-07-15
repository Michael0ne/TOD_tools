class master_control_save_ctrl_PC : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("master_control_save_ctrl_PC", "Node", false, false);
		
		//	TODO: add members and methods to master_control_save_ctrl_PC.
		
		inst->CalculateSize();
		
		return inst;
	};
};

master_control_save_ctrl_PC::Create();