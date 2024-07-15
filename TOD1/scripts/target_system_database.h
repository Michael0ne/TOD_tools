class target_system_database : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("target_system_database", "Node", false, false);
		
		//	TODO: add members and methods to target_system_database.
		
		inst->CalculateSize();
		
		return inst;
	};
};

target_system_database::Create();