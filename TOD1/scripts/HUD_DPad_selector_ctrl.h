class HUD_DPad_selector_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_DPad_selector_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to HUD_DPad_selector_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_DPad_selector_ctrl::Create();