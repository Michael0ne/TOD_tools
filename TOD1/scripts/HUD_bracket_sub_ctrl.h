class HUD_bracket_sub_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_bracket_sub_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to HUD_bracket_sub_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_bracket_sub_ctrl::Create();