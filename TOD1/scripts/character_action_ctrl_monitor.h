class character_action_ctrl_monitor : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("character_action_ctrl_monitor", "Node", false, false);
		
		//	TODO: add members and methods to character_action_ctrl_monitor.
		
		inst->CalculateSize();
		
		return inst;
	};
};

character_action_ctrl_monitor::Create();