class database_SubMap_ID : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_SubMap_ID", "Node", false, false);
		
		//	TODO: add members and methods to database_SubMap_ID.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_SubMap_ID::Create();