class character_Inside_vehicle_Move_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("character_Inside_vehicle_Move_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to character_Inside_vehicle_Move_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

character_Inside_vehicle_Move_ctrl::Create();