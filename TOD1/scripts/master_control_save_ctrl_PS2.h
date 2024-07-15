class master_control_save_ctrl_PS2 : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("master_control_save_ctrl_PS2", "Node", false, false);
		
		//	TODO: add members and methods to master_control_save_ctrl_PS2.
		
		inst->CalculateSize();
		
		return inst;
	};
};

master_control_save_ctrl_PS2::Create();