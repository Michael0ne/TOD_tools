class master_game_time_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("master_game_time_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to master_game_time_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

master_game_time_ctrl::Create();