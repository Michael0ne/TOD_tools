class HUD_debug : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_debug", "Node", false, false);
		
		//	TODO: add members and methods to HUD_debug.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_debug::Create();