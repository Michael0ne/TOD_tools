class HUD_root : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_root", "Node", false, false);
		
		//	TODO: add members and methods to HUD_root.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_root::Create();