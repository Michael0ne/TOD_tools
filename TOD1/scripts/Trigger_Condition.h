class Trigger_Condition : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Condition", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Condition.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Condition::Create();