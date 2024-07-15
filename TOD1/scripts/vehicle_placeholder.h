class vehicle_placeholder : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("vehicle_placeholder", "Node", false, false);
		
		//	TODO: add members and methods to vehicle_placeholder.
		
		inst->CalculateSize();
		
		return inst;
	};
};

vehicle_placeholder::Create();