class database_number_ID : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_number_ID", "Node", false, false);
		
		//	TODO: add members and methods to database_number_ID.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_number_ID::Create();