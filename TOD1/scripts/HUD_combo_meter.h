class HUD_combo_meter : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_combo_meter", "Node", false, false);
		
		//	TODO: add members and methods to HUD_combo_meter.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_combo_meter::Create();