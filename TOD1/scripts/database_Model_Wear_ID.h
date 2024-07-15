class database_Model_Wear_ID : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_Model_Wear_ID", "Node", false, false);
		
		//	TODO: add members and methods to database_Model_Wear_ID.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_Model_Wear_ID::Create();