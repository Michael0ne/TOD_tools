class database_pedestrian_type : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_pedestrian_type", "Node", false, false);
		
		//	TODO: add members and methods to database_pedestrian_type.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_pedestrian_type::Create();