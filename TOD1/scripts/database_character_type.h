class database_character_type : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_character_type", "Node", false, false);
		
		//	TODO: add members and methods to database_character_type.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_character_type::Create();