class database_vehicle_type : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_vehicle_type", "Node", false, false);
		
		//	TODO: add members and methods to database_vehicle_type.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_vehicle_type::Create();