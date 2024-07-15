class database_settings_dict : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_settings_dict", "Node", false, false);
		
		//	TODO: add members and methods to database_settings_dict.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_settings_dict::Create();