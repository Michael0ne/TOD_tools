class default_weapon : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("default_weapon", "Node", false, false);
		
		//	TODO: add members and methods to default_weapon.
		
		inst->CalculateSize();
		
		return inst;
	};
};

default_weapon::Create();