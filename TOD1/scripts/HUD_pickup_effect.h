class HUD_pickup_effect : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("HUD_pickup_effect", "Node", false, false);
		
		//	TODO: add members and methods to HUD_pickup_effect.
		
		inst->CalculateSize();
		
		return inst;
	};
};

HUD_pickup_effect::Create();