class database_damage_modifier : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_damage_modifier", "Node", false, false);
		
		//	TODO: add members and methods to database_damage_modifier.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_damage_modifier::Create();