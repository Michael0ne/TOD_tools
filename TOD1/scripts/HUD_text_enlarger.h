class HUD_text_enlarger : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_text_enlarger", "Node", false, false);
		
		//	TODO: add members and methods to HUD_text_enlarger.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_text_enlarger::Create();