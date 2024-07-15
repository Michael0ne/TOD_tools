class HUD_combo_root : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_combo_root", "Node", false, false);
		
		//	TODO: add members and methods to HUD_combo_root.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_combo_root::Create();