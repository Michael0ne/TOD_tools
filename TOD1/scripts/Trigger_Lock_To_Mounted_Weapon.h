class Trigger_Lock_To_Mounted_Weapon : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Lock_To_Mounted_Weapon", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Lock_To_Mounted_Weapon.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Lock_To_Mounted_Weapon::Create();