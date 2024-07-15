class master_control_save_ctrl_XBOX : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("master_control_save_ctrl_XBOX", "Node", false, false);
		
		//	TODO: add members and methods to master_control_save_ctrl_XBOX.
		
		inst->CalculateSize();
		
		return inst;
	};
};

master_control_save_ctrl_XBOX::Create();