class HUD_overall_score : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_overall_score", "Node", false, false);
		
		//	TODO: add members and methods to HUD_overall_score.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_overall_score::Create();