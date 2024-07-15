class vehicle_ai : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("vehicle_ai", "Node", false, false);
		
		//	TODO: add members and methods to vehicle_ai.
		
		inst->CalculateSize();
		
		return inst;
	};
};

vehicle_ai::Create();