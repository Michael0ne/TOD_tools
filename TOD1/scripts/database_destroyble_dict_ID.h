class database_destroyble_dict_ID : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_destroyble_dict_ID", "Node", false, false);
		
		//	TODO: add members and methods to database_destroyble_dict_ID.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_destroyble_dict_ID::Create();