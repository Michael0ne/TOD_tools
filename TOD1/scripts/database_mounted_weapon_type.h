class database_mounted_weapon_type : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_mounted_weapon_type", "Node", false, false);
		
		//	TODO: add members and methods to database_mounted_weapon_type.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_mounted_weapon_type::Create();