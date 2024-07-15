class HUD_on_foot_root : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_on_foot_root", "Node", false, false);
		
		//	TODO: add members and methods to HUD_on_foot_root.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_on_foot_root::Create();