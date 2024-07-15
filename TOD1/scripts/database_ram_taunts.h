class database_ram_taunts : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_ram_taunts", "Node", false, false);
		
		//	TODO: add members and methods to database_ram_taunts.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_ram_taunts::Create();