class HUD_power_up_effects_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_power_up_effects_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to HUD_power_up_effects_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_power_up_effects_ctrl::Create();