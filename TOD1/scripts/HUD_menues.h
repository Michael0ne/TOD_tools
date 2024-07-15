class HUD_menues : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_menues", "Node", false, false);
		
		//	TODO: add members and methods to HUD_menues.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_menues::Create();