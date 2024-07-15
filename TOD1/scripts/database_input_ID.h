class database_input_ID : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_input_ID", "Node", false, false);
		
		//	TODO: add members and methods to database_input_ID.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_input_ID::Create();