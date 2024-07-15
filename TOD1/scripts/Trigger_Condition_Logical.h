class Trigger_Condition_Logical : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Condition_Logical", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Condition_Logical.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Condition_Logical::Create();