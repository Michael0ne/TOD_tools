class Trigger_Activate_Weapons : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_Weapons", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Activate_Weapons.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Activate_Weapons::Create();