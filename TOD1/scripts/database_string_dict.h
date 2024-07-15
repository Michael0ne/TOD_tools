class database_string_dict : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_string_dict", "Node", false, false);
		
		//	TODO: add members and methods to database_string_dict.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_string_dict::Create();