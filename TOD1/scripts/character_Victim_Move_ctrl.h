class character_Victim_Move_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("character_Victim_Move_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to character_Victim_Move_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

character_Victim_Move_ctrl::Create();