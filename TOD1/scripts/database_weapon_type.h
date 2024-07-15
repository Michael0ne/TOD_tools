class database_weapon_type : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_weapon_type", "Node", false, false);
		
		//	TODO: add members and methods to database_weapon_type.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_weapon_type::Create();