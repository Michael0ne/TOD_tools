class HUD_SimpleCountdown : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_SimpleCountdown", "Node", false, false);
		
		//	TODO: add members and methods to HUD_SimpleCountdown.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_SimpleCountdown::Create();