class HUD_mission : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_mission", "Node", false, false);
		
		//	TODO: add members and methods to HUD_mission.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_mission::Create();