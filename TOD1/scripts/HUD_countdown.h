class HUD_countdown : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_countdown", "Node", false, false);
		
		//	TODO: add members and methods to HUD_countdown.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_countdown::Create();