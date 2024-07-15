class trafficsystem_vehicledata : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("trafficsystem_vehicledata", "Node", false, false);
		
		//	TODO: add members and methods to trafficsystem_vehicledata.
		
		inst->CalculateSize();
		
		return inst;
	};
};

trafficsystem_vehicledata::Create();