class database_traffic_point : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_traffic_point", "Node", false, false);
		
		//	TODO: add members and methods to database_traffic_point.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_traffic_point::Create();