class database_Mission_ID : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_Mission_ID", "Node", false, false);
		
		//	TODO: add members and methods to database_Mission_ID.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_Mission_ID::Create();