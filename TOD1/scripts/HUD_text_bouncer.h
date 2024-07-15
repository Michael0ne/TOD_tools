class HUD_text_bouncer : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_text_bouncer", "Node", false, false);
		
		//	TODO: add members and methods to HUD_text_bouncer.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_text_bouncer::Create();