class Database_Sound_Weapon_Specific_ID : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Database_Sound_Weapon_Specific_ID", "Node", false, false);
		
		//	TODO: add members and methods to Database_Sound_Weapon_Specific_ID.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Database_Sound_Weapon_Specific_ID::Create();