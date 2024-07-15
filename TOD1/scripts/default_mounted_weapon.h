class default_mounted_weapon : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("default_mounted_weapon", "Node", false, false);
		
		//	TODO: add members and methods to default_mounted_weapon.
		
		inst->CalculateSize();
		
		return inst;
	};
};

default_mounted_weapon::Create();