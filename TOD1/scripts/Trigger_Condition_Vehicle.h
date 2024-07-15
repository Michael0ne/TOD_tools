class Trigger_Condition_Vehicle : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Condition_Vehicle", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Condition_Vehicle.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Condition_Vehicle::Create();