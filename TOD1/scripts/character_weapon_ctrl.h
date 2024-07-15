class character_weapon_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("character_weapon_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to character_weapon_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

character_weapon_ctrl::Create();