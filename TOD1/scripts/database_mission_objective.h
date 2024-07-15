class database_mission_objective : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_mission_objective", "Node", false, false);
		
		//	TODO: add members and methods to database_mission_objective.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_mission_objective::Create();