class database_mission_award : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_mission_award", "Node", false, false);
		
		//	TODO: add members and methods to database_mission_award.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_mission_award::Create();