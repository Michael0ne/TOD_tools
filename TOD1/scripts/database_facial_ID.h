class database_facial_ID : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_facial_ID", "Node", false, false);
		
		//	TODO: add members and methods to database_facial_ID.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_facial_ID::Create();