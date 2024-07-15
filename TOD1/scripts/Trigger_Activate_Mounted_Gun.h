class Trigger_Activate_Mounted_Gun : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_Mounted_Gun", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Activate_Mounted_Gun.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Activate_Mounted_Gun::Create();