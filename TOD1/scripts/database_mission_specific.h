class database_mission_specific : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_mission_specific", "Node", false, false);
		
		//	TODO: add members and methods to database_mission_specific.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_mission_specific::Create();