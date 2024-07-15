class database_mission_award_info : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_mission_award_info", "Node", false, false);
		
		//	TODO: add members and methods to database_mission_award_info.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_mission_award_info::Create();