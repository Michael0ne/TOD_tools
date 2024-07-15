class database_pedestrian_group : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_pedestrian_group", "Node", false, false);
		
		//	TODO: add members and methods to database_pedestrian_group.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_pedestrian_group::Create();