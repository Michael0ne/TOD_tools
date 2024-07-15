class HUD_health_effects_bar : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_health_effects_bar", "Node", false, false);
		
		//	TODO: add members and methods to HUD_health_effects_bar.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_health_effects_bar::Create();