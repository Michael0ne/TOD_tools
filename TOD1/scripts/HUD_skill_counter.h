class HUD_skill_counter : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_skill_counter", "Node", false, false);
		
		//	TODO: add members and methods to HUD_skill_counter.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_skill_counter::Create();