class VehicleRepulsor : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("VehicleRepulsor", "Node", false, false);
		
		//	TODO: add members and methods to VehicleRepulsor.
		
		inst->CalculateSize();
		
		return inst;
	};
};

VehicleRepulsor::Create();