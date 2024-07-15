class HUD_fade : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_fade", "Node", false, false);
		
		//	TODO: add members and methods to HUD_fade.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_fade::Create();