class Trigger_Activate_Reset_Character : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_Reset_Character", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Activate_Reset_Character.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Activate_Reset_Character::Create();