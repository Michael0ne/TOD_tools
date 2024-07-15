class Trigger_Activate_Prop : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_Prop", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Activate_Prop.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Activate_Prop::Create();