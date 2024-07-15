class Trigger_Activate_MiscEffect : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_MiscEffect", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Activate_MiscEffect.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Activate_MiscEffect::Create();