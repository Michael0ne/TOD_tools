class database_vehicle_subtype : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_vehicle_subtype", "Node", false, false);
		
		//	TODO: add members and methods to database_vehicle_subtype.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_vehicle_subtype::Create();