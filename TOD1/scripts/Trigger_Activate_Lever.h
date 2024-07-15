class Trigger_Activate_Lever : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_Lever", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Activate_Lever.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Activate_Lever::Create();