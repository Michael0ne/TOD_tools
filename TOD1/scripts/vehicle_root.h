class vehicle_root : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("vehicle_root", "Node", false, false);
		
		//	TODO: add members and methods to vehicle_root.
		
		inst->CalculateSize();
		
		return inst;
	};
};

vehicle_root::Create();