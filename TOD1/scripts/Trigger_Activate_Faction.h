class Trigger_Activate_Faction : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_Faction", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Activate_Faction.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Activate_Faction::Create();