class HUD_ammo_bar : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_ammo_bar", "Node", false, false);
		
		//	TODO: add members and methods to HUD_ammo_bar.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_ammo_bar::Create();