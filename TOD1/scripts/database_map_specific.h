class database_map_specific : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_map_specific", "Node", false, false);
		
		//	TODO: add members and methods to database_map_specific.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_map_specific::Create();