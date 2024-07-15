class Database_Sound_ID : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Database_Sound_ID", "Node", false, false);
		
		//	TODO: add members and methods to Database_Sound_ID.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Database_Sound_ID::Create();