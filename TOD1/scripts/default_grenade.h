class default_grenade : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("default_grenade", "Node", false, false);
		
		//	TODO: add members and methods to default_grenade.
		
		inst->CalculateSize();
		
		return inst;
	};
};

default_grenade::Create();