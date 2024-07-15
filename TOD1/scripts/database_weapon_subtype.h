class database_weapon_subtype : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_weapon_subtype", "Node", false, false);
		
		//	TODO: add members and methods to database_weapon_subtype.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_weapon_subtype::Create();