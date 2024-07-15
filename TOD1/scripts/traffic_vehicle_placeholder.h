class traffic_vehicle_placeholder : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("traffic_vehicle_placeholder", "Node", false, false);
		
		//	TODO: add members and methods to traffic_vehicle_placeholder.
		
		inst->CalculateSize();
		
		return inst;
	};
};

traffic_vehicle_placeholder::Create();