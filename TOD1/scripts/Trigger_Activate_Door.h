class Trigger_Activate_Door : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_Door", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Activate_Door.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Activate_Door::Create();