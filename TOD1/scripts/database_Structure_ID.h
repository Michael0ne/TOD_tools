class database_Structure_ID : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_Structure_ID", "Node", false, false);
		
		//	TODO: add members and methods to database_Structure_ID.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_Structure_ID::Create();