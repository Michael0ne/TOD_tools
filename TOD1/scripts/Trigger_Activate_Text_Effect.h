class Trigger_Activate_Text_Effect : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_Text_Effect", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Activate_Text_Effect.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Activate_Text_Effect::Create();