class database_weapon_ammo_ID : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_weapon_ammo_ID", "Node", false, false);
		
		//	TODO: add members and methods to database_weapon_ammo_ID.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_weapon_ammo_ID::Create();