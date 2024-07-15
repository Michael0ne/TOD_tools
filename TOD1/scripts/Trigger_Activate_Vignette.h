class Trigger_Activate_Vignette : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_Vignette", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Activate_Vignette.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Activate_Vignette::Create();