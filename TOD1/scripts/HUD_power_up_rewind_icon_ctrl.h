class HUD_power_up_rewind_icon_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_power_up_rewind_icon_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to HUD_power_up_rewind_icon_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_power_up_rewind_icon_ctrl::Create();