class Trigger_Activate_Area_Damage : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_Area_Damage", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Activate_Area_Damage.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Activate_Area_Damage::Create();