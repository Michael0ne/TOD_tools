class HUD_combo_reaction_sign : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_combo_reaction_sign", "Node", false, false);
		
		//	TODO: add members and methods to HUD_combo_reaction_sign.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_combo_reaction_sign::Create();