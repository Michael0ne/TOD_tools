class barrel_respawn_manager : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("barrel_respawn_manager", "Node", false, false);
		
		//	TODO: add members and methods to barrel_respawn_manager.
		
		inst->CalculateSize();
		
		return inst;
	};
};

barrel_respawn_manager::Create();