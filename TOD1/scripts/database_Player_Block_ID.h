class database_Player_Block_ID : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_Player_Block_ID", "Node", false, false);
		
		//	TODO: add members and methods to database_Player_Block_ID.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_Player_Block_ID::Create();