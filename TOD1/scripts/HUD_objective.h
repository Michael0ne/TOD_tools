class HUD_objective : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_objective", "Node", false, false);
		
		//	TODO: add members and methods to HUD_objective.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_objective::Create();