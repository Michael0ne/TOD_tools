class HUD_radar : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_radar", "Node", false, false);
		
		//	TODO: add members and methods to HUD_radar.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_radar::Create();