class vehicle_teleport_safe_point : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("vehicle_teleport_safe_point", "Node", false, false);
		
		//	TODO: add members and methods to vehicle_teleport_safe_point.
		
		inst->CalculateSize();
		
		return inst;
	};
};

vehicle_teleport_safe_point::Create();