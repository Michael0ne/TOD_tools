class Trigger_Activate_Combo : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_Combo", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Activate_Combo.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Activate_Combo::Create();