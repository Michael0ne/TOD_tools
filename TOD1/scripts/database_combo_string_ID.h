class database_combo_string_ID : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_combo_string_ID", "Node", false, false);
		
		//	TODO: add members and methods to database_combo_string_ID.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_combo_string_ID::Create();