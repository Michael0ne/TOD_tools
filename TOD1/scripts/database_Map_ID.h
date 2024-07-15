class database_Map_ID : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_Map_ID", "Node", false, false);
		
		//	TODO: add members and methods to database_Map_ID.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_Map_ID::Create();