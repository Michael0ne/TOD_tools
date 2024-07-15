class character_powerup_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("character_powerup_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to character_powerup_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

character_powerup_ctrl::Create();