class target_system_factiontable : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("target_system_factiontable", "Node", false, false);
		
		//	TODO: add members and methods to target_system_factiontable.
		
		inst->CalculateSize();
		
		return inst;
	};
};

target_system_factiontable::Create();