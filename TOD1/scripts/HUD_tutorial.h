class HUD_tutorial : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_tutorial", "Node", false, false);
		
		//	TODO: add members and methods to HUD_tutorial.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_tutorial::Create();