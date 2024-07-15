class Trigger_Condition_Counter : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Condition_Counter", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Condition_Counter.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Condition_Counter::Create();