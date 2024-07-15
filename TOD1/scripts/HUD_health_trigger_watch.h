class HUD_health_trigger_watch : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_health_trigger_watch", "Node", false, false);
		
		//	TODO: add members and methods to HUD_health_trigger_watch.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_health_trigger_watch::Create();