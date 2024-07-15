class vehicle_navigator : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("vehicle_navigator", "Node", false, false);
		
		//	TODO: add members and methods to vehicle_navigator.
		
		inst->CalculateSize();
		
		return inst;
	};
};

vehicle_navigator::Create();