class master_mission_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("master_mission_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to master_mission_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

master_mission_ctrl::Create();