class weapon_effects_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("weapon_effects_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to weapon_effects_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

weapon_effects_ctrl::Create();