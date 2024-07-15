class database_goodie_ID : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_goodie_ID", "Node", false, false);
		
		//	TODO: add members and methods to database_goodie_ID.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_goodie_ID::Create();