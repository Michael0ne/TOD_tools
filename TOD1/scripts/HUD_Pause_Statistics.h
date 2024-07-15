class HUD_Pause_Statistics : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_Pause_Statistics", "Node", false, false);
		
		//	TODO: add members and methods to HUD_Pause_Statistics.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_Pause_Statistics::Create();