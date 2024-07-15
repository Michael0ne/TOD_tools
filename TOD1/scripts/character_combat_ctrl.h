class character_combat_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("character_combat_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to character_combat_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

character_combat_ctrl::Create();